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

namespace chanser{
  

  void Archive::ArchiveIt(const TString& finalname, TFile* archiveTo){
    auto fileDir=MakeDir(archiveTo);

    std::cout<<"Archive::ArchiveIt "<<gDirectory->GetName()<<" "<<finalname<<" "<<gSystem->DirName(Loader::GetClassFile(finalname))<<" "<<Loader::GetClassFile(BareClassName(finalname))<<std::endl;

    //get files associcated with finalstate finalname
    auto files = ListSourceFiles(gSystem->DirName(Loader::GetClassFile(BareClassName(finalname))),finalname);
    //Copy source code into root file archive directory
    fileDir->cd();
    for(auto& fname : files)
      ArchiveSourceFile(fname);

    //get other users own classes
    auto otherFiles = Remove(Loader::CompilesList(),files);
    
    //std::cout<<"Archive "<<Loader::CompilesList().size()<<" "<<otherFiles.size() << " "<<files.size()<<std::endl;
    for(auto& fname : otherFiles){
      auto classesDir=fileDir->mkdir("Other");
      classesDir->cd();
      ArchiveSourceFile(fname);
      //and include header file, assume .h
      auto hfile=fname;
      auto suffix=hfile(hfile.Last('.'),hfile.Length()-hfile.Last('.'));
      hfile.ReplaceAll(suffix,".h");						      ArchiveSourceFile(hfile);
    }
    //move back to toplevel file directory
    archiveTo->cd();
  }
  ////////////////////////////////////////////////////////
  ///return elements of v1 which are not in v2
  std::vector<TString >  Archive::Remove(const std::vector<TString >& v1,const std::vector<TString >& v2){
      std::cout<<"Archive::Remove from "<<std::endl;
      for(auto& name : v1)
	std::cout<<" "<<name;
      std::cout<<std::endl;
      for(auto& name : v2)
	std::cout<<" "<<name;
      std::cout<<std::endl;
      
    std::vector<TString > v3;
    std::copy_if(v1.begin(), v1.end(), std::back_inserter(v3),
     [&v2](const TString& arg)
     { return (std::find(v2.begin(), v2.end(), arg) == v2.end());});
    
    for(auto& name : v3)
      std::cout<<" "<<name;
    std::cout<<std::endl;
    std::cout<<"Archive::Remove found "<<v3.size()<<std::endl;
    return std::move(v3);
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
      //get the full filename
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
    TString actualName=afile;
    std::cout<<"ArchiveSourceFile "<<afile<<std::endl;
    if(actualName.Contains("$CHANSER_CLASSES"))
      actualName.ReplaceAll("$CHANSER_CLASSES",gSystem->Getenv("CHANSER_CLASSES"));
    TMacro m(actualName);
    m.SetName(gSystem->BaseName(actualName));
    m.Write();
  }

  void Archive::ExtractFinalState(const TString& afile,const TString& fsname){

    //returns 0 if new dir -1 if exists
    //filesystem top level directory for all final states processed here
    gSystem->MakeDirectory(Form("chanser_FinalStates/"));
    
    //open the root file with this finalstate
    auto arxiv=std::unique_ptr<TFile>{TFile::Open(afile)};

    //get the required code from the Archive
    auto finalStateDir=dynamic_cast<TDirectoryFile*>(arxiv->Get("Archive"));
    
    auto chfsname=fsname.Data(); //final state name as const char*
    
    //get final state object key so can access full class name with namespace
    auto keys=arxiv->GetListOfKeys();
    //e.g. final state name Pi4, name of this class has namespace $USER e.g dglazier
    //Name in file = final state name = Pi4, class name has namespace =dglazier::Pi4
    auto classname=TString{(dynamic_cast<TKey*>(keys->FindObject(chfsname)))->GetClassName()};
    auto userNamespace = classname(0,classname.First(':'));
    if(classname.Contains("::"))//has namespace
      classname.ReplaceAll("::","_");//for filename e.g. dglazier_Pi4
    
    std::cout<<"ExtractFinalState "<<classname<<" "<<chfsname<<std::endl;
    
    //filesystem directory for this specific final state
    TString topFinalState = Form("chanser_FinalStates/%s/",classname.Data());
    if(gSystem->MakeDirectory(topFinalState)==0){ 
      //Only allow one instance of this class from this USER
      
      auto chclassname=classname.Data();
      auto cpp= dynamic_cast<TMacro*>(finalStateDir->Get(Form("%s.cpp",chfsname)));
      if(cpp==nullptr) {
	std::cout<<"Error,  Archive::ExtractFinalState, cannot find "<<chfsname<<" in Archive"<<std::endl;
	finalStateDir->ls();
	return;
      }
      
      cpp->SaveSource(Form("%s/%s.cpp",topFinalState.Data(),chfsname));
    
      auto h= dynamic_cast<TMacro*>(finalStateDir->Get(Form("%s.h",chfsname)));
      h->SaveSource(Form("%s/%s.h",topFinalState.Data(),chfsname));
      
      auto data= dynamic_cast<TMacro*>(finalStateDir->Get(Form("TreeData%s.h",chfsname)));
      data->SaveSource(Form("%s/TreeData%s.h",topFinalState.Data(),chfsname));
      
      //Compile this final state into a library with $USER namespace
      //included in lib name and put in chanser_FinalStates
      //Loader::CompileTo(Form("chanser_FinalStates/%s/%s.cpp",chclassname,chfsname),Form("chanser_FinalStates/%s",chclassname));
      insertInCompileThese(Form("%s/%s.cpp",topFinalState.Data(),chfsname),Form("chanser_FinalStates/%s",chclassname));
    }
    //This instance of the Final state class may use additional other
    //classes so extract them if required
    auto otherDir=dynamic_cast<TDirectoryFile*>(finalStateDir->Get("Other"));
    std::cout<<"GOING TO Adding Other source code "<<otherDir<<std::endl;
    if(otherDir){
      gSystem->MakeDirectory(Form("chanser_FinalStates/%s/Other",classname.Data()));
      //loop over all files in Other and save source if not already  present
      auto otherKeys=otherDir->GetListOfKeys();
      std::cout<<"GOING TO Adding Other source code keys"<<otherKeys->GetEntries()<<std::endl;
   
      //TIter nextOther(otherKeys);
      // while( (otherMacro=dynamic_cast<TMacro*>(nextOther())) ){
      for(Int_t iother=0;iother<otherKeys->GetEntries();iother++){
	std::cout<<"Adding Other source code "<<otherKeys->At(iother)->GetName()<<" "<<otherKeys->At(iother)->ClassName()<<" "<<otherKeys->At(iother)->Class_Name()<<std::endl;
	//TObject* otherObject=dynamic_cast<TObject*>(otherKeys->At(iother));
	//std::cout<<"Adding Other source code "<<otherMacro->GetName()<<std::endl;
	TMacro* otherMacro=dynamic_cast<TMacro*>(otherDir->Get(otherKeys->At(iother)->GetName()));
	std::cout<<"Adding Other source code "<<otherMacro->GetName()<<std::endl;
	//test if file exists
	if( gSystem->Which(Form("%s/Other/",topFinalState.Data()),gSystem->BaseName(otherMacro->GetName())) )
	  continue;
	//Not there yet, so extract
	TString extractedOtherName=(Form("%s/Other/%s",topFinalState.Data(),gSystem->BaseName(otherMacro->GetName())));
	otherMacro->SaveSource(extractedOtherName);
	if(extractedOtherName.Contains(".h"))
	  continue; //only compile .cpp etc
	//Check if this class has already been scheduled for compilation
	//from another final state
	
	//from the library name from file base name without suffix
	TString tempName=gSystem->BaseName(otherMacro->GetName());
	tempName=userNamespace+"_"+tempName(0,tempName.Last('.'));
	
	auto libname = Form("chanser_FinalStates/%s",tempName.Data());

	insertInCompileThese(extractedOtherName,libname);
      }
    }
  }
  //////////////////////////////////////////////////////
  /// List of classes to compile to be give to HipoProcessor
  /// filename (to be compiled) , libname (to be compiled to)
  std::vector<std::pair<TString, TString>>&  Archive::CompileThese(){
    return archive::gCompileThese;
  }
  //////////////////////////////////////////////////////
  ///compile each item in CompileThese
  void Archive::doCompileThese(){
    std::cout<<"doCompileThese() "<<CompileThese().size()<<std::endl;
    for(auto& compile:CompileThese())
      Loader::CompileTo(compile.first,compile.second);
  }
  /////////////////////////////////////////////////////
  ///Check if this library is already scheduled for compilation
  Bool_t Archive::insertInCompileThese(const TString& filename, const TString& libname){
    auto& cl=archive::gCompileThese;
    std::cout<<"insetCompile "<<CompileThese().size()<<" "<<filename<<" "<<libname<<std::endl;
    
    if( std::find_if( cl.begin(), cl.end(),[&libname](const std::pair<TString, TString>& element){ return element.second == libname;} ) != cl.end())
      return kFALSE; //already there
    
    std::cout<<"insetCompile pushing back "<<CompileThese().size()<<std::endl;
    cl.push_back(std::make_pair(filename, libname));
    return kTRUE;
    
  }
}
