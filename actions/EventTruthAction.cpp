#include "EventTruthAction.h"
#include "FinalState.h"
#include "VectorUtility.h"

namespace chanser{
    
  void EventTruthAction::PrintAction(){
    
    std::cout<<" EventTruthAction::Print() "<<std::endl;
    std::cout<<"     Going to truth match simulated data "<<std::endl<<std::endl;

  }

  ///////////////////////////////////////////////////////////////
  void EventTruthAction::Configure( FinalState* fs){

    if(fs->HasTruth()==kFALSE) return;
				 
    auto topos=fs->TopoManager().ObserveTopos(); //const, can't change them   
    //get truth particles to match to
    _truth=&fs->GetTruthParticles(); //copy the ptrs
    
    for(auto const& topo : topos){
      //get particles for each topology
      baseparticle_ptrs parts=topo.GetParticles(); //make a copy

      _particles.push_back(parts);

    }
    //use a Lambda to capture the finalstate class so we don't have to
    FinalStateTruthMatch =[fs](Short_t match){fs->SetTruthMatched(match);};
  }
 
  Bool_t EventTruthAction::CheckEventTruth(baseparticle_ptrs parts){
    Bool_t isTrue=kTRUE;
    for(auto& p: parts){
      isTrue*=CheckParticleTruth(p);
    }
    return isTrue;
  }
  Bool_t EventTruthAction::CheckParticleTruth(BaseParticle* p){
    //loop over truth vectors to see if this particle has its best match
    //first only use truth particle with same PDG code as p
    // cout<<" EventTruthAction::CheckParticleTruth "<<p <<" "<<_truth->size()<<endl;
    Int_t id=p->PDG();
    /*  auto samePids=ranges::filter(*_truth,[id](const TruthParticle* truth)
	{return truth->_pdgCode==id;});*/
    //get the nearest truth particle to the reconstructed vector
    //Note this may fail if two tracks are close together
    auto matched = std::min_element(_truth->begin(),_truth->end(),[p](const TruthParticle *p1,const TruthParticle* p2){
	////////////////////////////////////////
	return p->p3DistanceSq(p1->_p4.Vect()) <
	p->p3DistanceSq(p2->_p4.Vect());
      });
    ////////////////////////////////////////

    //  cout<<(*matched)->_pdgCode<<" "<<(*matched)->_p4.P()<<" "<<p->PDG()<<" "<<p->Truth()->_p4.P()<<endl;
    //is the nearest one, the particle's truth ?
    return *matched==p->Truth() ? kTRUE:kFALSE;
      
  }
}
