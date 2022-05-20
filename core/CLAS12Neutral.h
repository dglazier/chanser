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

    static constexpr UInt_t invalidIndex=999999;
    
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

    bool isValid() const{return _indexInUse!=invalidIndex;}

    void ShiftTime(Float_t shift) override {    
      if(isValid()){
	for(auto candidate:_candidates){candidate->ShiftTime(shift);}
      }
      BaseParticle::ShiftTime(shift);
    }

  private :
    std::vector<CLAS12Particle*> _candidates; //DST neutrals combined into single neutral
    UInt_t _indexInUse{invalidIndex};
    
  };

  inline void chanser::CLAS12Neutral::MinorClear(){
    _candidates.clear();
    _indexInUse=invalidIndex;
     CLAS12Particle::MinorClear();
  }


  inline void chanser::CLAS12Neutral::CopyTransient(const BaseParticle* part){
    //clear to force clean vector
    _candidates.clear();
    _indexInUse=invalidIndex;

    /*Some CLAS12Neutrals may still be CLAS12Particles if they haven't
      gone through the MaskSecondaryNeutrons or aren't in FD*/
    auto c12n=dynamic_cast<const CLAS12Neutral*>(part);
    if(c12n!=nullptr){
      for(auto candidate:c12n->Candidates()){
	CLAS12Particle * newC12p = new CLAS12Particle();
	newC12p->CopyTransient(candidate);
	AddCandidate(newC12p);
      }
      UseCandidate(c12n->whichNeutral());
    }
    CLAS12Particle::CopyTransient(part);
  }

  
  
  
}
