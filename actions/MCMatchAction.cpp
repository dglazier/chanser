#include "MCMatchAction.h"
#include "FinalState.h"
#include "VectorUtility.h"

namespace chanser{
    
  void MCMatchAction::PrintAction(){
    
    std::cout<<" MCMatchAction::Print() "<<std::endl;
    std::cout<<"     Going to truth match simulated data "<<std::endl<<std::endl;

  }

  ///////////////////////////////////////////////////////////////
  void MCMatchAction::Configure( FinalState* fs){

    if(fs->HasTruth()==kFALSE) return;
				 
    auto topos=fs->TopoManager().ObserveTopos(); //const, can't change them   
    //get truth particles to match to
    // _truth=&fs->GetTruthParticles(); //copy the ptrs

   
    for(auto const& topo : topos){
      //get particles for each topology
      auto parts=topo.GetParticles(); //make a copy

      //actually want CLAS12Particle
       clas12particle_ptrs c12particles;
      //loop over parts and find generated ID number
      Short_t ip=0;
      std::vector<Int_t> topoGenIDs;//genIDs vector in synch with particles
      
      for(auto const& particle : parts){
	//convert to CLAS12Particles
	c12particles.push_back(dynamic_cast<CLAS12Particle*>(particle));
	//Int_t pdg = particle->PDG();
	//Add particle to output data
	topoGenIDs.push_back(topo.GetPartConfig(ip).GenID());
	++ip;
      }
      _particles.push_back(c12particles);
      _genIDs.push_back(topoGenIDs);
 
    }
    //use a Lambda to capture the finalstate class so we don't have to
    FinalStateTruthMatch =[fs](Short_t match){fs->SetTruthMatched(match);};
  }
 
  Bool_t MCMatchAction::CheckEventTruth(const clas12particle_ptrs& parts,std::vector<Int_t> genIDs){
    Bool_t isTrue=kTRUE;
    UInt_t igen=0;
    for(auto& p: parts){
      //is the rec particle truthmatched * (is it the correct one ?)
      //is it the correct one => is it in the same position in the LUND file
      //as that given for this final state particle
      isTrue*=p->CLAS12()->mc()->isMatched()*
	(p->CLAS12()->mc()->getMatch()->getQuality()>0.9)
	*(p->CLAS12()->mc()->getEntry()==genIDs[igen]);

      ++igen;
   }
    return isTrue;
  }
 
}
