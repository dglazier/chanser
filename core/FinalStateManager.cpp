#include "FinalStateManager.h"
#include <TFile.h>
#include <iostream>

namespace chanzer{

  ////////////////////////////////////////////////////////////
  ///Load a configured final state that has been
  ///saved in root file
  Bool_t  FinalStateManager::LoadFinalState(TString fsname,TString filename,TString worker){
    std::cout<<"ehh FinalStateManager::LoadFinalState "<<fsname<<" "<<filename<<" ooo"<<std::endl;
    auto file=TFile::Open(filename);
    finalstate_uptr fs;
  
    fs.reset(dynamic_cast<FinalState*>(file->Get(fsname)));  
    fs->SetWorkerName(worker);//in case on PROOF
      
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
  void   FinalStateManager::ProcessAll(){
    Init();
    //read event
    Long64_t  counter=0;
    while(_data->InitEvent()){
      //analyse the event
      ProcessEvent();
      counter++;
      //	if(counter>100) break;
    }
    cout<<" FinalStateManager::ProcessAll() "<<counter<< " events processed from "<<_data->NEventsRead()<<" in file"<<endl;
    for(auto& fs:_finalStates){
      cout<<"  FinalState "<<fs->GetName()<<" events "<<fs->_nEvents;
      if(fs->_nEvents)cout<<" average perms "<<fs->_TotPerm/fs->_nEvents<<endl;
    }
  }
  ///////////////////////////////////////////////////////////////
  void   FinalStateManager::Init(){
    for(const auto& fs:_finalStates)
      fs->Init();
  }
  //////////////////////////////////////////////////////////////
  ///Process event over all final states
  void   FinalStateManager::ProcessEvent(){

    auto eventTopo = _data->eventPids();
    Bool_t doneRead=kFALSE;
     
    for(auto& fs:_finalStates){
      //See if this final state had any topologies
      //fulfilled by this event
      if(!fs->CheckForValidTopos(eventTopo))
	continue;

      if(!doneRead){ //only read one per event
	//got a valid event, read all data
	_data->ReadEvent();
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
  
