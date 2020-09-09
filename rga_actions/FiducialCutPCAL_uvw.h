//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///            Implements cuts on the PCAL Fiducial region

#pragma once

#include "BaseCut.h"

namespace chanser{


  class FiducialCutPCAL_uvw : public chanser::BaseCut{
      
  public:
    FiducialCutPCAL_uvw()=default; // must have default constructor
    
    FiducialCutPCAL_uvw(Float_t cut){_cutVal=cut;}
      
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      double PCalLU = c12->cal(1)->getLu();
      double PCalLV = c12->cal(1)->getLv();
      double PCalLW = c12->cal(1)->getLw();
      if(PCalLU>_cutVal && PCalLV>_cutVal && PCalLW>_cutVal) return true;
      return false;
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
      
    ClassDefOverride(chanser::FiducialCutPCAL_uvw,1);

  };//class FiducialCutPCAL_uvw

}
