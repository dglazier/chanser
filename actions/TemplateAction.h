//////////////////////////////////////////////////////////////
///
///Class:		StartTimeAction
///Description:
///

#pragma once

#include "ActionManager.h"

namespace chanser{
    
   class StartTimeAction : public ActionManager {

  public:
    StartTimeAction()=default;
    ///Must give an output directory for saving trees to
   StartTimeAction(TString name):ActionManager(name){};
    
    virtual ~StartTimeAction()=default;
    StartTimeAction(const StartTimeAction& other) = default; //Copy Constructor
    StartTimeAction(StartTimeAction&& other) = default; //Move Constructor
      
    StartTimeAction& operator=(const StartTimeAction& other)=default;
    StartTimeAction& operator=(StartTimeAction&& other)=default;

    void PrintAction() override;
    Bool_t Execute(UInt_t ti)  override{
      //check cut for topology number ti
      return kTRUE;
    }
      
  
    void Configure(FinalState* fs) override;
  
    
    ClassDefOverride(chanser::StartTimeAction,1);
  };

}
