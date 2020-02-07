#pragma once


#include "BaseOutEvent.h"


class TreeDatatemplateFS : public chanzer::BaseOutEvent{

 public:
  TreeDatatemplateFS(){SetName("templateFS");}
  virtual ~TreeDatatemplateFS()=default;
      
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
      
  ClassDefOverride(TreeDatatemplateFS,1);
};
