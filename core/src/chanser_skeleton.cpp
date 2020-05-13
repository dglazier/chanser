#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>


int main(int argc, char **argv) {

  TString CHANZER=gSystem->Getenv("CHANSER");
  TRint  *app = new TRint("App", &argc, argv);

  TString runMacro;
  TString optName;
  
  //get command line options first check if makeall
  for(Int_t i=1;i<app->Argc();i++){
    TString opt=app->Argv(i);
    if((opt.Contains(".C"))){
      runMacro=opt;
    };
    if((opt.Contains("name="))){
      optName=TString(opt(5,opt.Length()));
    };
  }
  
 // Run the TApplication (not needed if you only want to store the histograms.)
  app->ProcessLine(".x $CHANSER/macros/LoadSkeleton.C");

  
  if(runMacro.Length()){
    std::cout<<"Run skeleton macro :    "<<runMacro<<std::endl;
    if(optName.Length())
      app->ProcessLine(Form(".x %s(\"%s\")",runMacro.Data(),optName.Data()));
    else
      app->ProcessLine(Form(".x %s",runMacro.Data()));
    
  }
  app->Terminate(0);
  
  return 0;
}
