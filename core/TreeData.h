////////////////////////////////////////////////////////////////
///
///Class:               TreeData
///Description:
///            A base class to be used as a tree branch !
///            Inherit from this and add data members to the derived class
///            Then call dervived.Branches(tree);
///            To select data members to tree branch use
///            TreeData::SetBranches({"ThisOne","ThisTwo"});
///            to attach to tree

#pragma once

#include <TTree.h>
#include <TString.h>
#include <TObject.h>
#include <TList.h>

//#include "bank.h"
//#include "dictionary.h"
#include "ntuple_writer.h"


namespace chanser{

  class TreeData  {

  public:
    //   TreeData(TString name){fName=name;}
    TreeData()=default;
    //TreeData(const TreeData&)=default;
    // TreeData(TreeData&&)=default;
    virtual ~TreeData()=default;
    
    void Branches(TTree* tree,TList* dmList);
    virtual void Branches(TTree* tree)=0;

    void Hipo(hipo::ntuple_writer* writer,TList* dmList);
    virtual void Hipo(hipo::ntuple_writer* writer)=0;
   
    TString GetName(){return _name;}
    void SetName(TString name){_name=name;}
    void SetBranches(std::vector<TString > brs){_forBranch=brs;}
      
  private:
    TString _name="";//!
    std::vector<TString > _forBranch;//!

    std::unique_ptr<hipo::bank> _hipoBank={nullptr};//!
    std::vector<void* > _linksHipo;//!
       
    ClassDef(TreeData,1);

  };//class TreeData

}
using chanser::TreeData;
