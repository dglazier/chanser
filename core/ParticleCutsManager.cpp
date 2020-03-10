#include "ParticleCutsManager.h"
#include "FinalState.h"

namespace chanser{
    
  void ParticleCutsManager::PrintAction(){
    std::cout<<" ParticleCutsManager::Print() "<<std::endl;
    for( const auto& cut : _pdgToCut )
      std::cout<<" particle "<<cut.first<<" cut "<<cut.second->GetName()<<std::endl;

      
    std::cout<<" default  cut "<< _useableDefault->GetName() <<std::endl;
	
  }

  ///////////////////////////////////////////////////////////////
  void ParticleCutsManager::Configure(FinalState* fs){
 
    auto topos=fs->TopoManager().ObserveTopos();
      
    // std::cout<<" ParticleCutsManager::Configure topos "<<topos.size()<<std::endl;

    if(_useableDefault==nullptr) _useableDefault=&_defaultCut;
      
    for(auto const& topo : topos){
	
      //	auto pcuts=ParticleCutsPtr{new ParticleCuts()};//don't construct tree
      auto pcuts=ParticleCuts{};
	
      //Loop over all particles in this topology and assign a cut
      auto topo_parts=topo.GetParticles();
      //std::cout<<" ParticleCutsManager::Configure particles "<<topo_parts.size()<<std::endl;
      for(auto const& particle : topo_parts){
	  
	Int_t pdg = particle->PDG();
	  
	//check if cut assigned for particular particle species
	if(_pdgToCut.find(pdg)==_pdgToCut.end())
	  pcuts.AddParticle(_useableDefault,particle);
	else{ //if not use default
	  //  std::cout<<" "<<_pdgToCut[pdg]->ClassName()<<std::endl;
	  pcuts.AddParticle(_pdgToCut[pdg],particle);
	}
	  
	    
      }
      _particleCuts.push_back(std::move(pcuts));

    }
    Branches(fs);
    //std::cout<<" ParticleCutsManager::Configure "<<_particleCuts.size()<<std::endl;
  }
  ///////////////////////////////////////////////////////////////// 
  void ParticleCutsManager::Branches(const FinalState* fs){
    cout<<" ParticleCutsManager::Branches "<<fs->FinalTree()<<" "<<fs->FinalHipo()<<endl;
    if(fs->FinalTree()!=nullptr)
      fs->FinalTree()->Branch(GetName(),&_passCut,_name+"/I");
    if(fs->FinalHipo()){
      fs->FinalHipo()->bank(GetName(),"Pass/I");
      fs->FinalHipo()->linkItemFunc(GetName(),"Pass/I",&_passCut);
    }
  }
  ////////////////////////////////////////////////////////////////////////
}
