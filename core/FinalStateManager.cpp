#include "FinalStateManager.h"
#include <TFile.h>
#include <TSystem.h>
#include <TBenchmark.h>
#include <iostream>
#include "HipoData.h"

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
    
    cout<<"FinalStateManager::LoadFinalState  "<<_data<<endl;
    fs->SetEventParticles(&_eventParts); //link to data
    fs->SetEventInfo(_data->GetEventInfo());
    /*if(_data->IsSim()){
      fs->SetTruthParticles(&_data->GetTruth()); //link to truth particles
      fs->SetHasTruth();
      }*/

    //functions otherwise handled by constructor
    //The ROOT streamer is called after the constructor
    //so we have to wait to call some initialisations in PostRead
    fs->PostRead();
        
    if(!fs){
      std::cerr<<"FinalStateManager::LoadFinalState(TString fsname,TString filename)"<<std::endl<<"\t\t"<<fsname<<" in file "<<filename<<std::endl;
      return kFALSE;
    }
     //fs->Init(); //now can initialise topology iterators
    // _finalStates.push_back((fs));
    _rawFinalStates.push_back(fs.get());
    _finalStates.push_back(std::move(fs));//take a copy

    delete file;
    return kTRUE;
  }

  /////////////////////////////////////////////////////////////
  ///Pass a FinalState which has been created locally
  Bool_t  FinalStateManager::LoadFinalState(finalstate_uptr fs){
    fs->SetEventParticles(&_eventParts);
    fs->SetEventInfo(_data->GetEventInfo());
    /*if(_data->IsSim()){
      fs->SetTruthParticles(&_data->GetTruth()); //link to truth particles
      fs->SetHasTruth();
      }*/
    _rawFinalStates.push_back(fs.get());
    _finalStates.push_back(std::move(fs));//take a copy
    return kTRUE;
  }
  ///////////////////////////////////////////////////////////////
  ///Process all events in the hipo file
  void  FinalStateManager::ProcessAll(Long64_t Nmax){
    
    Init();
    // run and final states now initialised
    // update final states run dependent information
    Notify();
    
    gBenchmark->Start("FinalStateManager::ProcessAll");

    //read event
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
    std::cout<<" FinalStateManager::Init()"<<std::endl;
    
       
    if(_data->IsLund()){ //if reading LUND events only
      for(const auto& fs : _finalStates)
	fs->SetGenerated();
    }
    
    else if(_data->IsSim()){ //if simulated data and reading reconstructed
      std::cout<<" FinalStateManager::Init() Simulated data, allow truth"<<std::endl;
      for(const auto& fs : _finalStates){
	fs->SetTruthParticles(&_data->GetTruth()); //link to truth particles
	fs->SetHasTruth();
      }
    }
    else {
      for(const auto& fs : _finalStates){
	fs->SetHasntTruth();
      }
    }
    
    for(const auto& fs : _finalStates){
      fs->Init(_baseOutDir);
      fs->Print();
    }
    
  }
  ///////////////////////////////////////////////////////////////
  ///Flag we have a new run
  ///We do not actually want to do anything until file is opened
  void  FinalStateManager::Notify(){
    _changeRun=true;
  }
  ///////////////////////////////////////////////////////////////
  ///For run dependent stuff
  void  FinalStateManager::ChangeRun(){
    for(const auto& fs : _finalStates){
      fs->ChangeRun();
    }
    _changeRun=false; //don't call again until anothe Notify
  }
  //////////////////////////////////////////////////////////////
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
    
    //If Notify is called when a new file is opened, _changeRun will be true
    if(_changeRun==true){
      //normally event is only read if FinalState requires it
      //in case we have a new run just read the event now
      _data->ReadEvent();
      // doneRead=kTRUE;
      ChangeRun();
    }
 
    //std::cout<<"FinalStateManager::ProcessEvent() # particles "<<eventTopo.size()<<std::endl;
    Bool_t goodEvent=kFALSE;
    for(auto& fs:_finalStates){
      //std::cout<<"FinalStateManager::ProcessEvent() "<<fs->GetName()<<std::endl;
      //See if this final state had any topologies
      //fulfilled by this event
      if(!fs->CheckForValidTopos(eventTopo))
	continue;

      if(!doneRead){ //only read one per event
	//got a valid event, read all data
	_data->ReadEvent();
	doneRead=kTRUE;
 	//std::cout<<"FinalStateManager::ProcessEvent() read event "<<endl;
	//organise the particle vectors for the event
	if(!_eventParts.ReadEvent(_data->GetParticles()))
	  break; //something wrong with event disegard it

	//	_data->IsSim() ? fs->SetHasTruth() : fs->SetHasntTruth();
      }

      //process this final state
      fs->ProcessEvent();
      if(fs->WasGoodEvent()) goodEvent=kTRUE;
    }
    //if any final state though this was a good event, write it
    if(goodEvent) _data->WriteEvent(); //only if the data can write itself
  }
  ///////////////////////////////////////////////////////
  //Write output files etc...
  void FinalStateManager::EndAndWrite(){
    for(auto& fs:_finalStates)
      fs->EndAndWrite();
  }


    
}
  
