//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "EventParticles.h"
#include "ParticleConfig.h"
#include "Topology.h"
#include "TopologyManager.h"
#include "ActionManager.h"
#include "ParticleCutsManager.h"
#include "ParticleIter.h"
#include "DataManager.h"
#include "BaseOutEvent.h"
#include "FiledTree.h"
#include "ntuple_writer.h"
#include "Archive.h"

#include <TString.h>
#include <memory>
#include <vector>

namespace chanser{

  using topo_funcs = std::map<TString,  VoidFuncs >;

  using std::vector;

  class FinalState : public TNamed{

    
  public :
  
  FinalState(TString chPid,TString incl):_topoMan(this,chPid,incl),_ownsActions{0}{
      _optPid=chPid;
      _optIncl=incl;
  
    }
      
    virtual ~FinalState(); //need to delete ActionManagers
      
    FinalState(const FinalState& other) = default; //Copy Constructor
    FinalState(FinalState&& other) = default; //Move Constructor
      
    FinalState& operator=(const FinalState& other)=default;
    FinalState& operator=(FinalState&& other)=default;

    void PostRead(){
      InitTopoMan();
      Define();
      SetTopologies();
      CreateFinalTree(_finalTreeFile.GetName());
      CreateFinalHipo(_finalHipoFile.GetName());
    }
      
    virtual void Init();

    virtual void Define(){};
      
    void AutoIter();
    virtual Bool_t CheckParticle(const BaseParticle* part) const{return kTRUE;}

        
    void SetEventParticles(EventParticles *eventp){_eventParts=eventp;}

    void SetTruthParticles(const truth_ptrs *tr){_truth=tr;}

    virtual void SetEventInfo(const BaseEventInfo* evi){};
    void SetRunInfo(const BaseRunInfo* rui){};
      
    void ProcessEvent();
      
    void WriteToFile(TString filename);
    void ShowParticles();
    void ShowTopologies(){_topoMan.Print();};

    BaseParticle* GetParticle(const TString name) const;
    Bool_t CheckForValidTopos(const std::vector<short> eventPids){
      return _topoMan.CheckForValidTopos(eventPids);
    }
    Bool_t NeedTopos(){return _topoMan.ValidTopos().size()==0;}
      
    void SetHasTruth(){_hasTruth=kTRUE;}
    void SetHasntTruth(){_hasTruth=kFALSE;}
 
    void AddTopology(const TString names){
      _usedTopos.push_back(names);
      AddTopology(names,_doToTopo[names]);
    }
    void SetTopologies();

    Long64_t _TotPerm=0;
    Long64_t _nEvents=0;


    std::vector<TString >  GetUsedTopos(){return _usedTopos;}

    void InitTopoMan(){_topoMan.SetChargePID(_optPid);_topoMan.SetInclusive(_optIncl);}

    const TopologyManager& TopoManager() const {return _topoMan;}

      
    void CreateFinalTree(TString fname);
    void CreateFinalHipo(TString filename);
      
    virtual void ConfigureOutTree(TTree* tree);
    virtual void ConfigureOutHipo(hipo::ntuple_writer* writer);

    ttree_ptr FinalTree()const noexcept{ return _finalTree.get()!=nullptr ? _finalTree->Tree() : nullptr;}
    virtual  BaseOutEvent* GetOutEvent() noexcept{return nullptr;}
       
    hipo::ntuple_writer* FinalHipo()const noexcept{return _finalHipo.get();}

    void AddOutEvent(BaseOutEvent* treeData){
      if(FinalTree())treeData->Branches(FinalTree());
      if(FinalHipo())treeData->Hipo(FinalHipo());
    }
    void SetWorkerName(TString name){_workerName=name;}
    TString& WorkerName(){return _workerName;}
 
    void RegisterPostTopoAction(ActionManager& tam){
      _postTopoAction.push_back(&tam);
    }
    void RegisterPostKinAction(ActionManager& tam){
      _postKinAction.push_back(&tam);
    }
    
    actionman_ptrs& getPostTopoActions(){return _postTopoAction;}
    actionman_ptrs& getPostKinActions(){return _postKinAction;}
      
    void EndAndWrite();
       
  protected :

  FinalState():_topoMan{this},_ownsActions{1}{};
      
    void FSProcess();
 
    void InitEvent(){_gotCorrectOne=0;  _nPerm=0;_currTopoID=-1;_rejectEvent=0;};
    void AddTopology(const TString names,const VoidFuncs funcE);

  
    void  ConfigureIters(Topology *tt);
      
    void AddParticle(TString name,BaseParticle* part,Bool_t AddToFinal,Int_t genID);
    void ConfigParent(BaseParticle* parent,BaseParticle* child);
    
    ParticleIter* CreateParticleIter(vector<BaseParticle*> *parts,Int_t Nsel);
    vector<ParticleConfig* > HowManyParticles(Int_t pdg);

    void InitTruth();
    
    void CheckCombitorial();
    const Topology *CurrentTopo()const noexcept {return _currTopo;}
      
    virtual void Kinematics(){};
    virtual void UserProcess(){
      if(_finalTree.get())_finalTree->Fill();
      if(_finalHipo.get())_finalHipo->fill();
    };
      
    const EventParticles* GetEventParticles(){return _eventParts;}

      
    Long64_t _counter=0;

    TopologyManager _topoMan;//!
    topo_funcs _doToTopo;//!
 
  private:
      
    EventParticles* _eventParts{nullptr};//!
    const truth_ptrs* _truth{nullptr}; //!
   
    filed_uptr _finalTree;//!
    TNamed _finalTreeFile;
    
    std::unique_ptr<hipo::ntuple_writer> _finalHipo;//!
    TNamed _finalHipoFile;

	
    Topology *_currTopo{nullptr};//!
    ParticleIter* _currIter{nullptr};//!
      
    std::vector< ParticleConfig  > _pconfigs;//!
      
    //Final Particles vector for saving in output tree
    std::vector<BaseParticle*> _final;//!

    static constexpr Short_t _MISSING_DET =-9999;

    std::vector< TString > _usedTopos;

    //action managers
    actionman_ptrs _postTopoAction;
    actionman_ptrs _postKinAction;

      
    TString _optPid;
    TString _optIncl;
    TString _workerName; //for PROOF worker ID
 
    Int_t _currTopoID=-1;
    Int_t _nPerm=0;
    Short_t _gotCorrectOne=0;
    Short_t _isGenerated=0;
    Short_t _isPermutating0=0;
    Short_t _rejectEvent=0;
    Short_t _hasTruth=0;
    Short_t _itersConfigured=0;
    Short_t _ownsActions=1;//!
      
    ClassDef(chanser::FinalState,1); //class FinalState
  };

 
}
