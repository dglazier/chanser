#pragma once

#include "FinalState.h"
#include "EventInfo.h"
#include "RunInfo.h"
#include "Kinematics.h"

using chanser::BaseParticle;
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

    void HalveBunchTime(){_bunchTime/=2;}
    void SetStartTimePeak(Float_t st){_STimePeak=st;}
  protected :

    chanser::Kinematics _kinCalc;//!
    
    Float_t StartTimeFromTimeAndVertex(Float_t ptime,Float_t vz=0);
    Float_t StartTimeFromParticleVt(const CLAS12Particle& part){return _startTime=part.CLAS12()->par()->getVt(); }
    Float_t StartTimeFromEB()  {return _startTime=_eventInfo->_StartTime;}
    Float_t StartTimeFromEBFTB(){return _startTime=_eventInfo->_FTBStartTime;}
    
  private :
      
    const EventInfo* _eventInfo{nullptr};//!
    const RunInfo* _runInfo{nullptr};//!

    Float_t _startTime{0};
  
    Float_t _bunchTime = 2.0080160*2;
    Float_t _STimePeak =   44.125;
    
    ClassDefOverride(chanser::CLAS12FinalState,1); 
  };

  ////////////////////////////////////////
  ///Need to update this with RCDB info passed to runInfo
inline Float_t CLAS12FinalState::StartTimeFromTimeAndVertex(Float_t ptime,Float_t vz){
  //supply chosen (e-) particle vertex time
  Float_t rftime=_eventInfo->_RFTime;
  //Find the nearest rf beam bucket
  _startTime=_STimePeak-_bunchTime*((Int_t)(std::round(((_STimePeak-(ptime-rftime))/_bunchTime))))+rftime;

    
  _startTime-=vz/2.99792e+08*1E9; //Factor 100 ?
  return _startTime;
}

}
