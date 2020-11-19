//////////////////////////////////////////////////////////////
///
///Class:	FiducialCut_DC_XY	
///Description:
///     DC Fiducial cut in local XY variables

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"

namespace chanser{


  class FiducialCut_DC_XY : public BaseCut, public CLAS12Base{

    using Array4D = std::vector<std::vector<std::vector<std::vector<double>>>>;
      
  public:
    FiducialCut_DC_XY()=default; // must have default constructor
    FiducialCut_DC_XY(TString pid);
    
    void ChangeRun() final;
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{

      if(_fieldVal==-1 && _partPidVal >0)
	{return kTRUE;}

      Int_t layersPassed = 0;

      for(int _layer=0; _layer<3; _layer++)
	{
	  Int_t _regionVal = 0;
	  if(_layer==0){
	    _regionVal=6;
	  }
	  else if(_layer==1){
	    _regionVal=18;
	  }
	  else if(_layer==2){
	    _regionVal=36;
	  }
	  else{
	    std::cerr<<"FiducialCut_DC_XY, invalid layer should be 1,2,3 "<<std::endl;
	    exit(0);
	  }

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
      
      if ((Y > calc_min) && (Y < calc_max))
	{layersPassed++;}
  
	}

      return (layersPassed == 3)? kTRUE : kFALSE;
    }
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" applied to "<<_partPidVal<<std::endl;
    }
    
  private:
    // Short_t _regionVal={0};
    // Short_t _layer={0};
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
      
    ClassDefOverride(chanser::FiducialCut_DC_XY,1);

  };//class FiducialCut_DC_XY

}
