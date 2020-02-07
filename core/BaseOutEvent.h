////////////////////////////////////////////////////////////////
///
///Class:               BaseOutEvent
///Description:
///            A base class to be used as a tree branch !
///            Inherit from this and add data members to the derived class
///            Then call dervived.Branches(tree);
///            To select data members to tree branch use
///            BaseOutEvent::SetBranches({"ThisOne","ThisTwo"});
///            to attach to tree

#pragma once

#include <TTree.h>
#include <TString.h>
#include <TObject.h>
#include <TList.h>
#include "ntuple_writer.h"


namespace chanzer{

  class BaseOutEvent  : public TNamed{

  public:
  BaseOutEvent(TString name):TNamed(name,name){}
    BaseOutEvent()=default;
    virtual ~BaseOutEvent()=default;

    virtual void FillData(){};
    
    void Branches(TTree* tree,TList* dmList);
    virtual void Branches(TTree* tree)=0;

    void Hipo(hipo::ntuple_writer* writer,TList* dmList);
    virtual void Hipo(hipo::ntuple_writer* writer)=0;
   
    void SetBranches(std::vector<TString > brs){_forBranch=brs;}
      
  private:
    TString _name="";//!
    std::vector<TString > _forBranch;//!

    // std::unique_ptr<hipo::bank> _hipoBank;//!
    std::vector<void* > _linksHipo;//!
       
    ClassDef(chanzer::BaseOutEvent,1);

  };//class BaseOutEvent
  using base_outevt_ptr = BaseOutEvent*;
  using base_outevts =std::vector<base_outevt_ptr>;
 
}
