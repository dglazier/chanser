//////////////////////////////////////////////////////////////
///
///Class:      CLAS12NeutronParticleCut		
///Description:
///            Checks if a particle is a valid CLAS12Neutron
/// Author:
///            Richard Tyson

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"

namespace chanser{


  class CLAS12NeutronParticleCut : public BaseCut, public CLAS12Base{
      
  public:
    CLAS12NeutronParticleCut()=default; // must have default constructor  
    CLAS12NeutronParticleCut(Bool_t useC12Neutron):_useC12Neutron{useC12Neutron}{};

    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
 
      if(dynamic_cast<const CLAS12Particle*>(part)->CLAS12()->getRegion()!=clas12::FD){
	return true; //only FD n 
      }
      auto neutron = dynamic_cast<const CLAS12Neutron*>(part);
      return neutron->isValid()==_useC12Neutron;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" do we want CLAS12Neutrons? "<<_useC12Neutron<<std::endl;
    }

  private:
    Bool_t _useC12Neutron{true};
      
    ClassDefOverride(chanser::CLAS12NeutronParticleCut,1);

  };//class CLAS12NeutronParticleCut

}
