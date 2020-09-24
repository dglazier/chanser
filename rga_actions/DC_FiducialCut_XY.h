//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     DC Fiducial cut in local XY variables

#pragma once

#include "BaseCut.h"

namespace chanser{


  class DC_FiducialCut_XY : public chanser::BaseCut{

    using Array4D = std::vector<std::vector<std::vector<std::vector<double>>>>;
      
  public:
    DC_FiducialCut_XY()=default; // must have default constructor
    DC_FiducialCut_XY(Int_t layer, Int_t field);
    
    
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      
      if(c12->getRegion()!=clas12::FD) return true; //cut only appies to FD
      
      bool inbending = (_fieldVal==-1); //false is -1
      
      const auto& minparams = (inbending ? minparams_in : minparams_out);
      const auto& maxparams = (inbending ? maxparams_in : maxparams_out);
      
      
      double X;
      double Y;
      Double_t Pival = TMath::Pi();
      auto sector = c12->getSector();
      
      X = c12->traj(clas12::DC,_regionVal)->getX();
      Y = c12->traj(clas12::DC,_regionVal)->getY();
      

      if(sector == 2)
	{
	  const double X_new = X * std::cos(-60 * Pival / 180.) - Y * std::sin(-60 * Pival / 180.);
	  Y = X * std::sin(-60 * Pival / 180.) + Y * std::cos(-60 * Pival / 180.);
	  X = X_new;
	}
      
      else if(sector == 3)
	{
	  const double X_new = X * std::cos(-120 * Pival / 180.) - Y * std::sin(-120 * Pival / 180.);
	  Y = X * std::sin(-120 * Pival / 180.) + Y * std::cos(-120 * Pival / 180.);
	  X = X_new;
	}
      
      else if(sector == 4)
	{
	  const double X_new = X * std::cos(-180 * Pival / 180.) - Y * std::sin(-180 * Pival / 180.);
	  Y = X * std::sin(-180 * Pival / 180.) + Y * std::cos(-180 * Pival / 180.);
	  X = X_new;
	}
      
      else if(sector == 5)
	{
	  const double X_new = X * std::cos(120 * Pival / 180.) - Y * std::sin(120 * Pival / 180.);
	  Y = X * std::sin(120 * Pival / 180.) + Y * std::cos(120 * Pival / 180.);
	  X = X_new;
	}
      
      else if(sector == 6)
	{
	  const double X_new = X * std::cos(60 * Pival / 180.) - Y * std::sin(60 * Pival / 180.);
	  Y = X * std::sin(60 * Pival / 180.) + Y * std::cos(60 * Pival / 180.);
	  X = X_new;
	}
      
      int pid = 0;
      Int_t _partPidVal = c12->par()->getPid();
      
      if(_partPidVal == 11) pid = 0;
      else if(_partPidVal == 2212) pid = 1;
      else if(_partPidVal == 211) pid = 2;
      else if(_partPidVal == -211) pid = 3;
      else if(_partPidVal == 321) pid = 4;
      else if(_partPidVal == -321) pid = 5;
      
      Int_t layer = 5;
      if(_regionVal==6){layer=0;}
      else if(_regionVal==18){layer=1;}
      else if(_regionVal==36){layer=2;}
      
      if(inbending == true) pid = 0; // use only for electrons in inbending case
      
      double calc_min = minparams[pid][sector - 1][layer][0] + minparams[pid][sector - 1][layer][1] * X;
      double calc_max = maxparams[pid][sector - 1][layer][0] + maxparams[pid][sector - 1][layer][1] * X;
      
      return ((Y > calc_min) && (Y < calc_max));
      
      
    }
    
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" applied "<<std::endl;
    }
    
  private:
    Int_t _regionVal = chanser::THIS_INT_MAX2;
    Int_t _fieldVal = chanser::THIS_INT_MAX2;
    Array4D maxparams_in;
    Array4D maxparams_out;
    Array4D minparams_in;
    Array4D minparams_out;
      
    ClassDefOverride(chanser::DC_FiducialCut_XY,1);

  };//class DC_FiducialCut_XY

}
