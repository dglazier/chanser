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

  std::cout<<"1 loading lib "<<Form("%s/lib/libchansermva.so",CHANZER.Data())<<std::endl;


  TRint  *app = new TRint("App", &argc, argv);
// Run the TApplication (not needed if you only want to store the histograms.)
  app->SetPrompt("chanser_mva [%d] ");
  app->ProcessLine(".x $CHANSER/macros/Load.C");

  std::cout<<"2 loading lib "<<Form("%s/lib/libchansermva.so",CHANZER.Data())<<std::endl;

   gSystem->Load("libTMVA.so");
  std::cout<<"3 loading lib "<<Form("%s/lib/libchansermva.so",CHANZER.Data())<<std::endl;

   app->ProcessLine("TMVA::Tools::Instance();");
   std::cout<<" loading lib "<<Form("%s/lib/libchansermva.so",CHANZER.Data())<<std::endl;
   gSystem->Load(Form("%s/lib/libchansermva.so",CHANZER.Data()));
   app->ProcessLine("using namespace chanser;");
   app->ProcessLine("using namespace chanser::mva;");
 
   app->Run();

  //app->Terminate(0);
  
  return 0;
  
}
