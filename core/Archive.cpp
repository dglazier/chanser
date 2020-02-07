#include "Archive.h"
#include <TDirectory.h>
#include <TMacro.h>
#include <TSystem.h>
#include <iostream>
#include <utility>

namespace chanzer{
  

  void Archive::ArchiveIt(TString finalname, TFile* archiveTo){
    auto fileDir=MakeDir(archiveTo);

    auto files = ListSourceFiles("./",std::move(finalname));


    //Copy source dode into root file archive directory
    fileDir->cd();

    std::cout<<" Archive::ArchiveIt got "<<files.size()<<" files " <<std::endl;
    for(auto& fname : files)
      ArchiveSourceFile(fname);


    //move back to toplevel file directory
    archiveTo->cd();
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
	files.emplace_back(direntry);

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


}
