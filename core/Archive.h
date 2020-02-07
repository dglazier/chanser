//////////////////////////////////////////////////////////////
///
///Class:		Archive
///Description:
///
#pragma once
#include <TFile.h>
#include <TString.h>
#include <vector>

namespace chanzer{


  class Archive{


  public :
    static void ArchiveIt(TString finalname, TFile* archiveTo);

  private:
    static TDirectory* MakeDir(TFile* high);
		    
    static std::vector<TString > ListSourceFiles(const TString& dirname,const TString& finalname);
      
    static void ArchiveSourceFile(const TString& afile);

  };

}
