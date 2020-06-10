//////////////////////////////////////////////////////////////
///
///Class:		ParticleCorrectionManager
///Description:
///

#pragma once

#include "ActionManager.h"
#include "ParticleCorrection.h"
#include <TDatabasePDG.h>

namespace chanser{
  
  using vec_applycorrs = std::vector< ParticleCorrection >;
  using basecorr_uptr = std::unique_ptr<BaseCorrection>;
 
   class ParticleCorrectionManager : public ActionManager {

  public:
    ParticleCorrectionManager()=default;
    ///Must give an output directory for saving trees to
   ParticleCorrectionManager(TString name,Short_t forsim=0):ActionManager(name),_forSim{forsim}{};
    
    virtual ~ParticleCorrectionManager()=default;
    ParticleCorrectionManager(const ParticleCorrectionManager& other) = default; //Copy Constructor
    ParticleCorrectionManager(ParticleCorrectionManager&& other) = default; //Move Constructor
      
    ParticleCorrectionManager& operator=(const ParticleCorrectionManager& other)=default;
    ParticleCorrectionManager& operator=(ParticleCorrectionManager&& other)=default;

    void PrintAction() override;
    Bool_t Execute(UInt_t ti)  override{
      if(_forSim<1)  return kTRUE; //not for sim, or not simulated data
      //apply corrections for topology number ti
      _particleCorrs[ti].Apply();
      return kTRUE;
    }
      
  
    void Configure(FinalState* fs) override;
  
    void AddParticle(TString type,BaseCorrection* cor){
      if(TDatabasePDG::Instance()->GetParticle(type)){
	//cut.SetName(type);
	AddParticleCut(TDatabasePDG::Instance()->GetParticle(type)->PdgCode(),cor);
      }
    }
    void AddParticleCut(Int_t type,BaseCorrection* cut){
      Int_t pdg=0;
      if(TDatabasePDG::Instance()->GetParticle(type))
    	pdg=TDatabasePDG::Instance()->GetParticle(type)->PdgCode();
      _pdgToCorr[type]=basecorr_uptr{cut};
      //correction now belongs to me, do not delete it elsewhere
    }

   private:
    
    vec_applycorrs _particleCorrs;//!
    std::map<Int_t,basecorr_uptr > _pdgToCorr; ///needs written

    Short_t _forSim{kFALSE}; //should this correction be applie dto simulated data
    ClassDefOverride(chanser::ParticleCorrectionManager,1);
   };//ParticleCorrectionManager

}
