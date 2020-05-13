#include "SkeletonClass.h"
#include <TSystem.h>
#include <TError.h>
#include <iostream>

namespace chanser{
 
  void SkeletonClass::MakeCode(){

    TString CHANSER=gSystem->Getenv("CHANSER");
    TString classDir=gSystem->Getenv("CHANSER_CLASSES");
    auto userName=gSystem->Getenv("USER");


    auto dotcpp = _className+".cpp";
    auto doth = _className+".h";
  
 
    //check if code already exists
    TString checkFile=doth;//Form("%s.cpp",_header.Data());
    if(gSystem->FindFile(classDir.Data(),checkFile)){
      TString sOverwrite;
      Info("FSSkeleton::MakeCode()","Class Code already exists to exit type y, to overwrite type anything else");
      std::cin>>sOverwrite;;
      if(sOverwrite==TString("y")) exit(0);
    }

    
    //Copy template class header file
    gSystem->Exec(Form("cp $CHANSER/skeleton/template%s.cpp $CHANSER_CLASSES/%s",_templateName.Data(),dotcpp.Data()));
    gSystem->Exec(Form("cp $CHANSER/skeleton/template%s.h $CHANSER_CLASSES/%s",_templateName.Data(),doth.Data()));

    //Open the header file
    _curMacro=TMacro(Form("%s/%s",classDir.Data(),doth.Data()));

    //Replace templateFS with new class name
    ReplaceAllMacroText("templateClass",_className);
    ReplaceAllMacroText("USERNAME",userName);
 
   //Save header code
    _curMacro.SaveSource(Form("%s/%s",classDir.Data(),doth.Data()));

    
    //Open the cpp file
    _curMacro=TMacro(Form("%s/%s",classDir.Data(),dotcpp.Data()));

    //Replace templateClass with new class name
    ReplaceAllMacroText("templateClass",_className);
    ReplaceAllMacroText("USERNAME",userName);
 
    //Save cpp code
    _curMacro.SaveSource(Form("%s/%s",classDir.Data(),dotcpp.Data()));

    //make a class loader if it does not exist
    
    _curMacro=TMacro(Form("%s/LoadMyClasses.C",classDir.Data()));
     if(_curMacro.GetListOfLines()->GetEntries()==0){
       _curMacro.SaveSource(Form("%s/LoadMyClasses.C",classDir.Data()));

      //First class create loader
      ContinueLineAfter("{");
      ContinueLineAfter("//Add your classes here");
      ContinueLineAfter("}");
      
    }
    if(_curMacro.GetLineWith(dotcpp) )
      return; //already included this class

    AddLineAfter("//Add your classes here",Form("Loader::Compile(\"$CHANSER_CLASSES/%s\");",dotcpp.Data()));
   _curMacro.SaveSource(Form("%s/LoadMyClasses.C",classDir.Data()));
 
    return;
  }
}
