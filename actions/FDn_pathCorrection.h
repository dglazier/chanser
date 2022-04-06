#pragma once
#include "BaseCorrection.h"
#include "CLAS12Base.h"
#include "clas12defs.h"

namespace chanser{

  class FDn_pathCorrection : public BaseCorrection, public CLAS12Base{
      
  public:
    FDn_pathCorrection()=default;
    
  FDn_pathCorrection(Double_t corPCAL,Double_t corECIN,Double_t corECOUT):
    _corPCAL{corPCAL},_corECIN{corECIN},_corECOUT{corECOUT},_useDefault{1}{};


    void ChangeRun() final{


      if(_useDefault==0){

	/*In simulated data, the time used to calculate/apply correction
	  is uncalibrated, leading to different corrections for
	  MC and data*/
	std::cout<<"FDn path Cor use default"<<std::endl;
	if(GetFinalState()->HasTruth()){
	  std::cout<<"Has Truth"<<std::endl;
	  _corPCAL=8.13;
	  _corECIN=26.51;
	  _corECOUT=29.48;
	  std::cout<<"set vals"<<std::endl;
	} else{

	  /*There's a momentum dependence of the correction. This is hard
	    to establish as we're aiming to correct the momentum. Therefore
	    we use the average correction, which shifts with the beam energy.*/
	  Double_t BeamE=GetFinalState()->GetRunInfo()->_BeamEnergy;
	  std::cout<<"beam e: "<<BeamE<<std::endl;
	  if(BeamE>10.5){
	    _corPCAL=6.212;
	    _corECIN=7.927;
	    _corECOUT=3.044;
	  }
	  else if(BeamE<10.3){
	    _corPCAL=10.27;
	    _corECIN=10.89;
	    _corECOUT=5.584;
	  }
	}
      }
      
    }

   
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

    int _useDefault{0};
    Double_t _corPCAL{0};
    Double_t _corECOUT{0};
    Double_t _corECIN{0};
   
    
    void Print(Option_t* option = "")const final{
      if(_useDefault!=0){
	std::cout<<"\t\t"<<ClassName()<<" with PCAL "<<_corPCAL<<", ECIN "<<_corECIN<<", ECOUT "<<_corECOUT<<std::endl;
      } else{
	std::cout<<"\t\t"<<ClassName()<<" with default corrections."<<std::endl;
      }
    }

  private:
       
    ClassDefOverride(chanser::FDn_pathCorrection,1);

  };//class FTel_VzCorrection
}
