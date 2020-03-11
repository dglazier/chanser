/**
 \class Skeleton
 
 Class to generate skeleton code
 
*/

#include "Skeleton.h"
#include <TSystem.h>
#include <TError.h>
#include <iostream>

namespace chanser{
  using std::cout;
  using std::endl;
  
  //////////////////////////////////////////////////////
  void Skeleton::AddLineAfter(TString line0,TString line1,Int_t off){
    TList *lines=_curMacro.GetListOfLines();
    MoveToLine(line0);
    _place=_place+1+off;
    lines->AddAt(new TObjString(line1),_place);
  }

  void Skeleton::ContinueLineAfter(TString line1,Int_t off){
    TList *lines=_curMacro.GetListOfLines();
    _place=_place+1+off;
    lines->AddAt(new TObjString(line1),_place);
  }

  void Skeleton::MoveToLine(TString line0){
    TList *lines=_curMacro.GetListOfLines();
    TObject* obj=_curMacro.GetLineWith(line0);
    if(!obj) Error("Skeleton::MoveToLine","Line %s does not exist in %s",line0.Data(),"file");
    _place=lines->IndexOf(obj); //get line number
  }

  TString Skeleton::FindNextLineLike(TString linelike){
    TList *lines=_curMacro.GetListOfLines();
    TObjString* thisline=0;
    Int_t count=0;
    for(count=_place;count<lines->GetEntries();count++){
      thisline=dynamic_cast<TObjString*>(lines->At(count));
      if(thisline->String().Contains(linelike))
	break;
    }
    if(count==lines->GetEntries()) {_place=count; return "";} //didn't find line go to end
    else _place=count; //get line number
    return thisline->String();
  }

  void Skeleton::ReplaceInCurrLine(TString text0,TString text1){
    // TString strline=_curMacro.GetLineWith(text0)->GetString();
    TList *lines=_curMacro.GetListOfLines();
    TObjString*  thisline=dynamic_cast<TObjString*>(lines->At(_place));
    if(!thisline->String().Contains(text0)) cout<<"Warning : ReplaceInCurrLine text not found "<<text0<<" in line "<<thisline->String()<<endl;
    thisline->String().ReplaceAll(text0,text1);
  }

  void Skeleton::ReplaceMacroText(TString text0,TString text1){
    TString strline=_curMacro.GetLineWith(text0)->GetString();
    strline.ReplaceAll(text0,text1);
    _curMacro.GetLineWith(text0)->SetString(strline);
  }

  void Skeleton::ReplaceAllMacroText(TString text0,TString text1){
    TList *lines=_curMacro.GetListOfLines();
    TObjString* thisline=0;
    Int_t count=0;
    for(count=_place;count<lines->GetEntries();count++){
      thisline=dynamic_cast<TObjString*>(lines->At(count));
      if(thisline->String().Contains(text0))
	thisline->SetString(thisline->String().ReplaceAll(text0,text1));
    }

  }
}
