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
#include <iostream>

namespace chanser{

  namespace loader{
    static std::vector<TString> gCompilesList; //
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

    static TString FullFileName(const TString& filename);
  
    static std::map<TString,TString>& ClassFileMap();
    static std::vector<std::pair<TString,TString>>& CompilesToList();
    static std::vector<TString>& CompilesList();

    
  private :

 
    ClassDef(chanser::Loader,1);
  };
}
