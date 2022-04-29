#pragma once
#include "BaseCorrection.h"
#include "CLAS12Base.h"
#include "clas12defs.h"

namespace chanser{

  class FDn_pathCorrection : public BaseCorrection, public CLAS12Base{
      
  public:
    FDn_pathCorrection()=default;
    
  FDn_pathCorrection(Double_t corPCAL,Double_t corECIN,Double_t corECOUT):
    _corPCAL{corPCAL},_corECIN{corECIN},_corECOUT{corECOUT}{};


    void ParticleCorrect(BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12();
      if(c12->getRegion()!=clas12::FD)
	return; //only FD n

      double startTime=GetEventInfo()->CLAS12()->event()->getStartTime();
     
      double nPCALTime=c12->cal(clas12::PCAL)->getTime();
      double nECinTime=c12->cal(clas12::ECIN)->getTime();
      double nECoutTime=c12->cal(clas12::ECOUT)->getTime();

      double nPath=c12->cal(clas12::PCAL)->getPath()+_corPCAL;
      double nTime=c12->cal(clas12::PCAL)->getTime()-startTime;

      if(c12->cal(clas12::PCAL)->getEnergy()>0.01){
	nPath=c12->cal(clas12::PCAL)->getPath()+_corPCAL;
	nTime=c12->cal(clas12::PCAL)->getTime()-startTime;
      } else if(c12->cal(clas12::ECIN)->getEnergy()>0.01){
	nPath=c12->cal(clas12::ECIN)->getPath()+_corECIN;
	nTime=c12->cal(clas12::ECIN)->getTime()-startTime;
      } else if(c12->cal(clas12::ECOUT)->getEnergy()>0.01){
	nPath=c12->cal(clas12::ECOUT)->getPath()+_corECOUT;
	nTime=c12->cal(clas12::ECOUT)->getTime()-startTime;
      }
      double nCalcBeta=(nPath*1e-2)/(nTime*1e-9*2.99792e+08);
      double nCalcP=0.93957*nCalcBeta/sqrt(1-nCalcBeta*nCalcBeta);

      part->SetP(nCalcP);
      
    }
    
   private:

    Double_t _corPCAL{0};
    Double_t _corECOUT{0};
    Double_t _corECIN{0};
   
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with PCAL "<<_corPCAL<<", ECIN "<<_corECIN<<", ECOUT "<<_corECOUT<<std::endl;
    }

  private:
       
    ClassDefOverride(chanser::FDn_pathCorrection,1);

  };//class FTel_VzCorrection
}
