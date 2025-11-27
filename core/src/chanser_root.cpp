#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include <TProof.h>


int main(int argc, char **argv) {
  gSystem->Load("libEG.so");

  TString finalState(argv[1]);

  TRint  *app = new TRint("App", &argc, argv);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("chanser [%d] ");
  app->ProcessLine(".x $CLAS12ROOT/RunRoot/LoadClas12Root.C");
  //Compile and Load any given final state
  

  TString CHANZER=gSystem->Getenv("CHANSER");
 
   app->ProcessLine(".x $CHANSER/macros/Load.C");

 
   std::cout<<"Loading Final State class : "<<finalState<<std::endl;
   if(finalState.Length()&&finalState.Contains(':')){
     app->ProcessLine(Form("Loader::CompileClass(\"%s\")",finalState.Data()));
   }
  app->Run();

  //app->Terminate(0);
  
  return 0;
  
}
