#pragma once

#include "FinalState.h"
#include "EventInfo.h"
#include "RunInfo.h"
#include "Kinematics.h"
#include "CLAS12Particle.h"

using chanser::BaseParticle;
using chanser::CLAS12Particle;
using chanser::HSLorentzVector;
using chanser::BaseEventInfo;
using chanser::BaseOutEvent;
using chanser::BaseRunInfo;
using chanser::FinalState;

namespace chanser{

  
  class CLAS12FinalState : public FinalState{

  public:

    CLAS12FinalState()=default;
  CLAS12FinalState(TString ch,TString inc) : FinalState(ch,inc) {}
    virtual ~CLAS12FinalState()=default;

    const EventInfo* GetEventInfo() const noexcept{return _eventInfo;} 
    const RunInfo* GetRunInfo() const noexcept{return _runInfo;} 
      
    void SetEventInfo(const BaseEventInfo* evi) final {_eventInfo=dynamic_cast<const EventInfo*>(evi);};
    void SetRunInfo(const BaseRunInfo* evi) final {_runInfo=dynamic_cast<const RunInfo*>(evi);};

    void HalveBunchTime(){_bunchTime/=2;}
    void SetStartTimePeak(Float_t st){_sTimePeak=st;}

    Float_t BunchTime()const noexcept{return _bunchTime;}
    Float_t STimePeak()const noexcept{return _sTimePeak;}
       
    Float_t StartTimeFromTimeAndVertex(Float_t ptime,Float_t vz=0);
    Float_t StartTimeFromParticleVt(const CLAS12Particle& part){return _startTime=part.CLAS12()->par()->getVt(); }
    Float_t StartTimeFromEB()  {return _startTime=_eventInfo->_StartTime;}
    Float_t StartTimeFromEBFTB(){return _startTime=_eventInfo->_FTBStartTime;}
    
 
  protected :

    chanser::Kinematics _kinCalc;//!
  private :
      
    const EventInfo* _eventInfo={nullptr};//!
    const RunInfo* _runInfo={nullptr};//!

    Float_t _startTime{0};
  
    Float_t _bunchTime = 2.0080160*2;
    Float_t _sTimePeak =   44.205;
    
    ClassDefOverride(chanser::CLAS12FinalState,1); 
  };

  ////////////////////////////////////////
  ///Need to update this with RCDB info passed to runInfo
  inline Float_t CLAS12FinalState::StartTimeFromTimeAndVertex(Float_t ptime,Float_t vz){
    //supply chosen (e-) particle vertex time
    Float_t rftime=_eventInfo->_RFTime;
    //Find the nearest rf beam bucket
    _startTime=_sTimePeak-_bunchTime*((Int_t)(std::round(((_sTimePeak-(ptime-rftime))/_bunchTime))))+rftime;

    
    _startTime-=vz/2.99792e+08*1E9; //Factor 100 ?
    return _startTime;
  }

}
