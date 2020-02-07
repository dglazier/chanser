#pragma once


#include "BaseOutEvent.h"


class TreeDataPi2 : public chanzer::BaseOutEvent{

 public:
  TreeDataPi2(){SetName("Pi2");}
  virtual ~TreeDataPi2()=default;
      
  //data member for tree branches below here
  Double_t MissMass=0;
  Double_t MissMass2=0;

  Double_t MesonMass=0;
  /* Double_t DppMass=0; */
  /* Double_t D0Mass=0; */
  
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
      
  ClassDefOverride(TreeDataPi2,1);
};
