#pragma once

#include "FinalState.h"
#include "EventInfo.h"
#include "RunInfo.h"
#include "Kinematics.h"

using chanzer::BaseParticle;
using chanzer::HSLorentzVector;
using chanzer::BaseEventInfo;
using chanzer::BaseOutEvent;
using chanzer::BaseRunInfo;
using chanzer::FinalState;

namespace chanzer{
    
  class CLAS12FinalState : public FinalState{

  public:

    CLAS12FinalState()=default;
  CLAS12FinalState(TString ch,TString inc) : FinalState(ch,inc) {}
    virtual ~CLAS12FinalState()=default;

    const EventInfo* Event() const noexcept{return _eventInfo;} 
    const RunInfo* Run() const noexcept{return _runInfo;} 
      
    void SetEventInfo(const BaseEventInfo* evi) final {_eventInfo=dynamic_cast<const EventInfo*>(evi);};

  protected :

    chanzer::Kinematics _kinCalc;//!

  private :
      
    const EventInfo* _eventInfo{nullptr};//!
    const RunInfo* _runInfo{nullptr};//!

  
      
    //  ClassDefOverride(chanzer::CLAS12FinalState,1); 
  };


}

using chanzer::CLAS12FinalState;
