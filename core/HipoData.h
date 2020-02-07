//////////////////////////////////////////////////////////////
///
///Class:		HipoData
///Description:
///
#pragma once

#include "DataManager.h"
#include "RunInfo.h"
#include "EventInfo.h"
#include "clas12reader.h"


namespace chanzer{

  class HipoData : public DataManager{

  public :
    HipoData()=default;
    virtual ~HipoData()=default;

    //DataManager
    Bool_t Init() override;
    virtual Bool_t InitEvent() override;
    Bool_t ReadEvent(Long64_t entry=-1)override;
    Bool_t FetchPids();

    //HipoData
    void FillParticles();
    void FillTruth();
    void FillEventInfo();
    void FillRunInfo();
      
    clas12::clas12reader* Reader() const {return _c12;}
    void SetReader(clas12::clas12reader* reader){_c12=reader;Init();}
    Bool_t SetFile(const TString filename);
	
    const std::vector<short> eventPids() final{return _c12->preCheckPids();}
    const particle_ptrs& GetParticles() const final {return _eventParticles;};
    const truth_ptrs& GetTruth() const final {return _eventTruth;};

    const BaseRunInfo* GetRunInfo() const noexcept final{return &_runInfo;};
    const BaseEventInfo* GetEventInfo() const noexcept final{return &_eventInfo;};
      
  
  private :

    //clas12tools
    std::unique_ptr<clas12::clas12reader> _myC12; //if created here
    clas12::clas12reader* _c12=nullptr;  //passed from myC12 or external source

    particle_objs _particlePool; //pool of particle objects can use for each event
    truth_objs _truthPool; //pool of truth objects can use for each event

      
    particle_ptrs _eventParticles; //actual particles in event
    truth_ptrs _eventTruth; //actual truth particles in event

    RunInfo _runInfo;
    EventInfo _eventInfo;
 
  };
}
