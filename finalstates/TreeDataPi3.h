#pragma once


#include "BaseOutEvent.h"

#pragma link C++ class TreeDataPi3;

class TreeDataPi3 : public chanser::BaseOutEvent{

 public:
  TreeDataPi3(){SetName("Pi3");}
  virtual ~TreeDataPi3()=default;
      
  //data member for tree branches below here
  Double_t MissMass=0;
  Double_t MissMass2=0;

  Double_t MesonMass=0;
   
  Double_t W=0;
  Double_t Q2=0;
  Double_t Pol=0;
  Double_t Egamma=0;
  
  Double_t MesonCosTh=0;
  Double_t MesonPhi=0;
  
  //Function required to set tree branches
  void Branches(TTree* tree) final{
    BaseOutEvent::Branches(tree,Class()->GetListOfDataMembers());
  }
  void Hipo(hipo::ntuple_writer* writer) final{
    BaseOutEvent::Hipo(writer,Class()->GetListOfDataMembers());
  }
      
  ClassDefOverride(TreeDataPi3,1);
};
