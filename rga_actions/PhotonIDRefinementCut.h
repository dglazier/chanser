//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     my own derived particle cut

#pragma once

#include "BaseCut.h"

namespace chanser{


  class PhotonIDRefinementCut : public chanser::BaseCut{
      
  public:
    PhotonIDRefinementCut()=default; // must have default constructor
    
    PhotonIDRefinementCut(Float_t cut){_cutVal=cut;}
      
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      Double_t beta = c12->par()->getBeta();
      Double_t max_beta = 1+_cutVal;
      Double_t min_beta = 1-_cutVal;
      return (beta<max_beta && beta>min_beta) ?  kTRUE :  kFALSE;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with cutVal "<<_cutVal<<std::endl;
    }

  private:
    Float_t _cutVal=chanser::THIS_FLT_MAX;
      
    ClassDefOverride(chanser::PhotonIDRefinementCut,1);

  };//class PhotonIDRefinementCut

}
