//////////////////////////////////////////////////////////////
///
///Class:	FiducialCut_DC_ThetaPhi	
///Description:
///     DC Fiducial cuts in local theta and phi coords

#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"
#include "AnaDB.h"

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
      if(_applicable==kFALSE) return kTRUE;

      Int_t layersPassed = 0;

      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data
      
      if(c12->getRegion()!=clas12::FD) return true; //cut only applies to FD

      Short_t regionVal[3]={6,18,36};

      for(int layer = 0; layer<3; layer++)
	{
      //Int_t _partPidVal = c12->par()->getPid();
      
      auto trajZ = c12->traj(clas12::DC, regionVal[layer])->getZ();
      auto trajY = c12->traj(clas12::DC, regionVal[layer])->getY();
      auto trajX = c12->traj(clas12::DC, regionVal[layer])->getX();
      
      auto trajR = TMath::Sqrt(trajX*trajX+trajY*trajY+trajZ*trajZ);
      
      auto theta_DCr = TMath::RadToDeg() * TMath::ACos(trajZ/trajR);
      auto phi_DCr_raw = TMath::RadToDeg() * TMath::ATan2(trajY, trajX);
      
      short sector = c12->getSector() -1; //start at zero from here on
      
      short pos=sector;
      if (sector == 3 && phi_DCr_raw > 0)
	pos = 6; //+180 rather than -180

      auto phi_DCr = phi_DCr_raw + _phiRotation[pos];
      
      double calc_phi_min = _minparams[sector][layer][0]
	+ _minparams[sector][layer][1] * std::log(theta_DCr) 
	+ _minparams[sector][layer][2] * theta_DCr
	+ _minparams[sector][layer][3] * theta_DCr * theta_DCr;
      
      double calc_phi_max = _maxparams[sector][layer][0]
	+ _maxparams[sector][layer][1] * std::log(theta_DCr)
	+ _maxparams[sector][layer][2] * theta_DCr
	+ _maxparams[sector][layer][3] * theta_DCr * theta_DCr;

      layersPassed+= ((phi_DCr > calc_phi_min) && (phi_DCr < calc_phi_max));
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

    size_t _NSectors={6};
    size_t _NLayers={3};
    size_t _NCoefs={4}; //4 polynomial params
    
    std::vector<int> _phiRotation={0,-60,-120,-180, 120, 60, 180};

    tablevals3D_t _minparams;
    tablevals3D_t _maxparams;

    std::string _pname;

    Bool_t _applicable={true};    


    ClassDefOverride(chanser::FiducialCut_DC_ThetaPhi,1);

  };//class FiducialCut_DC_ThetaPhi

}
