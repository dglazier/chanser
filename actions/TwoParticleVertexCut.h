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
      auto c12=c12p->CLAS12(); //if you require other DST data
      auto c12p_el = static_cast<const chanser::CLAS12Particle*>(_cutAgainst);
      auto c12el = c12p_el->CLAS12();


      auto Had_Vz = c12->par()->getVz();
      auto El_Vz = c12el->par()->getVz();
      return  (TMath::Abs(Had_Vz-El_Vz) < _cutVal) ?  kTRUE :  kFALSE;
      
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
