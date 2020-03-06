#include "Archive.h"
#include "Loader.h"
#include <TDirectory.h>
#include <TMacro.h>
#include <TSystem.h>
#include <TEnv.h>
#include <TROOT.h>
#include <TProof.h>
#include <TList.h>
#include <TKey.h>
#include <iostream>
#include <utility>

namespace chanzer{
  

  void Archive::ArchiveIt(const TString& finalname, TFile* archiveTo){
    auto fileDir=MakeDir(archiveTo);

    std::cout<<"archiveTo "<<finalname<<" "<<gSystem->DirName(Loader::GetClassFile(finalname))<<" "<<Loader::GetClassFile(BareClassName(finalname))<<std::endl;

    auto files = ListSourceFiles(gSystem->DirName(Loader::GetClassFile(BareClassName(finalname))),std::move(finalname));


    //Copy source dode into root file archive directory
    fileDir->cd();

    std::cout<<" Archive::ArchiveIt got "<<files.size()<<" files " <<std::endl;
    for(auto& fname : files)
      ArchiveSourceFile(fname);


    //move back to toplevel file directory
    archiveTo->cd();
  }
  
  TString Archive::BareClassName(const TString& className){
    //Take name as class name without any namespaces
    return className(className.Last(':')+1,className.Length()); 
  }
  
  TDirectory* Archive::MakeDir(TFile* high){
    return high->mkdir("Archive");
  }
      
  std::vector<TString > Archive::ListSourceFiles(const TString& dirname,const TString& finalname){
    void *dirp = gSystem->OpenDirectory(dirname); //open directory on system
    if (!dirp){std::cout<<" ARCHIVE::ListSourceFiles directory "<<dirname <<" not found "<<std::endl; return std::vector<TString>();}

    char *direntry{nullptr};
    Long_t id, size,flags,modtime;

    std::vector<TString > files;
	
    //loop on all entries of this directory
    while ((direntry=(char*)gSystem->GetDirEntry(dirp))) {
      TString afile{Form("%s/%s",dirname.Data(),direntry)};
	  
      gSystem->GetPathInfo(afile,&id,&size,&flags,&modtime);//needed?

      //only copy files containing finalstate 
      if (!strstr(direntry,finalname.Data())) continue;
	  
      if (afile.EndsWith(".c")||
	  afile.EndsWith(".C")||
	  afile.EndsWith(".cpp")||
	  afile.EndsWith(".cxx")||
	  afile.EndsWith(".h")||
	  afile.EndsWith(".hxx")
	  //afile.EndsWith("")
	  ) {
	//add to list  
	files.emplace_back(dirname+"/"+direntry);

      }
 
    }

    return std::move(files);
  }

  void Archive::ArchiveSourceFile(const TString& afile){
    std::cout<<"ArchiveSourceFile( "<<afile<<std::endl;
    TMacro m(afile);
    m.SetName(gSystem->BaseName(afile));
    m.Write();
  }

  void Archive::ExtractFinalState(const TString& afile,const TString& fsname){
  
    gSystem->MakeDirectory(Form("chanzer_FinalStates/"));
    
    auto arxiv=std::unique_ptr<TFile>{TFile::Open(afile)};
    
    auto classDir=dynamic_cast<TDirectoryFile*>(arxiv->Get("Archive"));
    auto chfsname=fsname.Data(); //final state name as const char*
     std::cout<<"ExtractFinalState "<<chfsname<<std::endl;
   //get final state object key so can access full class name with namespace
    auto keys=arxiv->GetListOfKeys();
    auto classname=TString{(dynamic_cast<TKey*>(keys->FindObject(chfsname)))->GetClassName()};
    if(classname.Contains("::"))//has namespace
      classname.ReplaceAll("::","_");//for filename
    
    std::cout<<"ExtractFinalState "<<classname<<" "<<chfsname<<std::endl;
    gSystem->MakeDirectory(Form("chanzer_FinalStates/%s/",classname.Data()));
 
    auto chclassname=classname.Data();
    auto cpp= dynamic_cast<TMacro*>(classDir->Get(Form("%s.cpp",chfsname)));
    if(cpp==nullptr) {
      std::cout<<"Error,  Archive::ExtractFinalState, cannot find "<<chfsname<<" in Archive"<<std::endl;
      classDir->ls();
      return;
    }
   
    cpp->SaveSource(Form("chanzer_FinalStates/%s/%s.cpp",chclassname,chfsname));
    
    auto h= dynamic_cast<TMacro*>(classDir->Get(Form("%s.h",chfsname)));
    h->SaveSource(Form("chanzer_FinalStates/%s/%s.h",chclassname,chfsname));

    auto data= dynamic_cast<TMacro*>(classDir->Get(Form("TreeData%s.h",chfsname)));
    data->SaveSource(Form("chanzer_FinalStates/%s/TreeData%s.h",chclassname,chfsname));


    Loader::CompileTo(Form("chanzer_FinalStates/%s/%s.cpp",chclassname,chfsname),Form("chanzer_FinalStates/%s",chclassname));
    //gROOT->ProcessLine(Form(".x chanzer_FinalStates/%s.cpp++",chfsname));
    // auto libName=Form("chanzer_FinalStates/%s",chclassname);
    // gSystem->CompileMacro(Form("chanzer_FinalStates/%s/%s.cpp",chclassname,chfsname),"",libName);
    // gSystem->Load(libName);
    
    // if(gProof){
    //   // gProof->AddIncludePath(Form("%s/chanzer_FinalStates/",gSystem->Getenv("PWD")));
    //   // gProof->Load(Form("chanzer_FinalStates/%s.cpp+",chfsname),kTRUE);
      
    // }
  }


}
