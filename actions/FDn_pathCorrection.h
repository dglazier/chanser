#pragma once
#include "BaseCorrection.h"
#include "CLAS12Base.h"
#include "CLAS12Neutral.h"
#include "CLAS12Particle.h"
#include "clas12defs.h"

namespace chanser{

  class FDn_pathCorrection : public BaseCorrection, public CLAS12Base{
      
  public:
    FDn_pathCorrection()=default;

    FDn_pathCorrection(Bool_t useC12Neutral):_useC12Neutral{useC12Neutral}{
      _useDefault=true;
    };
    
  FDn_pathCorrection(Double_t corPCAL,Double_t corECIN,Double_t corECOUT,Bool_t useC12Neutral):_useC12Neutral{useC12Neutral}{
      _corLayer.push_back(corPCAL);
      _corLayer.push_back(corECIN);
      _corLayer.push_back(corECOUT);
      _useDefault=false;
    };


    void ParticleCorrect(BaseParticle* part) const noexcept override;
    void C12NeutronCorrection(BaseParticle* part) const noexcept;
    void C12ParticleCorrection(CLAS12Particle* c12p) const noexcept;

    void ChangeRun() final;
    
   private:

    tablevals_t _corLayer;
    Bool_t _useC12Neutral{false};
    Bool_t _useDefault{true};
   
    void Print(Option_t* option = "")const final{
      if(_useDefault){
	std::cout<<"\t\t"<<ClassName()<<" default corrections specified in anadb "<<std::endl;
      } else{
	std::cout<<"\t\t"<<ClassName()<<" with PCAL "<<_corLayer[0]<<", ECIN "<<_corLayer[1]<<", ECOUT "<<_corLayer[2]<<std::endl;
      }
    }

  private:
       
    ClassDefOverride(chanser::FDn_pathCorrection,1);

  };//class FTel_VzCorrection
}
