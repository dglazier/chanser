#include "Loader.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TProof.h>
#include <TEnv.h>
#include <iostream>

namespace chanser{


  Bool_t Loader::CompileClass(const TString& classAndFile){
    TString cname(classAndFile(0,classAndFile.First(':')));
    TString fname(classAndFile(classAndFile.First(':')+1,classAndFile.Length()));
    return CompileClass(cname,fname);
  }
  Bool_t Loader::CompileClass(const TString& classname,const TString&  filename){
    auto fullfile= FullFileName(filename);
    std::cout<<"Loader::CompileClass "<<classname<<" from "<<fullfile<<std::endl;
    //Link from this class to its .cpp  file
    ClassFileMap()[classname] = fullfile;
    return Compile(filename);
    
  }
  TString Loader::FullFileName(const TString& filename){
    auto fullfile=filename;
    if(filename.BeginsWith("/")==kFALSE&&filename.BeginsWith("$")==kFALSE)
      fullfile = TString(gSystem->Getenv("PWD"))+"/"+filename;
    return std::move(fullfile);
  }
  Bool_t Loader::Compile(const TString& filename){
    auto &cl= CompilesList();
    auto fullfile= FullFileName(filename);
 
    //if already compiled return true, so OK to continue
    if(std::find( cl.begin(), cl.end(), fullfile)!=cl.end())
      return kTRUE;
    
    cl.push_back(fullfile);
    std::cout<<" Loader::Compile "<<filename<<" "<<CompilesList().size()<<std::endl;
    gROOT->ProcessLine(Form(".L %s+",filename.Data()));
    
    return kTRUE;
  }
  Bool_t Loader::CompileTo(const TString& filename,TString libname){
    auto &cl= CompilesToList();

    //if already compiled return true, so OK to continue
    if( std::find_if( cl.begin(), cl.end(),[&filename](const std::pair<TString, TString>& element){ return element.first == filename;} ) != cl.end()) return kTRUE; //aready compiled 
	   
    cl.push_back(std::make_pair(filename,libname));

    if(libname.BeginsWith("/")==kFALSE&&libname.BeginsWith("$")==kFALSE)
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
      if(libname.Contains(TString(".")+so_ext)==kFALSE){
	libname +=".";
	libname +=so_ext;
      }

  
      gProof->Load(libname,kTRUE);
      
    }

    return kTRUE;
  }
  /////////////////////////////////////////////////////
  ///return the full path and filename for className
  TString Loader::GetClassFile(const TString& className){
    return ClassFileMap()[className];
  }

 
  std::vector<TString>&  Loader::CompilesList(){ std::cout<<"getting "<<loader::gCompilesList.size()<<std::endl;return loader::gCompilesList;}
  std::map<TString,TString>& Loader::ClassFileMap(){return loader::gClassFile;}
  std::vector<std::pair<TString,TString>>& Loader::CompilesToList(){return loader::gCompilesToList;}
 
  // std::vector<TString>& Loader::CompiledList()  {
  //   return chanser::global::gCompilesList;
  // }
}
