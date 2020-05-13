#include "StartTimeAction.h"
#include "FinalState.h"

namespace chanser{
    
  void StartTimeAction::PrintAction(){
    std::cout<<" StartTimeAction::Print() "<<std::endl;
    std::cout<<"      Using C12StartTime : "<<_stu->GetName()<<std::endl<<std::endl;
   
  }
  ///////////////////////////////////////////////////////////////
  void StartTimeAction::Configure(FinalState* fs){

    std::cout<<"StartTimeAction::Configure "<<_stu->Class_Name()<<" "<<_stu->GetName()<<std::endl;

    _stu->SetC12(static_cast<CLAS12FinalState* >(fs));

    auto topos=fs->TopoManager().ObserveTopos();
    Int_t itopo{0};
    for(auto const& topo : topos){
      _particles.push_back(vecparticles{});
      //Loop over all particles in this topology and assign a cut
      auto topo_parts=topo.GetParticles();
      for(auto const& particle : topo_parts){
	_particles[itopo].push_back(static_cast<CLAS12Particle*>(particle));
      }
      itopo++;
    }
  }

}
