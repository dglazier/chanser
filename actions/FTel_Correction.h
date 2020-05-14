#pragma once
#include "BaseCorrection.h"
#include "clas12defs.h"

namespace chanser{

  class FTel_Correction : public BaseCorrection{
      
  public:
    FTel_Correction()=default;
  FTel_Correction(Double_t a0,Double_t a1,Double_t a2,Double_t a3):
    _a0{a0},_a1{a1},_a2{a2},_a3{a3},_a4{a4}{};
      
      
    void ParticleCorrect(BaseParticle* part) const noexcept override{
      if(static_cast<CLAS12Particle*>(part)->CLAS12()->getRegion()!=FT)
	return; //only FT e-
      
      double p_mag = part->P4p()->P();
      
      double p_mag_corrected = (_a0 + _a1*p_mag + _a2*p_mag*pmag
				+ _a3*p_mag*p_mag*p_mag
				+ _a4*p_mag*p_mag*p_mag*pmag);

      part->SetP(p_mag_corrected);

    }
   private:

    Double_t _a0{-0.03689};
    Double_t _a1{1.1412};
    Double_t _a2{- 0.04316};
    Double_t _a3{0.007046};
    Double_t _a4{-0.0004055};
   
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with a0= "<<_a0<< " a1= "<<_a1<< " a2= "<<_a2<< " a3= "<<_a3<< " a4= "<<_a4<<std::endl;
    }

  private:
       
    ClassDefOverride(chanser::FTel_Correction,1);

  };//class FTel_Correction
}
