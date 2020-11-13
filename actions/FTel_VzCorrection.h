#pragma once
#include "BaseCorrection.h"
#include "clas12defs.h"

namespace chanser{

  class FTel_VzCorrection : public BaseCorrection{
      
  public:
    FTel_VzCorrection()=default;

  FTel_VzCorrection(Double_t zpos):
    _zpos{zpos}{};
      

   
    void ParticleCorrect(BaseParticle* part) const noexcept override{
      if(static_cast<CLAS12Particle*>(part)->CLAS12()->getRegion()!=clas12::FT)
	return; //only FT e-

      part->SetVertex(0,0,_zpos);
      
    }
    
   private:

    Double_t _zpos{0};
   
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with z position  "<<_zpos<<std::endl;
    }

  private:
       
    ClassDefOverride(chanser::FTel_VzCorrection,1);

  };//class FTel_VzCorrection
}
