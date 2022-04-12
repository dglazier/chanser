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
#include "clas12defs.h" //from clas12root

#include <TH1F.h>
#include <TH2F.h>

namespace chanser{

  //inherit from public CLAS12Base when have dbs code
  class MaskSecondaryNeutrons : public MaskedEventParticles{
      
  public :
    MaskSecondaryNeutrons()=default;

  MaskSecondaryNeutrons(Short_t mask,Double_t betaCut):_maskSecondaries{mask},_betaCut{betaCut}{
    };
    
    virtual ~MaskSecondaryNeutrons();//=default;
    MaskSecondaryNeutrons(const MaskSecondaryNeutrons& other) = delete; //Copy Constructor
    MaskSecondaryNeutrons(MaskSecondaryNeutrons&& other) = default; //Move Constructor
      
    MaskSecondaryNeutrons& operator=(const MaskSecondaryNeutrons& other)=delete;
    MaskSecondaryNeutrons& operator=(MaskSecondaryNeutrons&& other)=default;
 
    void AssignVectors(EventParticles* ep) override;
    void PrintMask() const override;
    Bool_t ReReadEvent() override;

    
    TString ClassName() const override {return "MaskSecondaryNeutrons";} 

  private:

    void doCorrection(std::vector<chanser::BaseParticle*> fd0s);
    bool vectorContainsD(std::vector<Double_t> v, Double_t key);
    bool vectorContains(std::vector<Int_t> v, Int_t key);

    
    //keep a link to EventParticles vector I will replace
    particles_ptrs* _originalPlus{nullptr}; //!
    particles_ptrs* _originalMinus{nullptr}; //!
    particles_ptrs* _original0{nullptr}; //!

    Short_t _maskSecondaries={0}; //Remove Secondaries?
    Double_t _betaCut={2}; //Cut on beta to ID neutrons

    static Bool_t CheckForFD(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->getRegion()==clas12::FD;
    }
    static Bool_t CheckForNotFD(particle_ptr p) noexcept{
      return static_cast<CLAS12Particle*>(p)->CLAS12()->getRegion()!=clas12::FD;
    }

    /* Doesn't want to take non-static beta, but has to be static
       method to be used in filter
    static Bool_t CheckNotPassBetaCut(particle_ptr p) noexcept{
      auto c12p=static_cast<CLAS12Particle*>(p)->CLAS12();
      //The below seems a bit weird but we want to use
      //the beta value from DSTs to calculate momentum
      //then recalculate beta assuming we have a neutron
      Double_t betaFromDST=c12p->par()->getBeta();
      Double_t P=0.93957*betaFromDST/sqrt(1-betaFromDST*betaFromDST);
      Double_t beta=P/sqrt(P*P+0.93957*0.93957);
      Bool_t passCut=false;
      if(std::isnan(beta) || beta>_betaCut){
	passCut=true;
      }
      return passCut;
    }

     static Bool_t CheckPassBetaCut(particle_ptr p) noexcept{
      auto c12p=static_cast<CLAS12Particle*>(p)->CLAS12();
      //The below seems a bit weird but we want to use
      //the beta value from DSTs to calculate momentum
      //then recalculate beta assuming we have a neutron
      Double_t betaFromDST=c12p->par()->getBeta();
      Double_t P=0.93957*betaFromDST/sqrt(1-betaFromDST*betaFromDST);
      Double_t beta=P/sqrt(P*P+0.93957*0.93957);
      Bool_t passCut=false;
      if(!std::isnan(beta) && beta<=_betaCut){
	passCut=true;
      }
      return passCut;
      }*/

    ClassDefOverride(chanser::MaskSecondaryNeutrons,1); //class MaskSecondaryNeutrons
    
  };



}
