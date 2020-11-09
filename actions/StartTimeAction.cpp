#include "StartTimeAction.h"
#include "FinalState.h"

namespace chanser{
    
  void StartTimeAction::PrintAction(){
    std::cout<<" StartTimeAction::Print() "<<std::endl;
    std::cout<<"      Using C12StartTime : "<<_stu->GetName()<<std::endl<<std::endl;
   
  }
  ///////////////////////////////////////////////////////////////
  void StartTimeAction::Configure(FinalState* fs){

 
    _stu->SetC12fs(static_cast<CLAS12FinalState* >(fs));

    auto clas12base = dynamic_cast<CLAS12Base*>(_stu.get());
    if(clas12base!=nullptr){
       clas12base->SetC12(dynamic_cast<CLAS12FinalState*>(fs));
    }
    
    auto topos=fs->TopoManager().ObserveTopos();
    Int_t itopo{0};
    for(auto const& topo : topos){
      _particles.push_back(vecparticles{});
      //Loop over all particles in this topology to correct start time for
      auto topo_parts=topo.GetParticles();
      for(auto const& particle : topo_parts){
	_particles[itopo].push_back(static_cast<CLAS12Particle*>(particle));
      }
      itopo++;
    }
  }

}
