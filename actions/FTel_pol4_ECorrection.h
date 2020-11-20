#pragma once
#include "BaseCorrection.h"
#include "CLAS12Base.h"
#include "clas12defs.h"

namespace chanser{

  class FTel_pol4_ECorrection : public BaseCorrection, public CLAS12Base{
      
  public:
    FTel_pol4_ECorrection()=default;
  FTel_pol4_ECorrection(Double_t a0,Double_t a1,Double_t a2,Double_t a3,Double_t a4):
    _a0{a0},_a1{a1},_a2{a2},_a3{a3},_a4{a4}{};
      

    void ChangeRun() final{

      std::cout<<" FTel_pol4_ECorrection "<<std::endl;

      auto period = GetRunInfo()->_runPeriod+"_" + GetRunInfo()->_fieldSetting;
 
      //Get maximum parameters
      auto table = GetAnaDB().GetTable(period,"FTEL_POL4_ECORRECTION",{5});
      if(table.IsValid()){
	tablevals_t val(5);
	table.Fill(val);
	_a0=val[0];
 	_a1=val[1];
	_a2=val[2];
	_a3=val[3];
	_a4=val[4];
      }
    }
 

    void ParticleCorrect(BaseParticle* part) const noexcept override{
      if(static_cast<CLAS12Particle*>(part)->CLAS12()->getRegion()!=clas12::FT)
	return; //only FT e-
      
      Double_t p_mag = part->P4p()->P();
      if(p_mag==0)return;
      
      Double_t p_mag_corrected = (_a0 + _a1*p_mag + _a2*p_mag*p_mag
				+ _a3*p_mag*p_mag*p_mag
				+ _a4*p_mag*p_mag*p_mag*p_mag);
    
      part->SetP(p_mag_corrected);

    }
   private:

    Double_t _a0{-0.03689};
    Double_t _a1{1.1412};
    Double_t _a2{-0.04316};
    Double_t _a3{0.007046};
    Double_t _a4{-0.0004055};
   
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with a0= "<<_a0<< " a1= "<<_a1<< " a2= "<<_a2<< " a3= "<<_a3<< " a4= "<<_a4<<std::endl;
    }

  private:
       
    ClassDefOverride(chanser::FTel_pol4_ECorrection,1);

  };//class FTel_pol4_ECorrection
}
