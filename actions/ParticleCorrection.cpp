#include "ParticleCorrection.h"

namespace chanser{
  /////////////////////////////////////////////
  void ParticleCorrection::Apply() const noexcept{
      
    auto np=_particles.size();
    //Loop over all corrections if fail 1 return false
    for(UInt_t ip=0;ip<np;++ip){
      _corrs[ip]->ParticleCorrect(_particles[ip]);
    }
   
   }

  //////////////////////////////////////////////
}


