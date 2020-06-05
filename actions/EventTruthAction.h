//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///

#pragma once

#include "ActionManager.h"
#include "ParticleData.h"
#include <TDatabasePDG.h>
#include <TSystem.h>
#include <functional>

namespace chanser{
    
     
  using baseparticle_ptrs=std::vector<BaseParticle*>;
  using truth_ptrs = std::vector<TruthParticle*>;
 

  class EventTruthAction : public ActionManager {

  public :
      
    EventTruthAction()=default;
    ///Must give an output directory for saving trees to
  EventTruthAction(TString name): ActionManager(name){};
      
    virtual ~EventTruthAction()=default;
    EventTruthAction(const EventTruthAction& other) = default; //Copy Constructor
    EventTruthAction(EventTruthAction&& other) = default; //Move Constructor
      
    EventTruthAction& operator=(const EventTruthAction& other)=default;
    EventTruthAction& operator=(EventTruthAction&& other)=default;

    void PrintAction() override;
      
    void InitDataEvent() override{if(_truth==nullptr) return;FinalStateTruthMatch(0); }
    Bool_t Execute(UInt_t ti)  override{
      if(_truth==nullptr)return kTRUE;
      //check if we have the correct truth
      FinalStateTruthMatch(static_cast<Short_t>(CheckEventTruth(_particles[ti])));
      //return true as do not want to cut event
      return kTRUE;
    }
      
    void Configure(FinalState* fs) override;

    Bool_t CheckEventTruth(baseparticle_ptrs parts);
    Bool_t CheckParticleTruth(BaseParticle* p);

  protected:
      
   
  private :

    //link to the final state class truth match function
    std::function<void(Short_t)> FinalStateTruthMatch;//!
    
    const truth_ptrs* _truth{nullptr};//! truth particles for event
    std::vector< baseparticle_ptrs > _particles; //! a set for each topology
  
      
    ClassDefOverride(chanser::EventTruthAction,1);
  };// EventTruthAction
    
  using eventtruthaction_uptr = std::unique_ptr<EventTruthAction>;
  using eventtruthaction_ptr = EventTruthAction*;
    



}

