//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     Cuts on the z-vertex position.

#pragma once

#include "BaseCut.h"

namespace chanser{


  class ZVertexCut : public chanser::BaseCut{
      
  public:
    ZVertexCut()=default; // must have default constructor
    
    ZVertexCut(Float_t cutLow, Float_t cutHigh){
      _cutValLow=cutLow;
      _cutValHigh=cutHigh;
    }
      
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12();
      Float_t vz = c12->par()->getVz();
      if(vz>_cutValLow && vz<_cutValHigh) return true;
      return false;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with cut on z-vertex position between "<<_cutValLow<<" and "<<_cutValHigh<<"cm."<<std::endl;
    }

  private:
    Float_t _cutValLow=chanser::THIS_FLT_MAX;
    Float_t _cutValHigh=chanser::THIS_FLT_MAX;
      
    ClassDefOverride(chanser::ZVertexCut,1);

  };//class ZVertexCut

}
