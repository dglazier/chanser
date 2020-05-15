#include <TSystem.h>
#include <TString.h>
#include <TInterpreter.h>
#include <TROOT.h>
#include <iostream>

namespace chanser{};
using namespace chanser;

void Load(){
  gSystem->Load("libEG");
  gSystem->Load("$CLAS12ROOT/lib/libHipo4");
  gSystem->Load("$CLAS12ROOT/lib/libClas12Banks");
  gSystem->Load("$CLAS12ROOT/lib/libClas12Root");

  TString CHANSER=gSystem->Getenv("CHANSER");
  TString classes=gSystem->Getenv("CHANSER_CLASSES");
  TString core=CHANSER+"/core";
  TString actions=CHANSER+"/actions";
  TString skeleton=CHANSER+"/skeleton";
  TString macros=CHANSER+"/macros";
  TString user= gSystem->Getenv("USERNAME");
  if(user>Length()==0) user= gSystem->Getenv("USER");
    
  if(!TString(gInterpreter->GetIncludePath()).Contains(core)){
    
    gInterpreter->AddIncludePath(core);
    gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),(core).Data()));
    gSystem->Load("$CHANSER/lib/libchanser.so");
    
    gInterpreter->AddIncludePath(actions);
    gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),(actions).Data()));
    gSystem->Load("$CHANSER/lib/libchanseractions.so");

    gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),(skeleton).Data()));

    gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),(macros).Data()));

    if(classes.Length()&&(gProof==nullptr))
      gROOT->ProcessLine(".x $CHANSER_CLASSES/LoadMyClasses.C");

    //LEt the user namespace be used locally in chanser_root
    if(gProof==nullptr){
      gROOT->ProcessLine(Form("namespace %s{};",user.Data()));
      gROOT->ProcessLine(Form("using namespace %s;",user.Data()));
    }
  }
  
}
