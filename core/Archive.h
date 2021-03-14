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

  namespace archive{
    static std::vector<std::pair<TString, TString>> gCompileThese;
  }
  
  class Archive{


  public :
    static void ArchiveIt(const TString& finalname, TFile* archiveTo);

    static void ExtractFinalState(const TString& afile,const TString& fsname,const TString& intoDir="chanser_FinalStates");
    
    static TString BareClassName(const TString& className);

    static void doCompileThese();
    
    static std::vector<std::pair<TString, TString>>&  CompileThese();
  private:

    static TDirectory* MakeDir(TFile* high);
    
    static std::vector<TString > ListSourceFiles(const TString& dirname,const TString& finalname);
      
    static void ArchiveSourceFile(const TString& afile);

    static std::vector<TString > Remove(const std::vector<TString >& v1,const std::vector<TString >& v2);

    static Bool_t insertInCompileThese(const TString& filename, const TString& libname);
   };

}
