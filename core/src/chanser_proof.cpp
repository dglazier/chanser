#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include <TProof.h>
#include <TError.h>


int main(int argc, char **argv) {
  gSystem->Load("libEG.so");

  auto Nworkers=argv[1];
  if(TString(Nworkers).Atoi()==0 ) {Fatal("chanser_proof","Must give a number of workers, chanser_proof N Process.C");}


  TRint  *app = new TRint("App", &argc, argv);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("chanser [%d] ");
  app->ProcessLine(".x $CLAS12ROOT/RunRoot/LoadClas12Root.C");
  app->ProcessLine(Form(".x $CLAS12ROOT/RunRoot/LoadProofLib.C(%s)",Nworkers));
  app->ProcessLine(Form("gROOT->SetBatch();"));
  //app->ProcessLine(Form("gProof->Load(\"%s\");",selectorMacro));

  // get the sandbox directroy
  TString sandbox="~/.proof";
  if(TString(gEnv->GetValue("ProofLite.Sandbox",""))!=TString()){
    sandbox=gEnv->GetValue("ProofLite.Sandbox","");
  }
  TString CHANZER=gSystem->Getenv("CHANSER");

  gSystem->Exec(Form("cp %s/lib/libchanser_rdict.pcm  %s/cache/.",CHANZER.Data(),sandbox.Data()));
  gSystem->Exec(Form("cp %s/lib/libchanseractions_rdict.pcm  %s/cache/.",CHANZER.Data(),sandbox.Data()));
  gSystem->Exec(Form("cp %s/lib/libchansermva_rdict.pcm  %s/cache/.",CHANZER.Data(),sandbox.Data()));
  app->ProcessLine(".x $CHANSER/macros/Load.C");
  
  app->ProcessLine(Form("gProof->Load(TString(gSystem->Getenv(\"CHANSER\"))+\"/lib/libchanser.so\",kTRUE)"));
  app->ProcessLine(Form("gProof->Load(TString(gSystem->Getenv(\"CHANSER\"))+\"/lib/libchansermva.so\",kTRUE)"));
  app->ProcessLine(Form("gProof->Load(TString(gSystem->Getenv(\"CHANSER\"))+\"/lib/libchanseractions.so\",kTRUE)"));

  //Wild card for 6.20 =>EG.pcm previous libEG_rdict.pcm
  //  gSystem->Exec(Form("cp %s/lib/libEG_rdict.pcm  %s/cache/libEG_rdict.pcm",gSystem->Getenv("ROOTSYS"),sandbox.Data()));
  gSystem->Exec(Form("cp %s/lib/*EG*.pcm  %s/cache/.",gSystem->Getenv("ROOTSYS"),sandbox.Data()));

  
  gProof->Load(Form("%s/lib/libEG.so",gSystem->Getenv("ROOTSYS")));
  // gSystem->Exec(Form("cp %s/lib/libEG.so  %s/cache/libEG.so",gSystem->Getenv("ROOTSYS"),sandbox.Data()));
  //gProof->Load(Form("%s/cache/libEG.so",sandbox.Data()));
  
  app->Run();

  app->Terminate(0);
  
  return 0;
  
}
