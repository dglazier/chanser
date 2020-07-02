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
#include <limits>

namespace chanser{

  constexpr Int_t THIS_INT_MAX = 2147483647;
 
  class FinalState;

  static constexpr Short_t NegID()  {return -1E4;}
  static constexpr Short_t PosID()  {return 1E4;}
 
  using particle_uptr = std::unique_ptr<BaseParticle>;
  using particle_ptr = BaseParticle*;
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
    particles_ptrs _vecProtons;//!
    particles_ptrs _vecNeutrons;//!
    particles_ptrs _vecPiPs;//!
    particles_ptrs _vecPiMs;//!
    particles_ptrs _vecKPs;//!
    particles_ptrs _vecKMs;//!
    particles_ptrs _vecEls;//!
    particles_ptrs _vecPos;//!
    particles_ptrs _vecPi0s;//!
    particles_ptrs _vecGams;//!
    particles_ptrs _vecAntiProtons;//!
 
    //Charge ID particles
    particles_ptrs _vecPlus;//!
    particles_ptrs _vecMinus;//!
    particles_ptrs _vec0;//!
    particles_ptrs _vec45;//!

    virtual Bool_t ReadEvent(const particles_ptrs &event);

    Int_t GetNElectrons(){return _vecEls.size();}
      
    particles_ptrs* GetParticleVector(Short_t pdg) const{
      return _mapPDGtoParticle.at(pdg);
    }

    void SetMaxParticles(int max){_maxPart=max;}

    void Print() const;

    virtual void SetMapVector(Short_t pdg, particles_ptrs* vec){
      if(std::find(_pids.begin(), _pids.end(), pdg) == _pids.end())
	_pids.push_back(pdg);
      _mapPDGtoParticle[pdg]=vec;
    }

    std::vector<Short_t> _pids;

  private:
    //map pdg value to particle type vector
    std::map< Short_t , particles_ptrs* > _mapPDGtoParticle{};//!
    std::vector<std::vector<BaseParticle*> *> _chargetoParticle={&_vecMinus,&_vec0,&_vecPlus};//!
   
    int _maxPart{THIS_INT_MAX};

    ClassDef(chanser::EventParticles,1); //class EventParticles
  };

  //Allow cuts to be based on data particles
  //before read into particle vectors
  class CheckParticle{

    virtual Bool_t OK() {return kTRUE;}

  };

}
