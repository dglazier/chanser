void LoadClas12Root(){
 
  TString CLAS12ROOT=gSystem->Getenv("CLAS12ROOT");
  TString LIB=CLAS12ROOT+"/lib/";
  gSystem->Load(LIB+"liblz4");
  gSystem->Load(LIB+"libHipo4");
  gSystem->Load(LIB+"libClas12Banks");
  gSystem->Load(LIB+"libClas12Root");
 
  gROOT->SetMacroPath(Form("%s:%s/RunRoot/",gROOT->GetMacroPath(),CLAS12ROOT.Data()));

  TString QADB=gSystem->Getenv("QADB");
  if(QADB.Length()!=0){ //For #ifdef CLAS_QADB in header files
    gSystem->AddIncludePath("-DCLAS_QADB");
    gROOT->ProcessLine("#define CLAS_QADB"); //For cling interpreter
  }
  TString CCDB=gSystem->Getenv("CCDB_HOME");
  if(CCDB.Length()!=0){ //For #ifdef CLAS_CCDB in header files
    gSystem->AddIncludePath("-DCLAS_CCDB");
    gROOT->ProcessLine("#define CLAS_CCDB"); //For cling interpreter
  }
  TString RCDB=gSystem->Getenv("RCDB_HOME");
  if(RCDB.Length()!=0){ //For #ifdef CLAS_CCDB in header files
     gSystem->AddIncludePath("-DCLAS_RCDB");
    gSystem->AddIncludePath("-DRCDB_MYSQL");
    gROOT->ProcessLine("#define CLAS_RCDB"); //For cling interpreter
    gROOT->ProcessLine("#define RCDB_MYSQ"); //For cling interpreter
    //gROOT->ProcessLine("#define RCDB_SQLITE"); //For cling interpreter
  }
  //remove aclic warnings for ccdb....
  gSystem->SetAclicMode(TSystem::EAclicMode::kOpt);
  TString optFlags = gSystem->GetFlagsOpt();
  optFlags+=" -Wno-ignored-qualifiers";
  optFlags+=" -Wno-format";
  gSystem->SetFlagsOpt(optFlags.Data());


}
