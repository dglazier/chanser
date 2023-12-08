#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include <TProof.h>
#include <TError.h>

using std::endl;
using std::cout;

int main(int argc, char **argv) {
  gSystem->Load("libEG.so");
  
  //  gEnv->SetValue("ProofLite.Sandbox", "/work/clas12/mesonex/Pass2/chanser_run/.proof/");
  gEnv->SetValue("ProofLite.Sandbox", "$PWD/.proof/");

  cout<<"Starting chanser_proof"<<endl;
  Bool_t ISMAC=kFALSE;
  if(TString(gSystem->Getenv("OSTYPE"))==TString("darwin"))
    ISMAC=kTRUE;

  gSystem->Load("libProof");

  auto Nworkers=argv[1];
  if(TString(Nworkers).Atoi()==0 ) {Fatal("chanser_proof","Must give a number of workers, chanser_proof N Process.C");}   

  int iNworkers = TString(Nworkers).Atoi(); 
  cout<<"Try starting proof, workers = "<<iNworkers<<endl;
 
  //create proof here or does not work on farm nodes!!
  TProof* proof = TProof::Open("://lite",Form("workers=%d",iNworkers));

  if(gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/liblz4.so",kTRUE)==0){}
  // get the sandbox directroy
  TString sandbox="~/.proof";
  if(TString(gEnv->GetValue("ProofLite.Sandbox",""))!=TString()){
    sandbox=gEnv->GetValue("ProofLite.Sandbox","");
  }
  cout<<"chanser_proof : copy files to .proof/cache and load libraries on PROOF"<<endl;
  //  copy pcm files to sandbox/cache
  gSystem->Exec(Form("cp $CLAS12ROOT/lib/libHipo4_rdict.pcm %s/cache/.",sandbox.Data()));
  gSystem->Exec(Form("cp $CLAS12ROOT/lib/libClas12Banks_rdict.pcm %s/cache/.",sandbox.Data()));
  gSystem->Exec(Form("cp $CLAS12ROOT/lib/libClas12Root_rdict.pcm %s/cache/.",sandbox.Data()));

  gSystem->Exec(Form("ls -lh %s/cache/.",sandbox.Data()));

  if(!ISMAC){
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libHipo4.so",kTRUE);
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libClas12Banks.so",kTRUE);
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libClas12Root.so",kTRUE);
  }
  else{
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libHipo4.dylib",kTRUE);
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libClas12Banks.dylib",kTRUE);
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libClas12Root.dylib",kTRUE);

  }


  gProof->Load(TString(gSystem->Getenv("CHANSER"))+"/lib/libchanser.so",kTRUE);
  gProof->Load(TString(gSystem->Getenv("CHANSER"))+"/lib/libchansermva.so",kTRUE);
  gProof->Load(TString(gSystem->Getenv("CHANSER"))+"/lib/libchanseractions.so",kTRUE);
  gProof->Load(TString(gSystem->Getenv("CHANSER"))+"/lib/libchanseractions_rga.so",kTRUE);

  gSystem->Exec(Form("cp %s/lib/*EG*.pcm  %s/cache/.",gSystem->Getenv("ROOTSYS"),sandbox.Data()));  
  gProof->Load(Form("%s/lib/libEG.so",gSystem->Getenv("ROOTSYS")));

  cout<<"chanser_proof : start application "<<endl;

  TRint  *app = new TRint("App", &argc, argv);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("chanser [%d] ");  
  app->ProcessLine(".x $CLAS12ROOT/RunRoot/LoadClas12Root.C");
  app->ProcessLine(Form("gROOT->SetBatch();"));
  
  
  TString CHANZER=gSystem->Getenv("CHANSER");
  
  gSystem->Exec(Form("cp %s/lib/libchanser_rdict.pcm  %s/cache/.",CHANZER.Data(),sandbox.Data()));
  gSystem->Exec(Form("cp %s/lib/libchanseractions_rdict.pcm  %s/cache/.",CHANZER.Data(),sandbox.Data()));
  gSystem->Exec(Form("cp %s/lib/libchanseractions_rga_rdict.pcm  %s/cache/.",CHANZER.Data(),sandbox.Data()));
  gSystem->Exec(Form("cp %s/lib/libchansermva_rdict.pcm  %s/cache/.",CHANZER.Data(),sandbox.Data()));
  app->ProcessLine(".x $CHANSER/macros/Load.C");
  
  cout<<"chanser_proof : Loaded chanser and clas12root setup to application, now run..."<<endl;

  app->Run();
 
  app->Terminate(0);
  
  return 0;
  
}
