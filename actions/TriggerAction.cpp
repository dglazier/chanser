#include "TriggerAction.h"
#include "FinalState.h"

namespace chanser{
    
  void TriggerAction::PrintAction(){
    std::cout<<" TriggerAction::Print() "<<std::endl;
    for(auto i=0; i<_trigBit.size() ; ++i)
      std::cout<<_names[i]<<" "<<_trigBit[i]<<endl;
  }
  ///////////////////////////////////////////////////////////////
  void TriggerAction::Configure(FinalState* fs){
    //only streaming names from root file
    _nTriggers=_names.size();
    _branchVals.resize(_nTriggers);
    _trigBit.resize(_nTriggers);
    //PrintAction();
    
    //initalise CLAS12Base
    SetC12(dynamic_cast<CLAS12FinalState*>(fs));
    
    auto trees=fs->GetOutTrees();
    for(auto& tree:trees){//loop over trees and add trigger branches
      for(int ib =0;ib<_names.size();++ib){ //loop over requested triggers and make branch
	
	tree->Branch(_names[ib].data(),&_branchVals[ib],TString(_names[ib]) + "/I");
      }
    }
  }

  void TriggerAction::ChangeRun(){
    //trigger bit to trigger maps
    auto period = RunPeriodType();
    
    //Get maximum parameters
    //1 x ntriggers vector = 2D
    auto table=GetAnaDB().GetTable(period,"TRIGGER_BITS",{1,_nTriggers});
    if(table.IsValid()){

      auto tags = table.GetTags();
     

      //1 x ntriggers vector = 2D
      tablevals2D_t val(1,tablevals_t(_nTriggers));
      table.Fill(val);
      if(_nTriggers!=val[0].size())
	std::cerr<<"WARNING, TriggerAction::ChangeRun() "<<val.size()<<" number of triggers in .db file does not much predefined number of "<<_nTriggers<<" in TriggerAction.h "<<val[0].size()<<std::endl;
      
      for(int i = 0 ; i<_nTriggers ; ++i){
	if(tags[i]!=_names[i]){
	  std::cerr<<"WARNING, TriggerAction::ChangeRun() trigger name in .db file ="<<tags[i]<<", differs from that given in constructor = "<<_names[i]<<std::endl;
	}
	_trigBit[i]=val[0][i];
	//std::cout<<"TriggerAction::ChangeRun() " <<tags[i]<<" "<<_trigBit[i]<<std::endl;
      } 
     
    }//ifvalid

  }//end ChangeRun

}
