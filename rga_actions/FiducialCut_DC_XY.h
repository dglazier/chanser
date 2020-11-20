//////////////////////////////////////////////////////////////
///
///Class:	FiducialCut_DC_XY	
///Description:
///     DC Fiducial cut in local XY variables

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"
#include "AnaDB.h"

namespace chanser{


  class FiducialCut_DC_XY : public BaseCut, public CLAS12Base{

       
  public:
    FiducialCut_DC_XY()=default; // must have default constructor
    FiducialCut_DC_XY(TString pid);
    
    void ChangeRun() final;
      
  

    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{

      if(_applicable==kFALSE) return kTRUE;
  
      Int_t layersPassed = 0;
      
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      
      if(c12->getRegion()!=clas12::FD) return true; //cut only appies to FD

      short sector = c12->getSector() -1; //start at zero from here on

      Short_t regionVal[3]={6,18,36};

      for(int layer=0; layer<3; layer++){ 
	  
	auto X = c12->traj(clas12::DC,regionVal[layer])->getX();
	auto Y = c12->traj(clas12::DC,regionVal[layer])->getY();
	  
	  
	//Rotate into "local" coordinates
	const double X_new = X * _cosPhiRot[sector] - Y * _sinPhiRot[sector];
	Y = X * _sinPhiRot[sector] + Y * _cosPhiRot[sector];
	X = X_new;
	  
	double calc_min = _minparams[sector][layer][0]
	  + _minparams[sector][layer][1] * X;
	double calc_max = _maxparams[sector][layer][0]
	  + _maxparams[sector][layer][1] * X;
	  	  
	layersPassed+= ((Y > calc_min) && (Y < calc_max));
      }
            
      return (layersPassed == 3) ? kTRUE : kFALSE;
    }
    
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" applied to "<<_partPidVal<<" "<<_pname<<std::endl;
    }
    
  private:
  
    // Short_t _regionVal={0};
    //Short_t _layer={0};
    Short_t _fieldVal = {0};

    Short_t _partPidVal= {-1};

    size_t _NSectors={6};
    size_t _NLayers={3};
    size_t _NCoefs={2}; //constant + linear
    
    std::vector<double> _cosPhiRot;
    std::vector<double> _sinPhiRot;

    tablevals3D_t _minparams;
    tablevals3D_t _maxparams;

    std::string _pname;
    
    Bool_t _applicable ={true};

    ClassDefOverride(chanser::FiducialCut_DC_XY,1);

  };//class FiducialCut_DC_XY

}
