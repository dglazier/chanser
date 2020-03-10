namespace chanser{};
using namespace chanser;
void Load(){
  gSystem->Load("libEG");
  gSystem->Load("$CLAS12ROOT/lib/libHipo4");
  gSystem->Load("$CLAS12ROOT/lib/libClas12Banks");
  gSystem->Load("$CLAS12ROOT/lib/libClas12Root");
  gSystem->Load("$CHANZER/lib/libchanser.so");
}
