//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     DC Fiducial cuts in theta and phi

#pragma once

#include "BaseCut.h"

namespace chanser{


  class DC_FiducialCut_ThetaPhi : public chanser::BaseCut{
    
    using Array4D = std::vector<std::vector<std::vector<std::vector<double>>>>;      
    
  public:
    DC_FiducialCut_ThetaPhi()=default; // must have default constructor
    DC_FiducialCut_ThetaPhi(Int_t layer, Int_t field);
    
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override
    {
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      
      if(c12->getRegion()!=clas12::FD) return true; //cut only applies to FD
      
      
      bool inbending = (_fieldVal==-1); //false if -1
      
      const auto& minparams = (inbending ? minparams_in : minparams_out);
      const auto& maxparams = (inbending ? maxparams_in : maxparams_out);
      
      double theta_DCr = 5000;
      double phi_DCr_raw = 5000;
      Double_t Pival = TMath::Pi();
      Int_t _partPidVal = c12->par()->getPid();
      
      auto trajZ = c12->traj(clas12::DC, _regionVal)->getZ();
      auto trajY = c12->traj(clas12::DC, _regionVal)->getY();
      auto trajX = c12->traj(clas12::DC, _regionVal)->getX();
      
      auto trajR = TMath::Sqrt(trajX*trajX+trajY*trajY+trajZ*trajZ);
      
      theta_DCr = TMath::RadToDeg() * TMath::ACos(trajZ/trajR);
      phi_DCr_raw = TMath::RadToDeg() * TMath::ATan2(trajY, trajX);
      
      double phi_DCr = 5000;
      auto sector = c12->getSector();
      
      if (sector == 1) phi_DCr = phi_DCr_raw;
      else if (sector == 2) phi_DCr = phi_DCr_raw - 60;
      else if (sector == 3) phi_DCr = phi_DCr_raw - 120;
      else if (sector == 4 && phi_DCr_raw > 0) phi_DCr = phi_DCr_raw - 180;
      else if (sector == 4 && phi_DCr_raw < 0) phi_DCr = phi_DCr_raw + 180;
      else if (sector == 5) phi_DCr = phi_DCr_raw + 120;
      else if (sector == 6) phi_DCr = phi_DCr_raw + 60;
      
      int pid = 0;
      if(_partPidVal == 11) pid=0;
      else if (_partPidVal==2212) pid=1;
      else if (_partPidVal==211) pid= 2;
      else if (_partPidVal==-211) pid = 3;
      else if (_partPidVal==321) pid=4;
      else if (_partPidVal==-321) pid=5;
      
      Int_t layer;
      layer = 5;
      if(_regionVal==6){layer = 0;}
      else if(_regionVal==18){layer=1;}
      else if(_regionVal==36){layer=2;}
      
      double calc_phi_min = minparams[pid][sector - 1][layer][0] + minparams[pid][sector - 1][layer][1] * std::log(theta_DCr) 
	+ minparams[pid][sector - 1][layer][2] * theta_DCr + minparams[pid][sector - 1][layer][3] * theta_DCr * theta_DCr;
      
      double calc_phi_max = maxparams[pid][sector - 1][layer][0] + maxparams[pid][sector - 1][layer][1] * std::log(theta_DCr)
	+ maxparams[pid][sector - 1][layer][2] * theta_DCr + maxparams[pid][sector - 1][layer][3] * theta_DCr * theta_DCr;
      
      return ((phi_DCr > calc_phi_min) && (phi_DCr < calc_phi_max))? kTRUE : kFALSE;
    };
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" applied"<<std::endl;
    }

  private:
    Int_t _regionVal=chanser::THIS_INT_MAX2;
    Int_t _fieldVal = chanser::THIS_INT_MAX2;
    Array4D maxparams_in;
    Array4D maxparams_out;
    Array4D minparams_in;
    Array4D minparams_out;
    


    ClassDefOverride(chanser::DC_FiducialCut_ThetaPhi,1);

  };//class DC_FiducialCut_ThetaPhi

}
