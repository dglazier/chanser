//////////////////////////////////////////////////////////////
///
///Class:      FiducialCut_PCAL_uvw		
///Description:
///            Implements cuts on the PCAL Fiducial region
/// within a distance _cutVal (cm) in u, v, w passed by the user.
///Author: 
///             Richard Tyson

#pragma once

#include "BaseCut.h"

namespace chanser{


  class FiducialCut_PCAL_uvw : public chanser::BaseCut{
      
  public:
    FiducialCut_PCAL_uvw()=default; // must have default constructor
    
    FiducialCut_PCAL_uvw(Float_t cut){_cutVal=cut;}
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      
      if(c12->getRegion()!=clas12::FD) return true; //cut only applies to FD

      double PCalLU = c12->cal(clas12::PCAL)->getLu();
      if(PCalLU<_cutVal)return false;
      double PCalLV = c12->cal(clas12::PCAL)->getLv();
      if(PCalLV<_cutVal)return false;
      double PCalLW = c12->cal(clas12::PCAL)->getLw();
      if(PCalLW<_cutVal)return false;
      return true;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" cuts on particles within "<<_cutVal<<"cm of the edge of the PCAL strips along u, v and w."<<std::endl;
    }

  private:
    /* _cutVal here represents the distance in cm along u/v/w.
     * A loose cut is suggested at 9cm, a medium one at 14cm,
     * a tight one at 19cm.
     */
    Float_t _cutVal=chanser::THIS_FLT_MAX;
      
    ClassDefOverride(chanser::FiducialCut_PCAL_uvw,1);

  };//class FiducialCut_PCAL_uvw

}
