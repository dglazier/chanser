//////////////////////////////////////////////////////////////
///
///Class:		CLAS12Neutral
///Description:
///              Class for neutral particles, helps with secondary neutrons mask
///Author:
///              Richard Tyson

#pragma once

#include "CLAS12Particle.h"
#include "region_particle.h"

namespace chanser{


  class CLAS12Neutral : public CLAS12Particle {


  public :
    CLAS12Neutral()=default;
    virtual ~CLAS12Neutral()=default;
  CLAS12Neutral(TString pdgname):CLAS12Particle(pdgname){}  	        //Constructor
    
    void MinorClear() override;
    void CopyTransient(const BaseParticle* part) override;

    const std::vector<CLAS12Particle*>& Candidates() const{return _candidates;}

    void AddCandidate(CLAS12Particle *p){
      _candidates.push_back(p);
    }

    void UseCandidate(UInt_t i){
      SetCLAS12Particle(_candidates[i]->CLAS12());
      _indexInUse=i;
    }

    UInt_t whichNeutral() const{
      return _indexInUse;
    }

  private :
    std::vector<CLAS12Particle*> _candidates; //DST neutrals combined into single neutral
    UInt_t _indexInUse{999};
    
  };

  inline void chanser::CLAS12Neutral::MinorClear(){
    _candidates.clear();
    _indexInUse=999;
     CLAS12Particle::MinorClear();
  }


  inline void chanser::CLAS12Neutral::CopyTransient(const BaseParticle* part){
    //clear to force clean vector
    _candidates.clear();
    _indexInUse=999;

    /*Some CLAS12Neutrals may still be CLAS12Particles if they haven't
      gone through the MaskSecondaryNeutrons or aren't in FD*/
    auto c12n=dynamic_cast<const CLAS12Neutral*>(part);
    if(c12n!=nullptr){
      _candidates=c12n->Candidates();
      _indexInUse=c12n->whichNeutral();
    }
    CLAS12Particle::CopyTransient(part);
  }

  
  
  
}
