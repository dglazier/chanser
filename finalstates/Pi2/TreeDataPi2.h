#pragma once


#include "BaseOutEvent.h"

#pragma link C++ class username::TreeDataPi2;

namespace username{

  class TreeDataPi2 : public chanser::BaseOutEvent{

  public:
    TreeDataPi2(){SetName("Pi2");}
    ~TreeDataPi2() override =default;
      
    //data member for tree branches below here
    Double_t MissMass=0;
    Double_t MissMass2=0;

    //example of e- kinematics
    //you can remove these if not using
    Double_t W=0;
    Double_t Q2=0;
    Double_t Pol=0;
    Double_t t=0;
    Double_t tprime=0;
    Double_t Egamma=0;
    Double_t MissE=0;
    Double_t MissP=0;
    Double_t MissMass2Pi=0;
    Double_t MissMassPPip=0;
    Double_t MissMassPPim=0;

    Double_t MesonMass=0;
    Double_t DppMass=0;
    Double_t D0Mass=0;
 
    Double_t ProdCosThGJ=0;
    Double_t ProdPhiGJ=0;  
    Double_t MesonCosThGJ=0;
    Double_t MesonPhiGJ=0;
    
    Short_t MesonExTrig=0;

    ///////////////////////////////////////////////////////////
    //LEAVE THE FOLLOWING FUNCTIONS
    //Function required to set tree branches
    void Branches(TTree* tree) final{
      BaseOutEvent::Branches(tree,Class()->GetListOfDataMembers());
    }
    void Hipo(hipo::ntuple_writer* writer) final{
      BaseOutEvent::Hipo(writer,Class()->GetListOfDataMembers());
    }
      
    ClassDefOverride(TreeDataPi2,1);
  };
}
