namespace chanser{};
using namespace chanser;
void LoadSkeleton(){
 gSystem->Load("$CHANSER/lib/libskeleton.so");
 TString CHANSER=gSystem->Getenv("CHANSER");
 TString skeleton=CHANSER+"/skeleton";
 TString macros=CHANSER+"/macros";
 gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),(skeleton).Data()));
 gROOT->SetMacroPath(Form("%s:%s",gROOT->GetMacroPath(),(macros).Data()));
 
}
