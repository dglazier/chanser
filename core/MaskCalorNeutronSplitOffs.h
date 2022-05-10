//////////////////////////////////////////////////////////////
///
///Class:		MaskCalorNeutronSplitOffs
///Description:
///    Class responsible for sorting particles into type vectors
///

#pragma once

#include "MaskedEventParticles.h"
#include "CLAS12Particle.h"
#include "clas12defs.h" //from clas12root

#include <TH1F.h>

namespace chanser{

  
  class MaskCalorNeutronSplitOffs : public MaskedEventParticles{
      
  public :
    MaskCalorNeutronSplitOffs()=default;

  MaskCalorNeutronSplitOffs(Float_t r0,Float_t rp,Float_t rm):_r0min{r0},_rpmin{rp},_rnmin{rm}{};
    
    virtual ~MaskCalorNeutronSplitOffs();//=default;
    MaskCalorNeutronSplitOffs(const MaskCalorNeutronSplitOffs& other) = delete; //Copy Constructor
    MaskCalorNeutronSplitOffs(MaskCalorNeutronSplitOffs&& other) = default; //Move Constructor
      
    MaskCalorNeutronSplitOffs& operator=(const MaskCalorNeutronSplitOffs& other)=delete;
    MaskCalorNeutronSplitOffs& operator=(MaskCalorNeutronSplitOffs&& other)=default;
 
    void AssignVectors(EventParticles* ep) override;
    void PrintMask() const override;
    Bool_t ReReadEvent() override;
    
    void UseTopoInfo(TopologyManager& topoInfo, TString pidInfo, TString incInfo) override;
    TString ClassName() const override {return "MaskCalorNeutronSplitOffs";} 
  
  private:
    
    //keep a link to EventParticles vector I will replace
    particles_ptrs* _originalNeutrons{nullptr}; //!


    Float_t _r0min={50};
    Float_t _rpmin={50};
    Float_t _rnmin={50};
    
    TH1F _hR0={"R0","Distance to neutral",1000,0,500};//!
    TH1F _hRp={"Rp","Distance to +ve",1000,0,500};//!
    TH1F _hRm={"Rm","Distance to -ve",1000,0,500};//!


    Short_t _neutID={22};
 
    static Bool_t CheckForNotPCAL(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->cal(clas12::PCAL)->getEnergy()==0;
    }

    ClassDefOverride(chanser::MaskCalorNeutronSplitOffs,1); //class MaskCalorNeutronSplitOffs
    
  };



}
