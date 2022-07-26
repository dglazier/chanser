#include "ParticleData.h"

namespace chanser{
  /////////////////////////////////////////////
  void ParticleData::Fill(){
    for(auto& out : _outputs){//loop over particles
      out->FillData();
     }
     _tree->Fill();
  }
   
}
