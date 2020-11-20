#pragma once
#include "BaseCorrection.h"
#include "CLAS12Base.h"
#include "clas12defs.h"

namespace chanser{

  class FTel_VzCorrection : public BaseCorrection, public CLAS12Base{
      
  public:
    FTel_VzCorrection()=default;

  FTel_VzCorrection(Double_t zpos):
    _zpos{zpos}{};
      
    void ChangeRun() final{

      auto period = GetRunInfo()->_runPeriod+"_" + GetRunInfo()->_fieldSetting;
 
      //Get maximum parameters
      auto table = GetAnaDB().GetTable(period,"FTEL_VZCORRECTION",{1});
      tablevals_t val(1);
      if(table.IsValid()){
	table.Fill(val);
	_zpos=val[0];
      }
      std::cout<<"FTel_VzCorrection set z position to "<<_zpos<<std::endl;
    }
 
   
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
