#include "FDn_pathCorrection.h"

namespace chanser{

  void FDn_pathCorrection::ParticleCorrect(BaseParticle* part) const noexcept{
    if(_useC12Neutral){C12NeutronCorrection(part);}
    else{C12ParticleCorrection(static_cast<chanser::CLAS12Particle*>(part));}
  }

  void FDn_pathCorrection::C12NeutronCorrection(BaseParticle* part) const noexcept{
    auto neutron = dynamic_cast<const CLAS12Neutral*>(part);
    //Just in case the wrong value for _useC12Neutral flag is used
    if(!neutron->isValid()){ 
      C12ParticleCorrection(dynamic_cast<chanser::CLAS12Particle*>(part));
      return;
    }
    auto candidates=neutron->Candidates(); 
    for(auto c12p:candidates){
      C12ParticleCorrection(c12p);
    }
  }//EndOf C12NeutronCorrection

  void FDn_pathCorrection::C12ParticleCorrection(CLAS12Particle* c12p) const noexcept{
    auto c12=c12p->CLAS12();
 
    if(c12->getRegion()!=clas12::FD){
      return; //only FD n 
    }

    int layerhit=0;
    if(c12->cal(clas12::PCAL)->getEnergy()>0.01){
      layerhit=0;
    } else if(c12->cal(clas12::ECIN)->getEnergy()>0.01){
      layerhit=1;
    } else if(c12->cal(clas12::ECOUT)->getEnergy()>0.01){
      layerhit=2;
    }

    //c12p->Time already start time corrected
    //have to make sure start time correction called before?
    //ordered PCAL if exists, then ECIN, then ECOUT
    auto nTime = c12p->Time();

    //c12p->Path in meters, we want cm
    double nPath=(c12p->Path()*1e+02)+_corLayer[layerhit]; 
   
    double nCalcBeta=(nPath*1e-2)/(nTime*1e-9*2.99792e+08);
    double nCalcP=0.93957*nCalcBeta/sqrt(1-nCalcBeta*nCalcBeta);
  
    auto sinTh=TMath::Sin(c12->getTheta());	    
    auto cosTh=TMath::Cos(c12->getTheta());
    auto sinPhi=TMath::Sin(c12->getPhi());	    
    auto cosPhi=TMath::Cos(c12->getPhi());

    double Px=nCalcP*sinTh*cosPhi;
    double Py=nCalcP*sinTh*sinPhi;
    double Pz=nCalcP*cosTh;
    double M=0.93957;
    c12p->SetXYZM(Px,Py,Pz,M);
    //c12p->SetMeasMass(M); ?
    //we used path in cm, but c12p in meters
    c12p->SetPath(nPath*1e-02);
  }//EndOf C12ParticleCorrection

  void FDn_pathCorrection::ChangeRun() {

    //Allow user specified parameters
    if(_useDefault){
      auto period=RunPeriodType();
      //Get parameters
      auto table = GetAnaDB().GetTable(period,"FDN_PATHCORRECTION",{3});
      if(table.IsValid()){
	tablevals_t tmp(3);
	table.Fill(tmp);
	_corLayer=tmp;
      }
    }

  }//EndOf ChangeRun

}
