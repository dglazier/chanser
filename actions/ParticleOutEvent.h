//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "BaseOutEvent.h"

namespace chanser{

    
  class ParticleOutEvent : public BaseOutEvent {

  public :
      
    ParticleOutEvent()=default;

    virtual std::unique_ptr<ParticleOutEvent> clone() {return std::unique_ptr<ParticleOutEvent>(new ParticleOutEvent{});}
      
    //
    virtual void SetParticle(BaseParticle* particle){};
    

    //Function required to set tree branches
    //virtual void Branches(ttree_ptr tree) =0 ;
    //Class()->GetListOfDataMembers() needs to be
    //called in actual class
    void Branches(TTree* tree) override{
      BaseOutEvent::Branches(tree,Class()->GetListOfDataMembers());
    }
    void Hipo(hipo::ntuple_writer* writer) override{
      BaseOutEvent::Hipo(writer,Class()->GetListOfDataMembers());
    }
      
    /////////////////////////////////////////////////
    /// data member for tree branches below here
    /// Experiment developers add your own data members
    /// if you want them in the tree
    /// then assign them in  ParticleData::FillData()
  private :
      


    ClassDefOverride(ParticleOutEvent,1);

  };
  using partoutev_uptr = std::unique_ptr<ParticleOutEvent>;


  /////////////////////////////////////////////////
}
