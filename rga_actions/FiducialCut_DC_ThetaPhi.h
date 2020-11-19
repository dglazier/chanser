//////////////////////////////////////////////////////////////
///
///Class:	FiducialCut_DC_ThetaPhi	
///Description:
///     DC Fiducial cuts in local theta and phi coords

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"

namespace chanser{


  class FiducialCut_DC_ThetaPhi : public BaseCut , public CLAS12Base {
    
    using Array4D = std::vector<std::vector<std::vector<std::vector<double>>>>;

    //enum class ParticleCode {El,Pr,Pip,Pim,Kp,Km};

    
  public:
    FiducialCut_DC_ThetaPhi()=default; // must have default constructor
    FiducialCut_DC_ThetaPhi(TString pid);
    
    void ChangeRun() final;

    
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override
    {
      if(_fieldVal==1 || _partPidVal ==0)
      	{return kTRUE;}

      Int_t layersPassed = 0;

      for (int _layer=0; _layer<3; _layer++)
	{
	  Int_t _regionVal=0;
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
	    std::cerr<<"FiducialCut_DC_ThetaPhi, invalid layer should be 1,2,3 "<<std::endl;
	    exit(0);
	  }

      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      
      if(c12->getRegion()!=clas12::FD) return true; //cut only applies to FD
      
     
      //Int_t _partPidVal = c12->par()->getPid();
      
      auto trajZ = c12->traj(clas12::DC, _regionVal)->getZ();
      auto trajY = c12->traj(clas12::DC, _regionVal)->getY();
      auto trajX = c12->traj(clas12::DC, _regionVal)->getX();
      
      auto trajR = TMath::Sqrt(trajX*trajX+trajY*trajY+trajZ*trajZ);
      
      auto theta_DCr = TMath::RadToDeg() * TMath::ACos(trajZ/trajR);
      auto phi_DCr_raw = TMath::RadToDeg() * TMath::ATan2(trajY, trajX);
      
      short sector = c12->getSector() -1; //start at zero from here on
      
      short pos=sector;
      if (sector == 3 && phi_DCr_raw > 0)
	pos = 6; //+180 rather than -180

      auto phi_DCr = phi_DCr_raw + _phiRotation[pos];
      
      double calc_phi_min = _minparams[_partPidVal][sector][_layer][0]
	+ _minparams[_partPidVal][sector][_layer][1] * std::log(theta_DCr) 
	+ _minparams[_partPidVal][sector][_layer][2] * theta_DCr
	+ _minparams[_partPidVal][sector][_layer][3] * theta_DCr * theta_DCr;
      
      double calc_phi_max = _maxparams[_partPidVal][sector][_layer][0]
	+ _maxparams[_partPidVal][sector][_layer][1] * std::log(theta_DCr)
	+ _maxparams[_partPidVal][sector][_layer][2] * theta_DCr
	+ _maxparams[_partPidVal][sector][_layer][3] * theta_DCr * theta_DCr;
      
      if ((phi_DCr > calc_phi_min) && (phi_DCr < calc_phi_max)) 
	{layersPassed++;}
	}
      
	return (layersPassed ==3)? kTRUE : kFALSE;

    }
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" applied to "<<_partPidVal<<std::endl;
    }

  private:
    //  Short_t _regionVal={0};
    //Short_t _layer={0};
    Short_t _fieldVal = {0};

    Short_t _partPidVal= {0};
    
    std::vector<int> _phiRotation={0,-60,-120,-180, 120, 60, 180};
  
    Array4D _minparams;
    Array4D _maxparams;
    Array4D _maxparams_in;
    Array4D _maxparams_out;
    Array4D _minparams_in;
    Array4D _minparams_out;
    


    ClassDefOverride(chanser::FiducialCut_DC_ThetaPhi,1);

  };//class FiducialCut_DC_ThetaPhi

}
