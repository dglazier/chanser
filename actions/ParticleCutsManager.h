//////////////////////////////////////////////////////////////
///
///Class:		ParticleCutsManager
///Description:
///

#pragma once

#include "ActionManager.h"
#include "ParticleCuts.h"
#include <TDatabasePDG.h>

namespace chanser{
    
   
  
  using vecparticlecuts = std::vector< ParticleCuts >;
  using basecut_uptr = std::unique_ptr<BaseCut>;
 
  class ParticleCutsManager : public ActionManager {

  public:
    ParticleCutsManager()=default;
    ///Must give an output directory for saving trees to
  ParticleCutsManager(TString name,Short_t forReal=0):ActionManager(name),_forReal(forReal){};
    
    virtual ~ParticleCutsManager()=default;
    ParticleCutsManager(const ParticleCutsManager& other) = default; //Copy Constructor
    ParticleCutsManager(ParticleCutsManager&& other) = default; //Move Constructor
      
    ParticleCutsManager& operator=(const ParticleCutsManager& other)=default;
    ParticleCutsManager& operator=(ParticleCutsManager&& other)=default;


    void InitDataEvent() override{ _passCut=0; }
    Bool_t Execute(UInt_t ti)  override{
      //check cut for topology number ti
      //Pass cut if all particles pass
      _passCut=_particleCuts[ti].PassCuts()==_particleCuts[ti].NParticles();
      return _forReal ? _passCut : kTRUE;
    }
      
  
    void Configure(FinalState* fs) override;
    void PostConfigure(FinalState* fs) override;
    void ChangeRun() override;
   
    void AddParticleCut(TString type,BaseCut* cut){
      if(TDatabasePDG::Instance()->GetParticle(type)){
	//cut.SetName(type);
	AddParticleCut(TDatabasePDG::Instance()->GetParticle(type)->PdgCode(),cut);
      }
    }
    void AddParticleCut(Int_t type,BaseCut* cut){
      Int_t pdg=0;
      if(TDatabasePDG::Instance()->GetParticle(type))
	//	pdg=TDatabasePDG::Instance()->GetParticle(type)->PdgCode();
	pdg=type;
      _pdgToCut[type]=basecut_uptr{cut};
      //cut now belongs to me, do not delete it elsewhere
    }

    //defcut now belongs to me, do not delete it elsewhere
    void SetDefaultCut(BaseCut*  defcut) {_useableDefault.reset(defcut);}
      
    const BaseCut* getCut(Int_t type) const {return _pdgToCut.at(type).get();}
      
    void PrintAction() override;

    void Branches(FinalState* fs);

  protected :
      
  private :

    //This class manages ParticleCuts 
    vecparticlecuts _particleCuts; //!
      
    std::map<Int_t,basecut_uptr > _pdgToCut;  //needs written
    
    basecut_uptr _useableDefault{nullptr};  //needs written

    UInt_t _passCut{0};//!
    Short_t _forReal{0};
      
    ClassDefOverride(chanser::ParticleCutsManager,1);
  };// ParticleCutsManager
    
  using pcutsman_uptr = std::unique_ptr<ParticleCutsManager>;
  using pcutsman_ptr = ParticleCutsManager*;
  using pcutsmans =std::vector<pcutsman_ptr>;
    
}
