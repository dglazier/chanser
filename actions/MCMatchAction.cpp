#include "MCMatchAction.h"
#include "FinalState.h"
#include "VectorUtility.h"

namespace chanser{
    
  void MCMatchAction::PrintAction(){
    
    std::cout<<" MCMatchAction::Print() "<<std::endl;
    std::cout<<"     Going to truth match simulated data using MC::Match banks"<<std::endl<<std::endl;

  }

  ///////////////////////////////////////////////////////////////
  void MCMatchAction::Configure( FinalState* fs){

    if(fs->HasTruth()==kFALSE) return;
				 
    auto topos=fs->TopoManager().ObserveTopos(); //const, can't change them   
    //get truth particles to match to
    _truth=&fs->GetTruthParticles(); //copy the ptrs
    
    for(auto const& topo : topos){
      //get particles for each topology
      baseparticle_ptrs parts=topo.GetParticles(); //make a copy
      //convert to CLAS12 particles
      c12particle_ptrs c12s;
      for(auto* part:parts){
	c12s.push_back(dynamic_cast<CLAS12Particle*>(part));
      }
      _particles.push_back(c12s);

    }
    //use a Lambda to capture the finalstate class so we don't have to
    FinalStateTruthMatch =[fs](Short_t match){fs->SetTruthMatched(match);};
  }
 
  Bool_t MCMatchAction::CheckEventTruth(c12particle_ptrs parts){
    //cout<<" MCMatchAction::CheckEventTruth "<<parts.size()<<endl;
    Bool_t isTrue=kTRUE;
    for(auto& p: parts){
      isTrue*=CheckParticleTruth(p);
    }
    //cout<<" MCMatchAction::CheckEventTruth true ? "<<isTrue<<endl;
   return isTrue;
  }
  Bool_t MCMatchAction::CheckParticleTruth(CLAS12Particle* p){
    //loop over truth vectors to see if this particle has its best match
    if(p->CLAS12()->mc()->isMatched()){
      //if particle is matched check its matched particle
      //has same index as the corresponding truth particle
      if( p->Truth()->_index == p->CLAS12()->mc()->getIndex())
	return kTRUE;
    }
    return kFALSE;

      
  }
}
