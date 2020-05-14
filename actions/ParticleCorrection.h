//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///

#pragma once

#include "BaseParticle.h"
#include "BaseCorrection.h"
#include <vector>

namespace chanser{

   
  class ParticleCorrection {

  public:
    ParticleCorrection()=default;
    virtual ~ParticleCorrection()=default;
 
    void AddParticle( BaseCorrection* cut,BaseParticle* particle){
      _corrs.push_back(cut);
      _particles.push_back(particle);
      _nParticles++;
    }

    void  Apply() const noexcept;
    UInt_t  NParticles() const noexcept{return _nParticles;}
     
  private:

    //These vectors should be in sync, 1 cut for 1 particle
    basecorrs _corrs; 
    std::vector<BaseParticle* > _particles;   
    UInt_t _nParticles{0};
    
    ClassDef(chanser::ParticleCorrection,1);
    
  };//class ParticleCorrection
  using applycorrect_ptr = ParticleCorrection*;
  using applycorrects =std::vector<applycorrect_ptr>;
  
}
