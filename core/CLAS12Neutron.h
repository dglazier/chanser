//////////////////////////////////////////////////////////////
///
///Class:		CLAS12Neutron
///Description:
///              Class for neutral particles, helps with secondary neutrons mask
///Author:
///              Richard Tyson

#pragma once

#include "CLAS12Particle.h"
#include "region_particle.h"

namespace chanser{

  
  class CLAS12Neutron : public CLAS12Particle {


  public :
    CLAS12Neutron()=default;
    virtual ~CLAS12Neutron()=default;
  CLAS12Neutron(TString pdgname):CLAS12Particle(pdgname){}  	        //Constructor

    static constexpr UInt_t invalidIndex=999999;
    
    void MinorClear() override;
    void CopyTransient(const BaseParticle* part) override;

    const std::vector<CLAS12Particle*>& Candidates() const{return _candidates;}

    void AddCandidate(CLAS12Particle *p){
      _candidates.push_back(p);
    }

    void UseCandidate(UInt_t i){
      SetCLAS12Particle(_candidates[i]->CLAS12());
      float Px=_candidates[i]->P4().X();
      float Py=_candidates[i]->P4().Y();
      float Pz=_candidates[i]->P4().Z();
      float M=0.93957;
      SetXYZM(Px,Py,Pz,M);
      _indexInUse=i;
    }

    UInt_t whichNeutron() const{
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
    std::vector<CLAS12Particle> _candidatePool;
    UInt_t _indexInUse{invalidIndex};
    
  };

  inline void chanser::CLAS12Neutron::MinorClear(){
    _candidates.clear();
    _indexInUse=invalidIndex;
     CLAS12Particle::MinorClear();
  }


  inline void chanser::CLAS12Neutron::CopyTransient(const BaseParticle* part){
    //clear to force clean vector
    _candidates.clear();
    _indexInUse=invalidIndex;

    /*Some CLAS12Neutrons may still be CLAS12Particles if they haven't
      gone through the MaskSecondaryNeutrons or aren't in FD*/
    auto c12n=dynamic_cast<const CLAS12Neutron*>(part);
    if(c12n!=nullptr){
      _candidatePool.resize(c12n->Candidates().size());
      int i=0;
      //New candidates copied over to avoid double correcting
      for(auto candidate:c12n->Candidates()){
        _candidatePool[i].CopyTransient(candidate);
	AddCandidate(&_candidatePool[i]);
	i++;
      }
      UseCandidate(c12n->whichNeutron());
    }
    CLAS12Particle::CopyTransient(part);
  }

  
  
  
}
