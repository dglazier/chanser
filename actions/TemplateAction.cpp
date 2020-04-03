#include "StartTimeAction.h"
#include "FinalState.h"

namespace chanser{
    
  void StartTimeAction::PrintAction(){
    std::cout<<" StartTimeAction::Print() "<<std::endl;
 	
  }
  ///////////////////////////////////////////////////////////////
  void StartTimeAction::Configure(FinalState* fs){
 
    auto topos=fs->TopoManager().ObserveTopos();
    
  }

}
