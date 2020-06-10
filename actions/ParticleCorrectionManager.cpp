#include "ParticleCorrectionManager.h"
#include "FinalState.h"

namespace chanser{
    
  void ParticleCorrectionManager::PrintAction(){
    std::cout<<" ParticleCorrectionManager::Print() "<<std::endl;
    for( const auto& cor : _pdgToCorr ){
      std::cout<<" for particle "<<cor.first<<" will use correction : "<<std::endl;
      cor.second->Print();
    }
    std::cout<<std::endl;
 
  }
  ///////////////////////////////////////////////////////////////
  void ParticleCorrectionManager::Configure(FinalState* fs){

    if(fs->HasTruth()&&(!_forSim) ) return;
    else _forSim=2; //only apply correction if valid for sim and analysing sim data
    
    auto topos=fs->TopoManager().ObserveTopos();

    //loop over topologies
      //loop over particles
      //find correction for that species from map
      //add corrections for that species
     for(auto const& topo : topos){
	
      auto topoCorrection=ParticleCorrection{};
	
      //Loop over all particles in this topology and assign a correction if exists
      auto topo_parts=topo.GetParticles();
      for(auto const& particle : topo_parts){
	  
	Int_t pdg = particle->PDG();
	  
	//check if correction assigned for particular particle species
	if(_pdgToCorr.find(pdg)!=_pdgToCorr.end())
	  topoCorrection.AddParticle(_pdgToCorr[pdg].get(),particle);
	//no default
	  
	    
      }
      _particleCorrs.push_back(topoCorrection);

    }
  }

}
