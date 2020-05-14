//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///    Interface to different types of particle correction

#pragma once

#include "BaseParticle.h"
#include "CLAS12Particle.h"
#include <TNamed.h>
#include <TMath.h>
#include <limits>

namespace chanser{
  

    
  class BaseCorrection: public TNamed {
      
  public :
      
    BaseCorrection()=default;
        
      
    virtual void ParticleCorrect(BaseParticle* part) const noexcept {}

    ClassDef(chanser::BaseCorrection,1);
 
  };
  using basecorr_ptr = BaseCorrection*;
  using basecorr =std::vector<basecorr_ptr>;
  using basecorrs =std::vector<basecorr_ptr>;
 
}
