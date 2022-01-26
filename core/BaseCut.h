//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///    Interface to different types of cut

#pragma once

#include "BaseParticle.h"
#include "CLAS12Particle.h"
#include <TNamed.h>
#include <TMath.h>
#include <limits>

namespace chanser{
  
  constexpr Float_t THIS_FLT_MAX = 3.40282e+38;
  constexpr Int_t THIS_INT_MAX2 = 2147483647;
    
  class BaseCut: public TNamed {
      
  public :
      
    BaseCut()=default;
        
      
    virtual Bool_t ParticleCut(const BaseParticle* part) const noexcept {return kTRUE;}

    ClassDef(chanser::BaseCut,1);
 
  };
  using basecut_ptr = BaseCut*;
  using basecuts =std::vector<basecut_ptr>;
  using constbasecuts =std::vector<const basecut_ptr>;
 
  ////////////////////////////////////////////////////////
  ///A simple derived cuts class based on Deltatime
    
  class DeltaTimeCut : public BaseCut{
      
  public:
    DeltaTimeCut()=default;
    DeltaTimeCut(Float_t cut){_timeCut=cut;}
      
      
      
    Bool_t ParticleCut(const BaseParticle* part) const noexcept override{
      return TMath::Abs(part->DeltaTime())<_timeCut ?  kTRUE :  kFALSE;
    }
      
    void SetTimeCut(Double_t val){_timeCut=val;}

    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with width "<<_timeCut<<std::endl;
    }

  private:
    Float_t _timeCut=THIS_FLT_MAX;
      
    ClassDefOverride(chanser::DeltaTimeCut,1);

  };//class DeltaTimeCut
  ////////////////////////////////////////////////////////
  ///A simple derived cuts class based on Deltatime
    
  class DeltaTimeVerCut : public BaseCut{
      
  public:
    DeltaTimeVerCut()=default;
    DeltaTimeVerCut(Float_t cut){_timeCut=cut;}
      
      
      
    Bool_t ParticleCut(const BaseParticle* part) const noexcept override{
      return TMath::Abs(part->DeltaTimeVer())<_timeCut ?  kTRUE :  kFALSE;
    }
      
    void SetTimeCut(Double_t val){_timeCut=val;}

    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with width "<<_timeCut<<std::endl;
    }

  private:
    Float_t _timeCut=THIS_FLT_MAX;
      
    ClassDefOverride(chanser::DeltaTimeVerCut,1);

  };//class DeltaTimeCut

  ////////////////////////////////////////////////////////
    ///A simple derived cuts class based on REC::Particle::pid
  
  class EventBuilderCut : public BaseCut{
      
  public:
    EventBuilderCut()=default;
      
      
    Bool_t ParticleCut(const BaseParticle* part) const noexcept override{
      // std::cout<<"ParticleCut "<<(static_cast<const CLAS12Particle*>(part))->CLAS12()<<"    "<<part->PDG()<<" "<<part->P4().Theta()*TMath::RadToDeg()<<std::endl;
      if((static_cast<const CLAS12Particle*>(part))->CLAS12()==nullptr) return kFALSE;
      return (static_cast<const CLAS12Particle*>(part))->CLAS12()->getPid()==part->PDG() ?  kTRUE :  kFALSE;
    }
      
    // void SetPidCut(Int_t val){_pidCut=val;}
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" just if the DST event builder PID is correct for this particle "<<std::endl;
    }

  private:
    //Int_t _pidCut=INT_MAX;
      
    ClassDefOverride(chanser::EventBuilderCut,1);

  };
  
}
