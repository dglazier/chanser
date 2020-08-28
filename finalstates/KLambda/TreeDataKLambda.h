#pragma once


#include "BaseOutEvent.h"

#pragma link C++ class dglazier::TreeDataKLambda;

namespace dglazier{

  class TreeDataKLambda : public chanser::BaseOutEvent{

  public:
    TreeDataKLambda(){SetName("KLambda");}
    ~TreeDataKLambda() final =default;
      
    //data member for tree branches below here
    Double_t MissMass=0;
    Double_t MissMass2=0;
    Double_t MissP=0;
    Double_t Massppim=0;
    Double_t MissMasseKp=0;
    Short_t Cut=0;
    //example of e- kinematics
    //you can remove these if not using
    //Double_t W=0;
    // Double_t Q2=0;
    //Double_t Pol=0;
    //Double_t Egamma=0;
  


    ///////////////////////////////////////////////////////////
    //LEAVE THE FOLLOWING FUNCTIONS
    //Function required to set tree branches
    void Branches(TTree* tree) final{
      BaseOutEvent::Branches(tree,Class()->GetListOfDataMembers());
    }
    void Hipo(hipo::ntuple_writer* writer) final{
      BaseOutEvent::Hipo(writer,Class()->GetListOfDataMembers());
    }
      
    ClassDefOverride(TreeDataKLambda,1);
  };
}
