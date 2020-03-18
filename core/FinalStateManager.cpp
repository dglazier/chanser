#include "FinalStateManager.h"
#include <TFile.h>
#include <TSystem.h>
#include <TBenchmark.h>
#include <iostream>

namespace chanser{

  ////////////////////////////////////////////////////////////
  ///Load a configured final state that has been
  ///saved in root file
  Bool_t  FinalStateManager::LoadFinalState(TString fsname,TString filename,TString worker){
    std::cout<<"FinalStateManager::LoadFinalState "<<fsname<<" "<<filename<<std::endl;

    auto file=TFile::Open(filename);
    finalstate_uptr fs;
  
    fs.reset(dynamic_cast<FinalState*>(file->Get(fsname)));
    if(worker!=TString()) worker.Prepend(".");
    fs->SetWorkerName(worker);//in case on PROOF
    fs->SetInputFileName(filename); //for making unique output dir
    
    //functions otherwise handled by constructor
    //The ROOT streamer is called after the constructor
    //so we have to wait to call some initialisations in PostRead
    fs->PostRead();
        
    if(!fs){
      std::cerr<<"FinalStateManager::LoadFinalState(TString fsname,TString filename)"<<std::endl<<"\t\t"<<fsname<<" in file "<<filename<<std::endl;
      return kFALSE;
    }
    cout<<"FinalStateManager::LoadFinalState  "<<_data<<endl;
    fs->SetEventParticles(&_eventParts); //link to data
    fs->SetTruthParticles(&_data->GetTruth()); //link to truth particles
    fs->SetEventInfo(_data->GetEventInfo());
      
    //fs->Init(); //now can initialise topology iterators
    // _finalStates.push_back((fs));
    _finalStates.push_back(std::move(fs));//take a copy

    delete file;
    return kTRUE;
  }

  /////////////////////////////////////////////////////////////
  ///Pass a FinalState which has been created locally
  Bool_t  FinalStateManager::LoadFinalState(finalstate_uptr fs){
    fs->SetEventParticles(&_eventParts);
    fs->SetTruthParticles(&_data->GetTruth());
    fs->SetEventInfo(_data->GetEventInfo());
    _finalStates.push_back(std::move(fs));//take a copy
  }
  ///////////////////////////////////////////////////////////////
  ///Process all events in the hipo file
  void   FinalStateManager::ProcessAll(Long64_t Nmax){
    Init();
    //read event
    gBenchmark->Start("FinalStateManager::ProcessAll");
    Long64_t  counter=0;
    while(_data->InitEvent()){
      //analyse the event
      ProcessEvent();
      counter++;
      if(counter>Nmax) break;
    }
    cout<<" FinalStateManager::ProcessAll() "<<counter<< " events processed from "<<_data->NEventsRead()<<" in file"<<endl;
    gBenchmark->Stop("FinalStateManager::ProcessAll");
    gBenchmark->Print("FinalStateManager::ProcessAll");

    for(auto& fs:_finalStates){
      cout<<"  FinalState "<<fs->GetName()<<" events "<<fs->_nEvents;
      if(fs->_nEvents)cout<<" average perms "<<fs->_TotPerm/fs->_nEvents<<endl;
    }
    //Save all outputs
    EndAndWrite(); 
  }
  ///////////////////////////////////////////////////////////////
  void  FinalStateManager::Init(){
    MakeBaseOutputDir();
    
    for(const auto& fs:_finalStates)
      fs->Init(_baseOutDir);
  }
  void  FinalStateManager::MakeBaseOutputDir(){
    if(!_baseOutDir.BeginsWith('/'))
      _baseOutDir = TString(gSystem->Getenv("PWD"))+"/"+_baseOutDir;
    gSystem->Exec(Form("mkdir -p %s",_baseOutDir.Data()));
    if(!_baseOutDir.EndsWith("/"))
      _baseOutDir.Append('/');
  }
  //////////////////////////////////////////////////////////////
  ///Process event over all final states
  void   FinalStateManager::ProcessEvent(){

    auto eventTopo = _data->eventPids();
    Bool_t doneRead=kFALSE;
    //std::cout<<"FinalStateManager::ProcessEvent() # particles "<<eventTopo.size()<<std::endl;
    for(auto& fs:_finalStates){
      //std::cout<<"FinalStateManager::ProcessEvent() "<<fs->GetName()<<std::endl;
      //See if this final state had any topologies
      //fulfilled by this event
      if(!fs->CheckForValidTopos(eventTopo))
	continue;

      if(!doneRead){ //only read one per event
	//got a valid event, read all data
	_data->ReadEvent();
	//std::cout<<"FinalStateManager::ProcessEvent() read event "<<endl;
	//organise the particle vectors for the event
	if(!_eventParts.ReadEvent(_data->GetParticles()))
	  break; //something wrong with event disegard it

	_data->IsSim() ? fs->SetHasTruth() : fs->SetHasntTruth();
      }


      //process this final state
      fs->ProcessEvent();
    }
  }
  ///////////////////////////////////////////////////////
  //Write output files etc...
  void FinalStateManager::EndAndWrite(){
    for(auto& fs:_finalStates)
      fs->EndAndWrite();
  }


    
}
  
