#pragma once


#include "BaseOutEvent.h"

#pragma link C++ class someone::TreeDataPi4;

namespace someone{

  class TreeDataPi4 : public chanser::BaseOutEvent{

  public:
    TreeDataPi4(){SetName("Pi4");}
    ~TreeDataPi4() final =default;
      
    //data member for tree branches below here
    Double_t MissMass=0;
    Double_t MissMass2=0;

    //example of e- kinematics
    //you can remove these if not using
    Double_t W=0;
    Double_t Q2=0;
    Double_t Pol=0;
    Double_t Egamma=0;
  

    //Meson stuff
    Double_t MesonMass=0;
    Double_t MesonCosThGJ=0;
    Double_t MesonPhiGJ=0;
    ///////////////////////////////////////////////////////////
    //LEAVE THE FOLLOWING FUNCTIONS
    //Function required to set tree branches
    void Branches(TTree* tree) final{
      BaseOutEvent::Branches(tree,Class()->GetListOfDataMembers());
    }
    void Hipo(hipo::ntuple_writer* writer) final{
      BaseOutEvent::Hipo(writer,Class()->GetListOfDataMembers());
    }
      
    ClassDefOverride(TreeDataPi4,1);
  };
}
