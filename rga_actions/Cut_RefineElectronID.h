//////////////////////////////////////////////////////////////
///
///Class:		
///Description:  Cut_RefineElectronID
///              Cut to accept only electrons with sampling fraction within a 
///              3.5 sigma band around the mean from fitted data. Also includes 
///              a sloped cut for electrons over 4.5 GeV
///              Details in RGA analysis note section 7.4.1.2
///Author :      A. Thornton

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"

namespace chanser{


  class Cut_RefineElectronID : public BaseCut, public CLAS12Base{
      
  public:
    Cut_RefineElectronID()=default; // must have default constructor

    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12();

      //variables for calc
      double sigma_range = 3.5;
      double mean = 0;
      double sigma = 0;
      double upper_lim_total = 0;
      double lower_lim_total = 0;
      bool pass_triangle = false;

      double part_p = part->P4().P();
      double ECIN_en = c12->cal(clas12::ECIN)->getEnergy();
      double ECOUT_en = c12->cal(clas12::ECOUT)->getEnergy();
      double EC_en_total = ECIN_en + ECOUT_en;
      double PCAL_en = c12->cal(clas12::PCAL)->getEnergy();
      int PCAL_sec = c12->cal(clas12::PCAL)->getSector()-1;

      //calc the mean and sigma, for specific sector -> method from S. Dhiel
      mean = _ecal_e_sampl_mu[0][PCAL_sec] + 
        (_ecal_e_sampl_mu[1][PCAL_sec] / 1000*TMath::Power(part_p-_ecal_e_sampl_mu[2][PCAL_sec],2));
      sigma = _ecal_e_sampl_sigma[0][PCAL_sec] + 
        (_ecal_e_sampl_sigma[1][PCAL_sec] / (10*(part_p-_ecal_e_sampl_sigma[2][PCAL_sec])));
      upper_lim_total = mean + sigma_range * sigma;
      lower_lim_total = mean - sigma_range * sigma;

      //electron within 3.5 sigma band
      bool pass_band = (EC_en_total/part_p <= upper_lim_total) && 
        (EC_en_total/part_p >= lower_lim_total);

      //triangular cut as seen in RGA analysis note fig 78
      if(part_p < 4.5){ 
        pass_triangle = true;
      } else {
        pass_triangle = ECIN_en/part_p > (0.2 - PCAL_en/part_p);
      }
      return (pass_band && pass_triangle)? kTRUE : kFALSE;
    }

    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<std::endl;
    }

  private:
    
    double _ecal_e_sampl_mu[3][6];
    double _ecal_e_sampl_sigma[6][6];

    ClassDefOverride(chanser::Cut_RefineElectronID,1);

  };//class Cut_RefineElectronID

}
