//////////////////////////////////////////////////////////////
///
///Class:		ActionManager
///Description:
///
#pragma once

#include <Rtypes.h>
#include <TString.h>
#include <utility>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>

namespace chanser{

  class FinalState;
  
  class ActionManager{
  public:
    ///Must give an output directory for saving trees to
    ActionManager()=default;
  ActionManager(TString name):_name(std::move(name)){};
    virtual ~ActionManager()=default;
      
    virtual Bool_t Execute(UInt_t ti){return kTRUE;}
    virtual void End(){};
    virtual void  Configure(FinalState* fs){};

    virtual void PrintAction(){std::cout<<"ActionManager"<<std::endl;};
      
    const char* GetName(){return _name.Data();}

    TString _name;
    ClassDef(chanser::ActionManager,1);
  };// ActionManager
    
  using actionman_uptr = std::unique_ptr<ActionManager>;
  using actionman_ptr = ActionManager*;
  // using actionmans =std::vector<ActionManager>;
  using actionman_ptrs =std::vector<actionman_ptr>;
    
}
