#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include <TProof.h>


int main(int argc, char **argv) {
  gSystem->Load("libEG.so");

  auto Nworkers=argv[1];
  //auto selectorMacro=argv[2];
  TRint  *app = new TRint("App", &argc, argv);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("chanser [%d] ");
  app->ProcessLine(".x $CLAS12ROOT/RunRoot/LoadClas12Root.C");
  app->ProcessLine(Form(".x $CLAS12ROOT/RunRoot/LoadProofLib.C(%s)",Nworkers));
  //app->ProcessLine(Form("gProof->Load(\"%s\");",selectorMacro));

  // get the sandbox directroy
  TString sandbox="~/.proof";
  if(TString(gEnv->GetValue("ProofLite.Sandbox",""))!=TString()){
    sandbox=gEnv->GetValue("ProofLite.Sandbox","");
  }
  TString CHANZER=gSystem->Getenv("CHANZER");

  gSystem->Exec(Form("cp %s/lib/libchanser_rdict.pcm  %s/cache/.",CHANZER.Data(),sandbox.Data()));
  app->ProcessLine(".x $CHANZER/macros/Load.C");
  
  app->ProcessLine(Form("gProof->Load(TString(gSystem->Getenv(\"CHANZER\"))+\"/lib/libchanser.so\",kTRUE)"));


  gSystem->Exec(Form("cp %s/lib/libEG_rdict.pcm  %s/cache/.",gSystem->Getenv("ROOTSYS"),sandbox.Data()));
  gProof->Load(Form("%s/lib/libEG.so",gSystem->Getenv("ROOTSYS")));
  
  app->Run();

  app->Terminate(0);
  
  return 0;
  
}
