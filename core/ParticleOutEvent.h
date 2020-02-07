//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "BaseParticle.h"
#include "BaseOutEvent.h"
#include <vector>

namespace chanzer{
   
  class ParticleOutEvent : public BaseOutEvent {

  public :
      
    ParticleOutEvent()=default;

      
    void SetParticle(BaseParticle* particle) {_part=particle;}; 

    void FillData() final;

    //Function required to set tree branches
    //virtual void Branches(ttree_ptr tree) =0 ;
    //Class()->GetListOfDataMembers() needs to be
    //called in actual class
    void Branches(TTree* tree) override{
      BaseOutEvent::Branches(tree,Class()->GetListOfDataMembers());
    }
    void Hipo(hipo::ntuple_writer* writer) final{
      BaseOutEvent::Hipo(writer,Class()->GetListOfDataMembers());
    }
      
    /////////////////////////////////////////////////
    /// data member for tree branches below here
    /// Experiment developers add your own data members
    /// if you want them in the tree
    /// then assign them in  ParticleData::FillData()
  private :
      
    Float_t P=0;
    Float_t Theta=0;
    Float_t Phi=0;
    Float_t Vz=0;
    Float_t Vt=0;
    Float_t Time=0;
    Float_t TimeVer=0;
    Float_t Edep=0;
    Float_t DeltaE=0;
    Float_t PreE=0;
    Float_t HTCC=0;
    Float_t LTCC=0;
    Float_t TrChi2=0;
    Float_t EBPid=0;
    Short_t Region=-1;
    Short_t Sector=-1;
    Short_t Status=0;


    BaseParticle*  _part{nullptr};
      
    ClassDefOverride(ParticleOutEvent,1);

  };

  ///////////////////////////////////////////////////////////////
  inline void ParticleOutEvent::FillData(){
    //move the c12p object to this particle for getting detector data
    auto c12p=_part->CLAS12();
    
    auto r2d=TMath::RadToDeg();
 
    P     =  _part->P4p()->P();
    Theta =  _part->P4p()->Theta()*r2d;
    Phi   =  _part->P4p()->Phi()*r2d;
    Vz    =  _part->Vertex().Z();
    Vt    =  c12p->par()->getVt();

    Time  =  _part->DeltaTime();
    TimeVer  =  _part->DeltaTimeVer();

    //All
    Region = c12p->getRegion();
    Edep = c12p->getDetEnergy();
    Sector = c12p->getSector();
    Status = c12p->par()->getStatus();
    EBPid=c12p->getPid();

    //All Charged
    DeltaE = c12p->getDeltaEnergy();

    //Region(FT,FD,CD) specific variables
    PreE=0;
    HTCC=0;
    LTCC=0;
    TrChi2=0;
    
    switch(c12p->getRegion()) {
      
    case clas12::FD :
      PreE = c12p->cal(clas12::PCAL)->getEnergy();
      // ECINM2u = c12p->cal(clas12::ECIN)->M2u();
      //ECOUTM2u = c12p->calorimetry(clas12::ECOUT)->M2u();
      
      HTCC = c12p->che(clas12::HTCC)->getNphe();
      LTCC = c12p->che(clas12::LTCC)->getNphe();
      
      TrChi2= c12p->trk(clas12::DC)->getChi2N();
      break;
    case clas12::FT :
      break;
    case clas12::CD :
      TrChi2= c12p->trk(clas12::CVT)->getChi2N();
      break;
    }
  }
  /////////////////////////////////////////////////
}
