#include "Loader.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TProof.h>
#include <TEnv.h>
#include <iostream>

namespace chanzer{


  Bool_t Loader::CompileClass(const TString& classAndFile){
    TString cname(classAndFile(0,classAndFile.First(':')));
    TString fname(classAndFile(classAndFile.First(':')+1,classAndFile.Length()));
    return CompileClass(cname,fname);
  }
  Bool_t Loader::CompileClass(const TString& classname,const TString&  filename){
    auto fullfile=filename;
    if(filename.BeginsWith("/")==kFALSE)
      fullfile = TString(gSystem->Getenv("PWD"))+"/"+filename;

    std::cout<<"Loader::CompileClass "<<classname<<" from "<<fullfile<<std::endl;
    chanzer::global::gClassFile[classname] = fullfile;
    return Compile(filename);
    
  }
  Bool_t Loader::Compile(const TString& filename){
    auto &cl= chanzer::global::gCompilesList;

    //if already compiled return true, so OK to continue
    if(std::find( cl.begin(), cl.end(), filename)!=cl.end())
      return kTRUE;
    
    chanzer::global::gCompilesList.push_back(filename);

    gROOT->ProcessLine(Form(".L %s+",filename.Data()));
    
    return kTRUE;
  }
  Bool_t Loader::CompileTo(const TString& filename,TString libname){
    auto &cl= chanzer::global::gCompilesToList;

    //if already compiled return true, so OK to continue
    if( std::find_if( cl.begin(), cl.end(),[&filename](const std::pair<TString, TString>& element){ return element.first == filename;} ) != cl.end()) return kTRUE; //aready compiled 
	   
    chanzer::global::gCompilesToList.push_back(std::make_pair(filename,libname));

    if(libname.BeginsWith("/")==kFALSE)
      libname = TString(gSystem->Getenv("PWD"))+"/"+libname;
    
    if(gSystem->CompileMacro(filename,"",libname)==0) return kFALSE;
    gSystem->Load(libname);

    if(gProof){
      //get path to .h file (assumed same as .cpp file)
      TString dirName = gSystem->DirName(filename); 
      if(!dirName.BeginsWith("/"))
	dirName = TString(gSystem->Getenv("PWD"))+"/"+dirName;
      gProof->AddIncludePath(dirName);

    // get the sandbox directroy
      TString sandbox="~/.proof";
      if(TString(gEnv->GetValue("ProofLite.Sandbox",""))!=TString()){
	sandbox=gEnv->GetValue("ProofLite.Sandbox","");
      }

     
      gSystem->Exec(Form("cp %s/%s*pcm %s/cache/.",gSystem->DirName(libname),gSystem->BaseName(libname),sandbox.Data()));
      
      auto so_ext = gSystem->GetSoExt();
      if(libname.Contains(so_ext)==kFALSE){
	libname +=".";
	libname +=so_ext;
      }

  
      gProof->Load(libname,kTRUE);
      
    }

    return kTRUE;
  }
  TString Loader::GetClassFile(const TString& className){
    return chanzer::global::gClassFile[className];
  }
  // std::vector<TString>& Loader::CompiledList()  {
  //   return chanzer::global::gCompilesList;
  // }
}
