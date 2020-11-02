//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     DC Fiducial cut in local XY variables

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"

namespace chanser{


  class DC_FiducialCut_XY : public BaseCut, public CLAS12Base{

    using Array4D = std::vector<std::vector<std::vector<std::vector<double>>>>;
      
  public:
    DC_FiducialCut_XY()=default; // must have default constructor
    DC_FiducialCut_XY(TString pid, Int_t field);
    
    void ChangeRun() final;
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      
      if(c12->getRegion()!=clas12::FD) return true; //cut only appies to FD

      auto X = c12->traj(clas12::DC,_regionVal)->getX();
      auto Y = c12->traj(clas12::DC,_regionVal)->getY();
  
      short sector = c12->getSector() -1; //start at zero from here on
      
      //Rotate into "local" coordinates
      const double X_new = X * _cosPhiRot[sector] - Y * _sinPhiRot[sector];
      Y = X * _sinPhiRot[sector] + Y * _cosPhiRot[sector];
      X = X_new;
      
      
      double calc_min = _minparams[_partPidVal][sector][_layer][0]
	+ _minparams[_partPidVal][sector][_layer][1] * X;
      double calc_max = _maxparams[_partPidVal][sector][_layer][0]
	+ _maxparams[_partPidVal][sector][_layer][1] * X;
      
      return ((Y > calc_min) && (Y < calc_max));
  
    }
    
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" applied to "<<_partPidVal<<" for layer "<<_layer<<std::endl;
    }
    
  private:
    Short_t _regionVal={0};
    Short_t _layer={0};
    Short_t _fieldVal = {0};

    Short_t _partPidVal= {0};

    std::vector<double> _cosPhiRot;
    std::vector<double> _sinPhiRot;
    
    Array4D _minparams;
    Array4D _maxparams;
    Array4D _maxparams_in;
    Array4D _maxparams_out;
    Array4D _minparams_in;
    Array4D _minparams_out;
      
    ClassDefOverride(chanser::DC_FiducialCut_XY,1);

  };//class DC_FiducialCut_XY

}
