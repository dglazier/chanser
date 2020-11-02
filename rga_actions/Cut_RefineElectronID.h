//////////////////////////////////////////////////////////////
///
///Class:		
///Description:  Cut_RefineElectronID
///Author : 
   

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"

namespace chanser{


  class Cut_RefineElectronID : public BaseCut, public CLAS12Base{
      
  public:
    Cut_RefineElectronID()=default; // must have default constructor
    
       
    void ChangeRun() final;

    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{

     //auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
     //auto c12=c12p->CLAS12(); //if you require other DST data
     // std::cout<<_sfracPars.size()<<" "<<_sfracPars[0][5]<<std::endl;
     return  kTRUE;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with range "<<std::endl;
    }

  private:
    
    clas12::TableOfDoubles_t _sfracPars;

    ClassDefOverride(chanser::Cut_RefineElectronID,1);

  };//class Cut_RefineElectronID

}
