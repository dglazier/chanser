#include "ParticleConfig.h"


namespace chanzer{
  //////////////////////////////////////////////////////////////////
  ///return all children with this pdg code
  std::vector<BaseParticle* >  ParticleConfig::Children(Int_t pdg){
    std::vector<BaseParticle*> children;
    for(auto& child : _children)
      if(child->PDG()==pdg)
	children.push_back(child);
    return children;
  }

}
