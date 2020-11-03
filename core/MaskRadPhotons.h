//////////////////////////////////////////////////////////////
///
///Class:		MaskRadPhotons
///Description:
///    Class responsible for removing photons radiated from electrons
///

#pragma once

#include "MaskedEventParticles.h"
#include "CLAS12Particle.h"
#include "clas12defs.h" //from clas12root

#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>

namespace chanser{

   
  class MaskRadPhotons : public MaskedEventParticles{
      
  public :
    MaskRadPhotons()=default;

  MaskRadPhotons(Float_t ecalR,Float_t dTheta,Short_t add):_ecalR{ecalR},_dTheta{dTheta},_addSplits{add}{};
    
    virtual ~MaskRadPhotons();//=default;
    MaskRadPhotons(const MaskRadPhotons& other) = default; //Copy Constructor
    MaskRadPhotons(MaskRadPhotons&& other) = default; //Move Constructor
      
    MaskRadPhotons& operator=(const MaskRadPhotons& other)=default;
    MaskRadPhotons& operator=(MaskRadPhotons&& other)=default;
 
    void AssignVectors(EventParticles* ep) override;
    void PrintMask() const override;
    Bool_t ReReadEvent() override;
    
  private:
    
    //keep a link to EventParticles vector I will replace
    particles_ptrs* _originalGams{nullptr}; //!


    Float_t _ecalR={0};
    Float_t _dTheta={0.1};
    
    TH1F _hR={"R","Distance to photon in PCAL",500,0,500};//!
    TH1F _hdTheta={"dT","dTheta",400,-30,10};//!
    TH1F _hdPhi={"dP","dPhi",180,-12,4};//!

    TH2F _hdThetaR={"dT0","dTheta vs distance to photon in PCAL",500,0,500,400,-30,10};//!
    TH2F _hdThetadPhi={"dTdP","dTheta vs dPhi",180,-12,4,400,-30,10};//!
    TH2F _hdPhiR={"dPR","dPhi vs distance to photon in PCAL",500,0,500,180,-12,4};//!


    Short_t _addSplits={1}; //Combine clusters?
    
    static Bool_t CheckForPCAL(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->cal(clas12::PCAL)->getEnergy()>0;
    }

    ClassDefOverride(chanser::MaskRadPhotons,1); //class MaskRadPhotons
    
  };



}
