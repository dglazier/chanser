#include <iostream>
#include <TRint.h>
#include <TError.h>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include <TMVA/Tools.h>


int main(int argc, char **argv) {

 
  TString CHANZER=gSystem->Getenv("CHANSER");
  if(CHANZER.Length()==0)
    Fatal("chanser_mva","need to set the environment  variable CHANSER");
  
  TString finalState(argv[1]);

  TRint  *app = new TRint("App", &argc, argv);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("chanser_mva [%d] ");
  app->ProcessLine(".x $CHANSER/macros/Load.C");


   gSystem->Load("libTMVA.so");

   app->ProcessLine("TMVA::Tools::Instance();");
   
   gSystem->Load(Form("%s/lib/libchansermva.so",CHANZER.Data()));
   app->ProcessLine("using namespace chanser;");
   app->ProcessLine("using namespace chanser::mva;");
 
   app->Run();

  //app->Terminate(0);
  
  return 0;
  
}
