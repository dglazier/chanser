//////////////////////////////////////////////////////////////
///
///Class:		ParticleCutsManager
///Description:
///

#pragma once

#include "ActionManager.h"
#include "ParticleCuts.h"
#include <TDatabasePDG.h>

namespace chanzer{
    
   
  
  using vecparticlecuts = std::vector< ParticleCuts >;

  class ParticleCutsManager : public ActionManager {

  public:
    ParticleCutsManager()=default;
    ///Must give an output directory for saving trees to
  ParticleCutsManager(TString name,Short_t forReal=0):ActionManager(name),_forReal(forReal){ std::cout<<"Constructing PCM "<<GetName()<<std::endl;};
    
    virtual ~ParticleCutsManager(){std::cout<<"DECConstructing PCM"<<std::endl;};
    ParticleCutsManager(const ParticleCutsManager& other) = default; //Copy Constructor
    ParticleCutsManager(ParticleCutsManager&& other) = default; //Move Constructor
      
    ParticleCutsManager& operator=(const ParticleCutsManager& other)=default;
    ParticleCutsManager& operator=(ParticleCutsManager&& other)=default;

  
    Bool_t Execute(UInt_t ti)  override{
      //check cut for topology number ti
      _passCut=_particleCuts[ti].PassCuts();
      return _forReal ? _passCut==_particleCuts[ti].NParticles() : kTRUE;
    }
      
    //void End() override {};
    void Configure(FinalState* fs) override;
      
    //  void Register( ParticleCuts& pcuts ){_particleCuts.push_back(pcuts);};
    void AddParticleCut(TString type,BaseCut& cut){
      if(TDatabasePDG::Instance()->GetParticle(type)){
	cut.SetName(type);
	AddParticleCut(TDatabasePDG::Instance()->GetParticle(type)->PdgCode(),cut);
      }
    }
    void AddParticleCut(Int_t type,BaseCut& cut){
      Int_t pdg=0;
      if(TDatabasePDG::Instance()->GetParticle(type))
	pdg=TDatabasePDG::Instance()->GetParticle(type)->PdgCode();
      _pdgToCut[type]=&cut; //copy cut
 
    }

    void SetDefaultCut(BaseCut&  defcut) {_useableDefault=&defcut;}
      
    const BaseCut* getCut(Int_t type) const {return _pdgToCut.at(type);}
      
    void PrintAction() override;

    void Branches(const FinalState* fs);

  protected :
      
  private :

    //This class manages ParticleCuts 
    vecparticlecuts _particleCuts; //!
      
    std::map<Int_t,basecut_ptr > _pdgToCut;
      
    BaseCut  _defaultCut;
    basecut_ptr _useableDefault{nullptr};//!

    Int_t _passCut{0};//!
    Short_t _forReal{0};
      
    ClassDefOverride(chanzer::ParticleCutsManager,1);
  };// ParticleCutsManager
    
  using pcutsman_uptr = std::unique_ptr<ParticleCutsManager>;
  using pcutsman_ptr = ParticleCutsManager*;
  using pcutsmans =std::vector<pcutsman_ptr>;
    



}
