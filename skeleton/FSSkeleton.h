//////////////////////////////////////////////////////////////
///
///Class:		FSSkeleton
///Description:
///           Classes for producing skeleton final state code
#pragma once

#include "Skeleton.h"
#include <TString.h>
#include <TObjArray.h>

namespace chanser{
  
  class FSSkeleton : public Skeleton  {
    
    
  public :
    
    FSSkeleton()=default;
    virtual ~FSSkeleton()=default;
    
  public :
    
    void MakeCode() override;

    void SetFinalState(TString proj){_finalName=proj;}
    void SetFinalStateTopo(TString topos){_finalTopo=topos;_finalTopo.ReplaceAll(" ","");};
    void SetFinalStateParts(TString finals){_finalParts=finals;_finalParts.ReplaceAll(" ","");};
    void SetFinalStateParents(TString finals){_finalParents=finals;_finalParents.ReplaceAll(" ","");};


  protected:

    //For derived classes to add additional functionality
    virtual void DeclareExtras(){};
    virtual void DefineExtras(){};

    //Code writing functions
    virtual void DeclareDetected();    
    virtual void DeclareParents();    
    virtual void DefineDetected();    
    virtual void DefineParents();    
    virtual void DefineTopologies();    
    virtual void CreateRunMacros();    
    virtual void CreateTreeData();    
    virtual void CreateTopoFuncs();    
  
 
    TString _userName;
    TString _finalName;
    TString _finalTopo;
    TString _finalParts;
    TString _finalParents;
    TString _strSkel;
    TObjArray* _topos=nullptr;
    TObjArray* _finals=nullptr;
    TObjArray* _parents=nullptr;

 };
}
