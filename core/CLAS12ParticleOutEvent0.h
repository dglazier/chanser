//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "CLAS12ParticleOutEvent.h"

namespace chanser{
   
  class CLAS12ParticleOutEvent0 : public CLAS12ParticleOutEvent {

  public :
      
    CLAS12ParticleOutEvent0()=default;
      
      
    void FillData() final;

    //Function required to set tree branches
    //virtual void Branches(ttree_ptr tree) =0 ;
    //Class()->GetListOfDataMembers() needs to be
    //called in actual class
    void Branches(TTree* tree) final{
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
    Float_t ToF=0;
    Float_t Path=0;
    Float_t HypTime=0;
    Float_t DeltaTime=0;
    Float_t DeltaTimeVer=0;
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

      
    ClassDefOverride(CLAS12ParticleOutEvent0,1);

  };

  ///////////////////////////////////////////////////////////////
  inline void CLAS12ParticleOutEvent0::FillData(){
    
    //move the c12p object to this particle for getting detector data
    auto c12p=_part->CLAS12();
    
    auto r2d=TMath::RadToDeg();
 
    P     =  _part->P4p()->P();
    Theta =  _part->P4p()->Theta()*r2d;
    Phi   =  _part->P4p()->Phi()*r2d;
    Vz    =  _part->Vertex().Z();
    Vt    =  c12p->par()->getVt();

    ToF  =  _part->Time();
    Path  =  _part->Path();
    HypTime  =  _part->HypTime();
    DeltaTime  =  _part->DeltaTime();
    DeltaTimeVer  =  _part->DeltaTimeVer();
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
