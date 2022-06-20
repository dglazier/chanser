//////////////////////////////////////////////////////////////
///
///Class:		HipoData
///Description:
///
#pragma once

#include "DataManager.h"
#include "RunInfo.h"
#include <TChain.h>
#include <fstream>
#include <string>
#include <iostream>

namespace chanser{

  class LundData : public DataManager{

  public :

  LundData()
    {
      _dataType=static_cast<Short_t>(chanser::DataType::Lund);
    };
 

    //DataManager
    Bool_t Init() final;
    virtual Bool_t InitEvent() final;
    Bool_t ReadEvent(Long64_t entry=-1) final;
    
    Bool_t NextFile();
    void AddFile(TString name){
      _chainOfFiles.Add(name);
    }
    
    const std::vector<short> eventPids() final{return _evPids;}
    const particle_ptrs& GetParticles() const final {return _eventParticles;};
    const truth_ptrs& GetTruth() const final {return _eventTruth;};

    const BaseRunInfo* GetRunInfo() const noexcept final{return &_runInfo;};
    const BaseEventInfo* GetEventInfo() const noexcept final{return &_eventInfo;};

    void FillRunInfo();
    
  private:

    particle_ptrs _eventParticles; //actual particles in event
    truth_ptrs _eventTruth; //actual truth particles in event
    truth_objs _truthPool; //pool of truth objects can use for each event
    std::vector<short> _evPids; //Pdg codes of particles in event
    Int_t _Nparts=0;
    
    RunInfo _runInfo;
    BaseEventInfo _eventInfo;
    
    size_t _nFile={0};
    TChain _chainOfFiles;
    std::ifstream _txtIn; //input text file stream
    std::string _sline;

    Double_t _Ebeam=0;
    
  };

}
 
