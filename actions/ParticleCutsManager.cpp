#include "ParticleCutsManager.h"
#include "FinalState.h"

namespace chanser{
    
  void ParticleCutsManager::PrintAction(){
    std::cout<<" ParticleCutsManager::Print() "<<GetName()<<std::endl;
    for( const auto& cut : _pdgToCut ){
      std::cout<<" for particle "<<cut.first<<" will use cut : "<<std::endl;
      cut.second->Print();
    }

      
    if(_useableDefault!=nullptr){
      std::cout<<" default  cut "<< _useableDefault->ClassName() <<std::endl;
      _useableDefault->Print();
    }
     std::cout<<std::endl;
  }

  ///////////////////////////////////////////////////////////////
  void ParticleCutsManager::Configure(FinalState* fs){
 
    auto topos=fs->TopoManager().ObserveTopos();
      
    for(auto const& topo : topos){
	
      auto pcuts=ParticleCuts{};
	
      //Loop over all particles in this topology and assign a cut
      auto topo_parts=topo.GetParticles();
      for(auto const& particle : topo_parts){
	  
	Int_t pdg = particle->PDG();
	  
	//check if cut assigned for particular particle species
	if(_pdgToCut.find(pdg)==_pdgToCut.end())
	  pcuts.AddParticle(_useableDefault.get(),particle);
	else{ //if not use default
	  pcuts.AddParticle(_pdgToCut[pdg].get(),particle);
	}
	  
	    
      }
      _particleCuts.push_back(pcuts);

    }
    Branches(fs);
   }
  ///////////////////////////////////////////////////////////////// 
  void ParticleCutsManager::Branches( FinalState* fs){
    auto trees=fs->GetOutTrees();
    //add cuts to all output tree (particle, final, ...)
    for(auto tree:trees){
      tree->Branch(GetName(),&_passCut,TString(GetName())+"/I");
    }
    if(fs->FinalHipo()){
      fs->FinalHipo()->bank(GetName(),"Pass/I");
      fs->FinalHipo()->linkItemFunc(GetName(),"Pass/I",&_passCut);
    }
  }
  ////////////////////////////////////////////////////////////////////////
}
