//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///    Interface to different types of cut

#pragma once

#include "BaseParticle.h"
#include <TNamed.h>
#include <TMath.h>
#include <limits>
#include <bits/stdc++.h>

namespace chanzer{
    
    
  class BaseCut: public TNamed {
      
  public :
      
    BaseCut()=default;
        
      
    virtual Bool_t ParticleCut(const BaseParticle* part) const noexcept {return kTRUE;}

    ClassDef(chanzer::BaseCut,1);
 
  };
  using basecut_ptr = BaseCut*;
  using basecuts =std::vector<basecut_ptr>;
  using constbasecuts =std::vector<const basecut_ptr>;
 
  ////////////////////////////////////////////////////////
  ///A simple derived cuts class based on Deltatime
    
  class DeltaTimeCut : public BaseCut{
      
  public:
    DeltaTimeCut(Double_t cut=FLT_MAX){_timeCut=cut;}
      
      
    Bool_t ParticleCut(const BaseParticle* part) const noexcept override{
      return TMath::Abs(part->DeltaTime())<_timeCut ?  kTRUE :  kFALSE;
    }
      
    void SetTimeCut(Double_t val){_timeCut=val;}
      
  private:
    Double_t _timeCut=DBL_MAX;
      
    ClassDefOverride(chanzer::DeltaTimeCut,1);

  };//class DeltaTimeCut

  ////////////////////////////////////////////////////////
    ///A simple derived cuts class based on REC::Particle::pid
  
  class EventBuilderCut : public BaseCut{
      
  public:
    EventBuilderCut(Int_t pdg=0){_pidCut=pdg;}
      
      
    Bool_t ParticleCut(const BaseParticle* part) const noexcept override{
      return part->CLAS12()->getPid()==part->PDG() ?  kTRUE :  kFALSE;
    }
      
    void SetPidCut(Int_t val){_pidCut=val;}
      
  private:
    Int_t _pidCut=INT_MAX;
      
    ClassDefOverride(chanzer::EventBuilderCut,1);

  };
  
}
