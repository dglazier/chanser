//////////////////////////////////////////////////////////////
///
///Class:		Archive
///Description:
///
#pragma once
#include <TFile.h>
#include <TString.h>
#include <vector>

namespace chanser{


  class Archive{


  public :
    static void ArchiveIt(const TString& finalname, TFile* archiveTo);

    static void ExtractFinalState(const TString& afile,const TString& fsname);
    
    static TString BareClassName(const TString& className);
    
  private:

    static TDirectory* MakeDir(TFile* high);
    
    static std::vector<TString > ListSourceFiles(const TString& dirname,const TString& finalname);
      
    static void ArchiveSourceFile(const TString& afile);
    
   };

}
