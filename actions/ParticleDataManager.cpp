#include "ParticleDataManager.h"
#include "FinalState.h"

namespace chanser{
    
  void ParticleDataManager::PrintAction(){
    std::cout<<" ParticleDataManager::Print() "<<_particleData.size()<<std::endl;
    for( const auto& data : _pdgToData ){
      std::cout<<" for particle "<<data.first<<" will use data : "<<std::endl;
      data.second->Print();
    }
    if( _defData.get()){
      std::cout<<std::endl<<" for default   will use data : "<<std::endl;
      _defData->Print();
    }
  }

  ///////////////////////////////////////////////////////////////
  void ParticleDataManager::Configure( FinalState* fs){
 
    // std::cout<<"ParticleDataManager::Configure "<< fs->GetOutputDir()<<" out    "<< _outDir <<std::endl;
    auto topos=fs->TopoManager().ObserveTopos(); //const, can't change them
      
    _outDir.Prepend(fs->GetOutputDir());     
    MakeOutDir();//directory for saving files
    // std::cout<<"ParticleDataManager::Configure "<< _outDir<<std::endl;
    for(auto const& topo : topos){
      auto topo_parts=topo.GetParticles();

      auto outfile=_outDir+Form("/ParticleVariables_%d.root",topo.ID());
      //add to merge list in case use proof, us outDir as unique name
      fs->AddMergeList("PARTICLEDATAMANANGER",outfile); 
      
      auto pdata=ParticleData{GetName(),outfile,topo_parts.size()};
	
      //Loop over all particles in this topology and assign output data
      UInt_t ip=0;
       for(auto const& particle : topo_parts){
	 Int_t pdg = particle->PDG();
	//Add particle to output data
	if(_pdgToData.find(pdg)==_pdgToData.end())//if not use default
	  pdata.AddParticle(_defData.get(),particle,topo.GetPartName(ip++));
	else{ 
	  pdata.AddParticle(_pdgToData[pdg].get(),particle,topo.GetPartName(ip++));
	}  
      }
      //add branches from final state tree
      if(_addFinal){
	fs->OutEvent().ConfigureOutTree(pdata.GetTree());
      }
      //add core branches from final state tree
      fs->AddFinalOutput(pdata.GetTree());
      
      fs->GetOutTrees().push_back(pdata.GetTree());
      _particleData.push_back(std::move(pdata));

    }
  
  }
  void ParticleDataManager::End(){
      
    for( auto& out : _particleData )
      out.Save();
  }
    
}
