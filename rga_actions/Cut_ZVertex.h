//////////////////////////////////////////////////////////////
///
///Class:      Cut_ZVertex		
///Description:
///            Cuts on the z-vertex position of particle within a distance
/// defined by the rg-a analysis note (recommended use for electrons).
/// Author:
///            Richard Tyson

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"

namespace chanser{


  class Cut_ZVertex : public BaseCut, public CLAS12Base{
      
  public:
    Cut_ZVertex()=default; // must have default constructor   
      
    void ChangeRun() final;
  
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12();
      
      if(c12->getRegion()!=clas12::FD) return true; //cut only applies to FD
  
      Float_t vz = c12->par()->getVz();
      
      return (vz>_cutValLow && vz<_cutValHigh) ? kTRUE : kFALSE;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with cut on z-vertex position between "<<_cutValLow<<" and "<<_cutValHigh<<"cm."<<std::endl;
    }

  private:
    Float_t _cutValLow=chanser::THIS_FLT_MAX;
    Float_t _cutValHigh=chanser::THIS_FLT_MAX;
      
    ClassDefOverride(chanser::Cut_ZVertex,1);

  };//class Cut_ZVertex

}
