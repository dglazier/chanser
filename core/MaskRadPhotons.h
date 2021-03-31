//////////////////////////////////////////////////////////////
///
///Class:		MaskRadPhotons
///Description:
///    Class responsible for removing photons radiated from electrons
/// and correcting the electron momentum.
///Author:
///     Richard Tyson

#pragma once

#include "TLorentzVector.h"
#include "MaskedEventParticles.h"
#include "CLAS12Particle.h"
#include "clas12defs.h" //from clas12root

#include <TH1F.h>
#include <TH2F.h>

namespace chanser{

  //inherit from public CLAS12Base when have dbs code
  class MaskRadPhotons : public MaskedEventParticles{
      
  public :
    MaskRadPhotons()=default;

  MaskRadPhotons(Float_t ecalR,Float_t dTheta,Short_t add):_ecalR{ecalR},_dTheta{dTheta},_addSplits{add}{
    };
    
    virtual ~MaskRadPhotons();//=default;
    MaskRadPhotons(const MaskRadPhotons& other) = default; //Copy Constructor
    MaskRadPhotons(MaskRadPhotons&& other) = default; //Move Constructor
      
    MaskRadPhotons& operator=(const MaskRadPhotons& other)=default;
    MaskRadPhotons& operator=(MaskRadPhotons&& other)=default;
 
    void AssignVectors(EventParticles* ep) override;
    void PrintMask() const override;
    Bool_t ReReadEvent() override;
    
    void ChangeRun(FinalState* fs) override;

    void UseTopoInfo(TopologyManager& topoInfo, TString pidInfo, TString incInfo) override;

  private:

    void doCorrection(std::vector<chanser::BaseParticle*> radParts, bool neutrons);
    Float_t GetMeanSF(Float_t  Edep);

    
    //keep a link to EventParticles vector I will replace
    particles_ptrs* _originalGams{nullptr}; //!
    particles_ptrs* _originalNeutrons{nullptr}; //!
    particles_ptrs* _originalEls{nullptr}; //!
    particles_ptrs* _originalPos{nullptr}; //!


    Float_t _ecalR={0};
    Float_t _dTheta={0.1};

    // Photon Sampling Fraction Parameters
    Float_t _sfPa={0};
    Float_t _sfPb={0};
    Float_t _sfPc={0};
    Float_t _sfPd={0};
    
    TH1F _hR={"R","Distance to photon in PCAL",500,0,500};//!
    TH1F _hdTheta={"dT","dTheta",400,-30,10};//!
    TH1F _hdPhi={"dP","dPhi",180,-12,4};//!

    TH2F _hdThetaR={"dTR","dTheta vs distance to photon in PCAL",500,0,500,400,-30,10};//!
    TH2F _hdThetadPhi={"dTdP","dTheta vs dPhi",180,-12,4,400,-30,10};//!
    TH2F _hdPhiR={"dPR","dPhi vs distance to photon in PCAL",500,0,500,180,-12,4};//!
    TH1F _hP={"P","Photons Momentum",100,0,5};//!
    TH1F _hPx={"Px","Photons x Momentum component",100,-2,2};//!
    TH1F _hPy={"Py","Photons y Momentum component",100,-2,2};//!
    TH1F _hPz={"Pz","Photons z Momentum component",100,0,5};//!

    TH1F _hRN={"RN","Distance to photon in PCAL (neutrons)",500,0,500};//!
    TH1F _hdThetaN={"dTN","dTheta (neutrons)",400,-30,10};//!
    TH1F _hdPhiN={"dPN","dPhi (neutrons)",180,-12,4};//!

    TH2F _hdThetaRN={"dTRN","dTheta vs distance to photon in PCAL (neutrons)",500,0,500,400,-30,10};//!
    TH2F _hdThetadPhiN={"dTdPN","dTheta vs dPhi (neutrons)",180,-12,4,400,-30,10};//!
    TH2F _hdPhiRN={"dPRN","dPhi vs distance to photon in PCAL (neutrons)",500,0,500,180,-12,4};//!

    TH1F _hPN={"PN","mis-IDed photons (neutrons) reconstructed Momentum",100,0,5};//!
    TH1F _hPxN={"PxN","mis-IDed photons (neutrons) reconstructed x Momentum component",100,-2,2};//!
    TH1F _hPyN={"PyN","mis-IDed photons (neutrons) reconstructed y Momentum component",100,-2,2};//!
    TH1F _hPzN={"PzN","mis-IDed photons (neutrons) reconstructed z Momentum component",100,0,5};//!

    Short_t _addSplits={1}; //Combine clusters?

    Short_t _elID={11};
    Short_t _posID={-11};

    Short_t  _fieldSign={1};
    
    static Bool_t CheckForPCAL(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->cal(clas12::PCAL)->getEnergy()>0;
    }

    ClassDefOverride(chanser::MaskRadPhotons,1); //class MaskRadPhotons
    
  };



}
