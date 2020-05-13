//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     my own derived particle cut

#pragma once

#include "BaseCut.h"

namespace USERNAME{


  class templateClass : public chanser::BaseCut{
      
  public:
    templateClass()=default; // must have default constructor
    
    templateClass(Float_t cut){_cutVal=cut;}
      
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      //auto c12=p->CLAS12(); //if you require other DST data
      return /* c12p->XXX <_cutVal*/ ?  kTRUE :  kFALSE;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with cutVal "<<_cutVal<<std::endl;
    }

  private:
    Float_t _cutVal=chanser::THIS_FLT_MAX;
      
    ClassDefOverride(USERNAME::templateClass,1);

  };//class templateClass

}
