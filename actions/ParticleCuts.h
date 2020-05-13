//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///

#pragma once

#include "BaseParticle.h"
#include "BaseCut.h"
#include <vector>

namespace chanser{

   
  class ParticleCuts {

  public:
    ParticleCuts()=default;
    virtual ~ParticleCuts()=default;
 
    void AddParticle( BaseCut* cut,BaseParticle* particle){
      _cuts.push_back(cut);
      _particles.push_back(particle);
      _nParticles++;
    }

    UInt_t  PassCuts() const noexcept;
    UInt_t  NParticles() const noexcept{return _nParticles;}
     
  private:

    //These vectors should be in sync, 1 cut for 1 particle
    basecuts _cuts; 
    std::vector<BaseParticle* > _particles;   
    UInt_t _nParticles{0};
    
    ClassDef(chanser::ParticleCuts,1);
    
  };//class ParticleCuts
  using partcut_ptr = ParticleCuts*;
  using partcuts =std::vector<partcut_ptr>;
  
}
