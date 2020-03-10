#include "ParticleCuts.h"

namespace chanser{
  /////////////////////////////////////////////
  UInt_t ParticleCuts::PassCuts() const noexcept{
      
    auto np=_particles.size();
    UInt_t npass=0;
    if(np==0) return kFALSE;
    //Loop over all cuts if fail 1 return false
    for(UInt_t ip=0;ip<np;++ip){
      if((_cuts[ip]->ParticleCut(_particles[ip]))) npass++;
    }
   
    //passed all return true	
    return npass;
  }

  //////////////////////////////////////////////
}


