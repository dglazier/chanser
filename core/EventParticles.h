//////////////////////////////////////////////////////////////
///
///Class:		EventParticles
///Description:
///    Class responsible for sorting particles into type vectors
///

#pragma once

#include "BaseParticle.h"

#include <map>
#include <vector>
#include<bits/stdc++.h>

namespace chanzer{

  class FinalState;

    
  using particle_uptr = std::unique_ptr<BaseParticle>;
  using particle_uptrs = std::vector<particle_uptr>;
  using particles_ptrs = std::vector<BaseParticle*>;
    
  class EventParticles{
      
  public :
    EventParticles();
    virtual ~EventParticles()=default;
    EventParticles(const EventParticles& other) = default; //Copy Constructor
    EventParticles(EventParticles&& other) = default; //Move Constructor
      
    EventParticles& operator=(const EventParticles& other)=default;
    EventParticles& operator=(EventParticles&& other)=default;
 
    //PDG ID particles
    particles_ptrs _vecProtons{0};//!
    particles_ptrs _vecNeutrons{0};//!
    particles_ptrs _vecPiPs{0};//!
    particles_ptrs _vecPiMs{0};//!
    particles_ptrs _vecKPs{0};//!
    particles_ptrs _vecKMs{0};//!
    particles_ptrs _vecEls{0};//!
    particles_ptrs _vecPos{0};//!
    particles_ptrs _vecPi0s{0};//!
    particles_ptrs _vecGams{0};//!
    particles_ptrs _vecAntiProtons{0};//!
 
    //Charge ID particles
    particles_ptrs _vecPlus{0};//!
    particles_ptrs _vecMinus{0};//!
    particles_ptrs _vec0{0};//!
    particles_ptrs _vec45{0};//!

    Bool_t ReadEvent(const particles_ptrs &event);

    Int_t GetNElectrons(){return _vecEls.size();}
      
    particles_ptrs* GetParticleVector(Int_t pdg) const{
      return _mapPDGtoParticle.at(pdg);
    }

    void SetMaxParticles(int max){_maxPart=max;}
      
  private:
    //map pdg value to particle type vector
    std::map< Int_t , particles_ptrs* > _mapPDGtoParticle{};//!
    std::vector<std::vector<BaseParticle*> *> _chargetoParticle={&_vecMinus,&_vec0,&_vecPlus};//! 
   
    int _maxPart{INT_MAX};

    ClassDef(chanzer::EventParticles,1); //class EventParticles
  };

  //Allow cuts to be based on data particles
  //before read into particle vectors
  class CheckParticle{

    virtual Bool_t OK() {return kTRUE;}

  };

}
