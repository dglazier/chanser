//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///

#pragma once

#include "ActionManager.h"
#include "CLAS12Particle.h"
#include <TDatabasePDG.h>
#include <TSystem.h>
#include <functional>

namespace chanser{
    
     
  using baseparticle_ptrs=std::vector<BaseParticle*>;
  using clas12particle_ptrs=std::vector<CLAS12Particle*>;
  using truth_ptrs = std::vector<TruthParticle*>;
 

  class MCMatchAction : public ActionManager {

  public :
      
    MCMatchAction()=default;
    ///Must give an output directory for saving trees to
  MCMatchAction(TString name): ActionManager(name){};
      
    virtual ~MCMatchAction()=default;
    MCMatchAction(const MCMatchAction& other) = default; //Copy Constructor
    MCMatchAction(MCMatchAction&& other) = default; //Move Constructor
      
    MCMatchAction& operator=(const MCMatchAction& other)=default;
    MCMatchAction& operator=(MCMatchAction&& other)=default;

    void PrintAction() override;
      
    void InitDataEvent() override{FinalStateTruthMatch(0); }
    Bool_t Execute(UInt_t ti)  override{
      if(_genIDs.empty()==kTRUE)return kTRUE;
      //check if we have the correct truth
      FinalStateTruthMatch(static_cast<Short_t>(CheckEventTruth(_particles[ti],_genIDs[ti])));
      //return true as do not want to cut event
      return kTRUE;
    }
      
    void Configure(FinalState* fs) override;

    Bool_t CheckEventTruth(const clas12particle_ptrs& parts,std::vector<Int_t> genIDs);
 
  protected:
      
   
  private :

    //link to the final state class truth match function
    std::function<void(Short_t)> FinalStateTruthMatch;//!
    
    //const truth_ptrs* _truth{nullptr};//! truth particles for event
    std::vector< clas12particle_ptrs > _particles; //! a set for each topology
    std::vector<std::vector<Int_t>> _genIDs; //! genIDs to compare matched particles with
      
    ClassDefOverride(chanser::MCMatchAction,1);
  };// MCMatchAction
    
  using mcmatchaction_uptr = std::unique_ptr<MCMatchAction>;
  using mcmatchaction_ptr = MCMatchAction*;
    



}

