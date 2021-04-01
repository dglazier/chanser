//////////////////////////////////////////////////////////////
///
///Class:		MaskCalorSplitOffs
///Description:
///    Class responsible for sorting particles into type vectors
///

#pragma once

#include "MaskedEventParticles.h"
#include "CLAS12Particle.h"
#include "clas12defs.h" //from clas12root

#include <TH1F.h>

namespace chanser{

  
  class MaskCalorSplitOffs : public MaskedEventParticles{
      
  public :
    MaskCalorSplitOffs()=default;

  MaskCalorSplitOffs(Float_t r0,Float_t rp,Float_t rm,Short_t add):_r0min{r0},_rpmin{rp},_rnmin{r0},_addSplits{add}{};
    
    virtual ~MaskCalorSplitOffs();//=default;
    MaskCalorSplitOffs(const MaskCalorSplitOffs& other) = default; //Copy Constructor
    MaskCalorSplitOffs(MaskCalorSplitOffs&& other) = default; //Move Constructor
      
    MaskCalorSplitOffs& operator=(const MaskCalorSplitOffs& other)=default;
    MaskCalorSplitOffs& operator=(MaskCalorSplitOffs&& other)=default;
 
    void AssignVectors(EventParticles* ep) override;
    void PrintMask() const override;
    Bool_t ReReadEvent() override;
    
    void UseTopoInfo(TopologyManager& topoInfo, TString pidInfo, TString incInfo) override;
    TString ClassName() const override {return "MaskCalorSplitOffs";} 
  private:
    
    //keep a link to EventParticles vector I will replace
    particles_ptrs* _originalGams{nullptr}; //!
    particles_ptrs* _originalNeuts{nullptr}; //!
    particles_ptrs* _original0s{nullptr}; //!


    Float_t _r0min={6};
    Float_t _rpmin={6};
    Float_t _rnmin={6};
    
    TH1F _hR0={"R0","Distance to neutral",180,0,90};//!
    TH1F _hRp={"Rp","Distance to +ve",180,0,90};//!
    TH1F _hRm={"Rm","Distance to -ve",180,0,90};//!


    Short_t _addSplits={0}; //Combine clusters?
    Short_t _gamID={22};
 
    static Bool_t CheckForPCAL(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->cal(clas12::PCAL)->getEnergy()>0;
    }
    static Bool_t CheckForNotPCAL(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->cal(clas12::PCAL)->getEnergy()==0;
    }
    static Bool_t CheckForFD(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->getRegion()==clas12::FD;
    }
    static Bool_t CheckForNotFD(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->getRegion()!=clas12::FD;
    }

    ClassDefOverride(chanser::MaskCalorSplitOffs,1); //class MaskCalorSplitOffs
    
  };



}
