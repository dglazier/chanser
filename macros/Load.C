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
  TString core=CHANSER+"/core";
   
  if(!TString(gInterpreter->GetIncludePath()).Contains(core)){
    gInterpreter->AddIncludePath(core);
    gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),(core).Data()));
    gSystem->Load("$CHANSER/lib/libchanser.so");
  }
  
}
