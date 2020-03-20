#include "HipoProcessor.h"
#include "Archive.h"
#include <TH2.h>
#include <TSystem.h>
#include <TProofServ.h>

namespace chanser{
  
 
  HipoProcessor::HipoProcessor(clas12root::HipoChain* chain,TString fsfile,TString base) : HipoSelector(chain),_baseDir(base) {
    chain->GetNRecords();//needed to count total records etc.

    if(!fsfile.BeginsWith('/'))
      fsfile = TString(gSystem->Getenv("PWD"))+"/"+fsfile;
    GetFinalStates(fsfile); //read which final states to process from text file
    Info(" HipoProcessor::HipoProcessor",Form(" from %s found %d entries",fsfile.Data(),_listOfFinalStates->GetEntries()));
  }
  HipoProcessor::~HipoProcessor() {
    if(_listOfFinalStates) delete _listOfFinalStates;_listOfFinalStates=nullptr;
  }
  void HipoProcessor::Begin(TTree * /*tree*/)
  {

    //Give list of final states to input so it can be initiated on slaves
    fInput->Add(_listOfFinalStates);//make chain of files avaialbel on slaves
    //Give the base output directory
    fInput->Add(new TNamed("FSBASEDIR",_baseDir.Data()));
    
    HipoSelector::Begin(0); //Do not remove this line!


  }
  
  void HipoProcessor::SlaveBegin(TTree * /*tree*/)
  {
    HipoSelector::SlaveBegin(0); //Do not remove this line!

    _fsm.LoadData(&_hipo);
    //now initiliase all final states
    _listOfFinalStates=(dynamic_cast<TList*>(fInput->FindObject("LISTOFFINALSTATES"))); //      _hipo.SetReader(_c12.get());

    cout<<"HipoProcessor::SlaveBegin "<<_listOfFinalStates->GetEntries()<<endl;
    for(Int_t ifs=0;ifs<_listOfFinalStates->GetEntries();++ifs){
      TString workerName;
      if(gProofServ) workerName=gProofServ->GetOrdinal();
      _fsm.LoadFinalState(_listOfFinalStates->At(ifs)->GetName(),_listOfFinalStates->At(ifs)->GetTitle(),workerName);
    }

    //Get the output directory
    _baseDir=(dynamic_cast<TNamed*>(fInput->FindObject("FSBASEDIR")))->GetTitle();
    _fsm.SetBaseOutDir(_baseDir);

    _fsm.GetEventParticles().SetMaxParticles(6);
    _fsm.Init();
     
  }
  
  void HipoProcessor::AddFilter(){
    //   _c12->addExactPid(11,1);    //exactly 1 electron
  }

  Bool_t HipoProcessor::ProcessEvent(){
    //Equivalent to TSelector Process
    //Fill in what you would like to do with
    //the data for each event....
    _hipo.SetReader(_c12.get());
    // cout<<"HipoProcessor::ProcessEvent"<<endl;
    if(_hipo.FetchPids()){
      _fsm.ProcessEvent();
    }

    return kTRUE;
  }
 
  void HipoProcessor::SlaveTerminate()
  {
    _fsm.EndAndWrite();
  }

  void HipoProcessor::Terminate()
  {
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.


    //Tidy up
    gROOT->ProcessLine(".! rm -r chanser_FinalStates/");
  }

  void HipoProcessor::GetFinalStates(TString fsfile){
    _listOfFinalStates=new TList();
    _listOfFinalStates->SetName("LISTOFFINALSTATES");
    _listOfFinalStates->SetOwner(kTRUE);

#include <fstream>
    std::ifstream infile(fsfile.Data());
    
    if(!infile.is_open()) {
      const char* messa=Form("No final states list file found %s",fsfile.Data());
      Fatal("HipoProcessor::GetFinalStates",messa,"");
    }
    cout<<" HipoProcessor::GetFinalStates("<<endl;
    std::string fss, fis; //finalstate name and root filename
    while (infile >> fss >> fis){
      if(fss[0]=='#') //comment out with #
      	continue; 
      std::cout<<"HipoProcessor::LoadFinalStates : "<<fss << " "<<fis<<std::endl;
      _listOfFinalStates->Add(new TNamed(fss,fis));
      Archive::ExtractFinalState(fis,fss); //finalstate name, filename (full path)
    }
    //Do any compilation that is needed
    Archive::doCompileThese();
  }
}
  
