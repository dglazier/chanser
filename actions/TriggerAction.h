//////////////////////////////////////////////////////////////
///
///Class:		TemplateAction
///Description:
///

#pragma once

#include "ActionManager.h"
#include "CLAS12Base.h"

namespace chanser{
    
  class TriggerAction : public ActionManager,public CLAS12Base {

  public:
    TriggerAction()=default;
    ///Must give an output directory for saving trees to
    TriggerAction(TString name,std::vector<string> trigNames):ActionManager(name), _names{trigNames} {
      _nTriggers=_names.size();
    };
    
    virtual ~TriggerAction()=default;
    TriggerAction(const TriggerAction& other) = default; //Copy Constructor
    TriggerAction(TriggerAction&& other) = default; //Move Constructor
      
    TriggerAction& operator=(const TriggerAction& other)=default;
    TriggerAction& operator=(TriggerAction&& other)=default;

    void PrintAction() override;
    Bool_t Execute(UInt_t ti)  override{
      //this is topology independent
      for(auto ib=0;ib<_nTriggers;++ib){
	_branchVals[ib]=GetEventInfo()->CLAS12()->checkTriggerBit(_trigBit[ib]);
      }
      return kTRUE;
    }
      
  
    void Configure(FinalState* fs) override;

    //CLAS12Base
    void ChangeRun() final;

  private:

    UInt_t _nTriggers=0; //
    std::vector<Int_t> _branchVals=std::vector<Int_t>(_nTriggers); //! Fired or not (number should = nTriggers)

    std::vector<Int_t> _trigBit=std::vector<Int_t>(_nTriggers);//! which trigger bit to check 
    std::vector<string> _names; //Names of triggers

    
    ClassDefOverride(chanser::TriggerAction,1);
  };

}
