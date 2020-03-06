#include "HipoProcessor.h"
#include "Archive.h"
#include <TH2.h>
#include <TSystem.h>
#include <TProofServ.h>

namespace chanzer{
  
 
  HipoProcessor::HipoProcessor(clas12root::HipoChain* chain,TString fsfile) : HipoSelector(chain) {
    chain->GetNRecords();
    _listOfFinalStates=new TList();
    _listOfFinalStates->SetName("LISTOFFINALSTATES");
    _listOfFinalStates->SetOwner(kTRUE);
    LoadFinalStates(fsfile);
  }
  HipoProcessor::~HipoProcessor() {
    if(_listOfFinalStates) delete _listOfFinalStates;_listOfFinalStates=nullptr;
  }
  void HipoProcessor::Begin(TTree * /*tree*/)
  {

    //Give list of final states to input so it can be initiated on slaves
    fInput->Add(_listOfFinalStates);//make chain of files avaialbel on slaves

    HipoSelector::Begin(0); //Do not remove this line!


  }
  
  void HipoProcessor::SlaveBegin(TTree * /*tree*/)
  {
    HipoSelector::SlaveBegin(0); //Do not remove this line!

    _fsm.LoadData(&_hipo);
    //now initiliase all final states
    _listOfFinalStates=(dynamic_cast<TList*>(fInput->FindObject("LISTOFFINALSTATES"))); //      _hipo.SetReader(_c12.get());
    for(Int_t ifs=0;ifs<_listOfFinalStates->GetEntries();++ifs){
      TString workerName;
      if(gProofServ) workerName=gProofServ->GetOrdinal();
      _fsm.LoadFinalState(_listOfFinalStates->At(ifs)->GetName(),_listOfFinalStates->At(ifs)->GetTitle(),workerName);
    }

	 
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

  }

  void HipoProcessor::LoadFinalStates(TString fsfile){

#include <fstream>
    std::ifstream infile(fsfile.Data());
    std::string fss, fis; //finalstate name and root filename
    while (infile >> fss >> fis){
      std::cout<<"HipoProcessor::LoadFinalStates : "<<fss << " "<<fis<<std::endl;
      _listOfFinalStates->Add(new TNamed(fss,fis));
      Archive::ExtractFinalState(fis,fss);
    }
  }
}
  
