//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     my own derived particle cut

#pragma once

#include "CLAS12ParticleCut.h"

namespace chanser{


  class TwoParticleVertexCut : public chanser::CLAS12ParticleCut{
      
  public:
    TwoParticleVertexCut()=default; // must have default constructor
     
  TwoParticleVertexCut(TString pname,Float_t cut):_partName{pname},_cutVal{cut}{}
      
    void SetC12(CLAS12FinalState* c12) override;//see cpp

    
    Bool_t ParticleCut(const BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      //auto c12=p->CLAS12(); //if you require other DST data
      // return /* c12p->XXX <_cutVal*/ ?  kTRUE :  kFALSE;
      return kTRUE;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with cutVal "<<_cutVal<<std::endl;
    }

  private:
    Float_t _cutVal=chanser::THIS_FLT_MAX;
    TString _partName;
    
    CLAS12Particle*  _cutAgainst={nullptr};//!particle to check vertex against

    ClassDefOverride(chanser::TwoParticleVertexCut,1);

  };//class TwoParticleVertexCut

}
