//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///    Interface compile and load macros

#pragma once

#include <TString.h>
#include <vector>
#include <map>
#include <memory>

namespace chanzer{

  namespace global{
    static std::vector<TString> gCompilesList;
    static std::vector<std::pair<TString,TString>> gCompilesToList;
    static std::map<TString,TString> gClassFile;
  }
  
  class Loader{

    
  public :
 
    static  Bool_t CompileClass(const TString& classname,const TString&  filename);
    static Bool_t CompileClass(const TString& classAndFile);
    static Bool_t Compile(const TString& filename);
    static Bool_t CompileTo(const TString& filename,TString libname);
    static TString GetClassFile(const TString& className);
    
    //  static std::vector<TString>& CompiledList();
    
  private :

 
    ClassDef(chanzer::Loader,1);
  };
}
