//////////////////////////////////////////////////////////////
///
///Class:		TemplateAction
///Description:
///

#pragma once

#include "ActionManager.h"

namespace chanser{
    
   class TemplateAction : public ActionManager {

  public:
    TemplateAction()=default;
    ///Must give an output directory for saving trees to
   TemplateAction(TString name):ActionManager(name){};
    
    virtual ~TemplateAction()=default;
    TemplateAction(const TemplateAction& other) = default; //Copy Constructor
    TemplateAction(TemplateAction&& other) = default; //Move Constructor
      
    TemplateAction& operator=(const TemplateAction& other)=default;
    TemplateAction& operator=(TemplateAction&& other)=default;

    void PrintAction() override;
    Bool_t Execute(UInt_t ti)  override{
      //check cut for topology number ti
      return kTRUE;
    }
      
  
    void Configure(FinalState* fs) override;
  
    
    ClassDefOverride(chanser::TemplateAction,1);
  };

}
