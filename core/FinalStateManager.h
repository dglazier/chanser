//////////////////////////////////////////////////////////////
///
///Class:		FinalStateManager
///Description:
///
#pragma once

#include "FinalState.h"
#include "DataManager.h"
#include <TString.h>
#include <vector>

namespace chanser{

  //  class Topology;
  //   class ParticleConfig;
  //class DataManager;
    
  //Typedef for pointer to function
  //used to link a derived class function with using its data members
  //To a function call in AddTopology and link to a call in Topology 
  // using VoidFuncs= std::function<void()>;

  using finalstate_uptr = std::unique_ptr<FinalState>;
  using finalstate_uptrs = std::vector<finalstate_uptr>;
  using finalstate_ptrs = std::vector<FinalState*>;
    
    
  class FinalStateManager{

  public :

    FinalStateManager()=default;
    virtual ~FinalStateManager()=default;


    Bool_t  LoadFinalState(TString fsname,TString filename,TString worker=""); //Saved in root file
    Bool_t  LoadFinalState(finalstate_uptr fs);  //unique_ptr to final state
       
    void LoadData(DataManager* data){_data=data;}

    void Init();
    void Notify();
    void ChangeRun();
      
    void   ProcessEvent();
    void   ProcessAll(Long64_t Nmax=9223372036854775807);

    void EndAndWrite();
      
    finalstate_ptrs& GetFinalStates(){return _rawFinalStates;};
   
    EventParticles& GetEventParticles(){return _eventParts;}

    const TString& BaseOutDir(){return _baseOutDir;};
    void SetBaseOutDir(const TString& name){_baseOutDir=name;}
    void MakeBaseOutputDir();
    
  private :
      

    DataManager* _data={nullptr}; //!
    EventParticles _eventParts;//!
   
    finalstate_uptrs _finalStates;//!
    finalstate_ptrs _rawFinalStates;//! for sharing
      
    bool _changeRun={true};//! always new run to start
    
    TString _baseOutDir;
    
    ClassDef(chanser::FinalStateManager,2); //class EventParticles
      
  };
}
