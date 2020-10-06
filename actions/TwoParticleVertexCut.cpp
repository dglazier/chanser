#include "TwoParticleVertexCut.h"

namespace chanser{


  void TwoParticleVertexCut::SetC12(CLAS12FinalState* c12){

    _cutAgainst = static_cast<CLAS12Particle*>(c12->GetParticle(_partName));

    if(_cutAgainst ==nullptr){
      std::cerr<<" TwoParticleVertexCut::SetC12 particle does not exist = "<<_partName<<std::endl;
    }
  }

}
