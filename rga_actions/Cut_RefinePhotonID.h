//////////////////////////////////////////////////////////////
///
///Class: Cut_RefinePhotonID		
///Description: To reject uncorrelated photons from the beam bunch a cut is placed on the maximum beta value from the calorimeter.
///     

#pragma once

#include "BaseCut.h"

namespace chanser{


  class Cut_RefinePhotonID : public chanser::BaseCut{
      
  public:
    Cut_RefinePhotonID()=default; // must have default constructor
    
  Cut_RefinePhotonID(Float_t mincut,Float_t maxcut):_minVal{mincut},_maxVal{maxcut}{}
       
      
    /*Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      Double_t beta = c12->par()->getBeta();
      Double_t max_beta = 1+_cutVal;
      Double_t min_beta = 1-_cutVal;
      return (beta<max_beta && beta>min_beta) ?  kTRUE :  kFALSE;
      }*/
   Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{

     //auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
     //auto c12=c12p->CLAS12(); //if you require other DST data
     //std::cout<<" betas "<<part->Beta()<<" "<<c12->par()->getBeta()<<" "<<c12->par()->getFTBBeta()<<std::endl;
     Double_t beta = part->Beta();
     return (beta<_maxVal && beta>_minVal) ?  kTRUE :  kFALSE;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with range "<<_minVal<<" to "<<_maxVal<<std::endl;
    }

  private:
    Float_t _minVal=-chanser::THIS_FLT_MAX;
    Float_t _maxVal=chanser::THIS_FLT_MAX;
      
    ClassDefOverride(chanser::Cut_RefinePhotonID,1);

  };//class Cut_RefinePhotonID

}
