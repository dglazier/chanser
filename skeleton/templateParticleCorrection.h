//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     my own derived particle cut

#pragma once

#include "BaseCorrection.h"
#include "CLAS12Particle.h"
#include "clas12defs.h"

namespace USERNAME{


  class templateClass : public chanser::BaseCorrection{
      
  public:
    templateClass()=default; // must have default constructor
    
    //templateClass(Float_t a0):_a0{a0}{} //give arguments
      
      
    void ParticleCorrect(chanser::BaseParticle* part) const noexcept override{
      auto p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=p->CLAS12(); //if you require other DST data
      //if(c12->getRegion()!=clas12::FD) return; //e.g only FD correction
      //...
      // Double_t p_mag = part->P4p()->P();
      //...
      // part->SetP(p_mag_corrected);
      //
      //or for example part->P4p()->SetXYZM(x,y,z,m);
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with parameter ";
      //std::cout<<_a0<<std::endl;
    }

  private:
    //Float_t _a0{0}; //example parameter
      
    ClassDefOverride(USERNAME::templateClass,1);

  };//class templateClass

}
