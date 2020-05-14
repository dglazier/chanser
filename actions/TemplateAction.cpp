#include "TemplateAction.h"
#include "FinalState.h"

namespace chanser{
    
  void TemplateAction::PrintAction(){
    std::cout<<" TemplateAction::Print() "<<std::endl;
 	
  }
  ///////////////////////////////////////////////////////////////
  void TemplateAction::Configure(FinalState* fs){
 
    auto topos=fs->TopoManager().ObserveTopos();
    
  }

}
