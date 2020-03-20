//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "CLAS12ParticleOutEvent.h"
//#include "ParticleOutEvent.h"

namespace dglazier{
   
  class MyParticleOutEvent : public chanser::CLAS12ParticleOutEvent {

  public :
      
    MyParticleOutEvent()=default;
    chanser::partoutev_uptr clone() final{return std::unique_ptr<MyParticleOutEvent>(new MyParticleOutEvent{});}
      
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
     
    ClassDefOverride(MyParticleOutEvent,1);

  };

  ///////////////////////////////////////////////////////////////
  inline void MyParticleOutEvent::FillData(){
    
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
 
   
  }
  /////////////////////////////////////////////////
}
