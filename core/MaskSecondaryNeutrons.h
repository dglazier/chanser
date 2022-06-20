//////////////////////////////////////////////////////////////
///
///Class:		MaskSecondaryNeutrons
///Description:
///    Class responsible for neutral hits produced by other neutrons
///Author:
///     Richard Tyson

#pragma once

#include "TLorentzVector.h"
#include "MaskedEventParticles.h"
#include "CLAS12Particle.h"
#include "CLAS12Neutron.h"
#include "clas12defs.h" //from clas12root

#include <TH1F.h>
#include <TH2F.h>

namespace chanser{

  //inherit from public CLAS12Base when have dbs code
  class MaskSecondaryNeutrons : public MaskedEventParticles{
      
  public :
    MaskSecondaryNeutrons()=default;

  MaskSecondaryNeutrons(Short_t mask):_maskSecondaries{mask}{};
    
    virtual ~MaskSecondaryNeutrons();//=default;
    MaskSecondaryNeutrons(const MaskSecondaryNeutrons& other) = delete; //Copy Constructor
    MaskSecondaryNeutrons(MaskSecondaryNeutrons&& other) = default; //Move Constructor
      
    MaskSecondaryNeutrons& operator=(const MaskSecondaryNeutrons& other)=delete;
    MaskSecondaryNeutrons& operator=(MaskSecondaryNeutrons&& other)=default;
 
    void AssignVectors(EventParticles* ep) override;
    void PrintMask() const override;
    Bool_t ReReadEvent() override;

    void UseTopoInfo(TopologyManager& topoInfo, TString pidInfo, TString incInfo) override;
    TString ClassName() const override {return "MaskSecondaryNeutrons";} 

  private:

    void doCorrection(std::vector<chanser::BaseParticle*> fd0s);

    
    //keep a link to EventParticles vector I will replace
    particles_ptrs* _original0{nullptr}; //!
    particles_ptrs* _originalGams{nullptr}; //!
    particles_ptrs* _originalNeutrons{nullptr}; //!

    Short_t _nID={2112};
    Short_t _maskSecondaries={0}; //Remove Secondaries?

    static Bool_t CheckForFD(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->getRegion()==clas12::FD;
    }
    static Bool_t CheckForNotFD(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->getRegion()!=clas12::FD;
    }

    ClassDefOverride(chanser::MaskSecondaryNeutrons,1); //class MaskSecondaryNeutrons
    
  };



}
