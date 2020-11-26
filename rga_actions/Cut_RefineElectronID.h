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
    Cut_RefineElectronID();
    void ChangeRun() final;

    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12();
      if(c12->getRegion()!=clas12::FD) return true; //cut only applies to FD

      double ecal_e_sampl_mu[3][6] = {
        {  0.2531 ,  0.2550 ,  0.2514 ,  0.2494,  0.2528 ,  0.2521 },
        { -0.6502 , -0.7472 , -0.7674 , -0.4913, -0.3988 , -0.703  },
        {  4.939  ,  5.350  ,  5.102  ,  6.440 ,  6.149  ,  4.957  }
      };

      double ecal_e_sampl_sigma[3][6] = {
          {  2.726e-3 ,  4.157e-3 ,  5.222e-3 , 5.398e-3 ,  8.453e-3 ,  6.533e-3 },
          {  1.062    ,  0.859    ,  0.5564   , 0.6576   ,  0.3242   ,  0.4423   },
          { -4.089    , -3.318    , -2.078    ,-2.565    , -0.8223   , -1.274    }
      };

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
      short PCAL_sec = c12->cal(clas12::PCAL)->getSector();

      std::cout<<_fitparams_mean[0][PCAL_sec]<<" "<<ecal_e_sampl_mu[0][PCAL_sec]<<std::endl;

      //calc the mean and sigma, for specific sector -> method from S. Dhiel
      mean = ecal_e_sampl_mu[0][PCAL_sec] + 
        (ecal_e_sampl_mu[1][PCAL_sec] / 1000*TMath::Power(part_p-ecal_e_sampl_mu[2][PCAL_sec],2));
      sigma = ecal_e_sampl_sigma[0][PCAL_sec] + 
        (ecal_e_sampl_sigma[1][PCAL_sec] / (10*(part_p-ecal_e_sampl_sigma[2][PCAL_sec])));
      upper_lim_total = mean + sigma_range * sigma;
      lower_lim_total = mean - sigma_range * sigma;

      //electron within 3.5 sigma band
      bool pass_band = ((EC_en_total/part_p) <= upper_lim_total) && ((EC_en_total/part_p) >= lower_lim_total);
      
      //triangular cut as seen in RGA analysis note fig 78
      if(part_p < 4.5){ 
        pass_triangle = true;
      } else {
        pass_triangle = ECIN_en/part_p > (0.2 - PCAL_en/part_p);
      }
      return pass_band && pass_triangle;
    }

    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<std::endl;
    }

  private:

    tablevals2D_t _fitparams_mean;
    tablevals2D_t _fitparams_sigma;
    Bool_t _applicable={true};    
    
    ClassDefOverride(chanser::Cut_RefineElectronID,1);

  };//class Cut_RefineElectronID

}
