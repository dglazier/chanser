//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "CLAS12Particle.h"
#include "ParticleOutEvent.h"

namespace chanser{
   
  class CLAS12ParticleOutEvent : public ParticleOutEvent {

  public :
      
    CLAS12ParticleOutEvent()=default;
      
      
    void SetParticle(BaseParticle* particle) final {_part=static_cast<CLAS12Particle*>(particle);}; 

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
      
  protected :
    
    CLAS12Particle*  _part{nullptr};//!

  private :
         /////////////////////////////////////////////////
    /// data member for tree branches below here
    /// Experiment developers add your own data members
    /// if you want them in the tree
    /// then assign them in  ParticleData::FillData()



      
    ClassDefOverride(CLAS12ParticleOutEvent,1);

  };


  /////////////////////////////////////////////////
}
