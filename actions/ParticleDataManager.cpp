#include "ParticleDataManager.h"
#include "FinalState.h"

namespace chanser{
    
  void ParticleDataManager::PrintAction(){
    std::cout<<" ParticleDataManager::Print() "<<_particleData.size()<<std::endl;
    std::cout<<"     Using ParticleOut Data class "<<_outData->ClassName()<<std::endl<<std::endl;
  }

  ///////////////////////////////////////////////////////////////
  void ParticleDataManager::Configure( FinalState* fs){
 
    auto topos=fs->TopoManager().ObserveTopos(); //const, can't change them
      
    _outDir.Prepend(fs->GetOutputDir());     
    MakeOutDir();//directory for saving files
      
    for(auto const& topo : topos){
      auto topo_parts=topo.GetParticles();
   
      auto outfile=_outDir+Form("/ParticleVariables_%d.root",topo.ID());
      auto pdata=ParticleData{GetName(),outfile,topo_parts.size()};
	
      //Loop over all particles in this topology and assign output data
      UInt_t ip=0;
      for(auto const& particle : topo_parts){
	//Add particle to output data
	pdata.AddParticle(_outData.get(),particle,topo.GetPartName(ip++));
	  
      }
      //add branches from final state tree
      if(_addFinal){
	fs->OutEvent().ConfigureOutTree(pdata.GetTree());
      }
      fs->GetOutTrees().push_back(pdata.GetTree());
      _particleData.push_back(std::move(pdata));

    }
  
  }
  void ParticleDataManager::End(){
      
    for( auto& out : _particleData )
      out.Save();
  }
    
}
