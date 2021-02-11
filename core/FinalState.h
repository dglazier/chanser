//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "EventParticles.h"
#include "MaskedEventParticles.h"
#include "ParticleConfig.h"
#include "Topology.h"
#include "TopologyManager.h"
#include "ActionManager.h"
#include "OutEventManager.h"
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
  using tlist_uptr= std::unique_ptr<TList>;
  
  using std::vector;

  //enum class FSOutputType{NONE, ROOTTREE, HIPONTUPLE};
  
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

    void Print(Option_t* option = "")const override;
    
    void PostRead(){
      InitTopoMan();
      Define();
      SetTopologies();
    }
      
    virtual void Init(const TString& baseDir); //takes name of output dir

    void ChangeRun();//for run dependent stuff
    virtual void DerivedChangeRun() {}//for run dependent stuff in derived class

    
    virtual void Define(){};
      
    void AutoIter();
    virtual Bool_t CheckParticle(const BaseParticle* part) const{return kTRUE;}

        
    void SetEventParticles(EventParticles *eventp);
    
    Bool_t HasTruth()const noexcept{return _hasTruth;};

    void SetTruthParticles(const truth_ptrs *tr){_truth=tr;}
    const truth_ptrs& GetTruthParticles()const noexcept{return *_truth;}
    void SetTruthMatched(Short_t match) noexcept{_truthMatchedCombi=match;}

    
    virtual void SetEventInfo(const BaseEventInfo* evi){};
    virtual void SetRunInfo(const BaseRunInfo* rui){};
      
    void ProcessEvent();
      
    void WriteToFile(TString filename);
    void ShowParticles()const;
    void ShowTopologies()const{_topoMan.Print();};

    BaseParticle* GetParticle(const TString name) const;
    ParticleConfig GetParticleConfig(const TString name) const;
    
    Bool_t CheckForValidTopos(const std::vector<short> eventPids){
      if(_isGenerated) return kTRUE;
      return _topoMan.CheckForValidTopos(eventPids);
    }
    Bool_t NeedTopos(){return _topoMan.ValidTopos().size()==0;}
      
    void SetHasTruth(){_hasTruth=kTRUE;}
    void SetHasntTruth(){_hasTruth=kFALSE;}
    void SetGenerated(){_isGenerated=kTRUE;SetHasTruth();}

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

    const OutEventManager& OutEvent() const {return _outEvent;};

    void UseOutputRootTree(){_outEvent.UseOutputRootTree();}
    void UseOutputHipoNtuple(){_outEvent.UseOutputHipoNtuple();}
    
   
    ttree_ptr FinalTree()const noexcept{ return _outEvent.FinalTree();}
    virtual  BaseOutEvent* GetOutEvent() noexcept{return nullptr;}
       
    hipo::ntuple_writer* FinalHipo()const noexcept{return _outEvent.FinalHipo();}
    void AddFinalOutput(hipo::ntuple_writer* nt);
    void AddFinalOutput(TTree* tree);
    
    void SetWorkerName(TString name){_workerName=name;}
    const TString& WorkerName(){return _workerName;}
    
    void SetInputFileName(TString name){_inputConfigFile=name;_inputConfigFile.ReplaceAll(".root","");}
    const TString& InputFileName() const{return _inputConfigFile;}

       
    void RegisterPreTopoAction(ActionManager& tam){
      _preTopoAction.push_back(&tam);
    }
    void RegisterPostTopoAction(ActionManager& tam){
      _postTopoAction.push_back(&tam);
    }
    void RegisterPostKinAction(ActionManager& tam){
      _postKinAction.push_back(&tam);
    }
    
    actionman_ptrs& getPreTopoActions() {return _postTopoAction;}
    actionman_ptrs& getPostTopoActions() {return _postTopoAction;}
    actionman_ptrs& getPostKinActions(){return _postKinAction;}
      
    void EndAndWrite();
    virtual TString GetUSER(){return "";};
    const TString& GetOutputDir(){return _outputDir;}

    std::vector<TTree*>& GetOutTrees() {return _listOfOutTrees;}
    
    //get the lists to be merged and delete it
    //this function can only be called once
    std::vector<tlist_uptr > UniqueMergeLists() {return std::move(_mergeLists);}
    TString FinalDirectory(){return _finalDirectory;}

    void AddMergeList(TString name, TString filename);

    //sink function mask no longer valid after calling this outside of this
    void MaskParticles(MaskedEventParticles* mask){
      _maskedParticles.push_back(std::unique_ptr<MaskedEventParticles>{mask});
    }
  
    Short_t WasGoodEvent(){return _goodEvent;}
    
  protected :

  FinalState():_topoMan{this},_ownsActions{1}{};
      
    void FSProcess();
 
    Bool_t InitEvent();
    
    void AddTopology(const TString names,const VoidFuncs funcE);

    void RejectEvent(Short_t re=1){_rejectEvent=re;}
    
    ParticleIter*  InnerParticle(Int_t pid);
    void InnerSelect(ParticleIter* recursiter,Int_t pid);
    void  ConfigureIters(Topology *tt);
      
    void AddParticle(TString name,BaseParticle* part,Bool_t AddToFinal,Int_t genID);
    void ConfigParent(BaseParticle* parent,BaseParticle* child);
    
    ParticleIter* CreateParticleIter(vector<BaseParticle*> *parts,Int_t Nsel);
    vector<ParticleConfig* > HowManyParticles(Int_t pdg);

    void InitTruth();
    void UseTruth();
    void NotTruth();
    void TruthKinematics();
    
    virtual base_outevt_uptr TreeDataFactory(){return base_outevt_uptr{}; }
    
    void PrepareOutEvent();
    virtual void SetOutEvent(BaseOutEvent* out){};
    
    void CheckCombitorial();
    const Topology *CurrentTopo()const noexcept {return _currTopo;}
      
    virtual void Kinematics(){};
    
    virtual void UserProcess(){
      _outEvent.Fill();
    };
    
    void FSTruthProcess();
    
    const EventParticles* GetEventParticles(){return _eventParts;}

      
    Long64_t _counter=0;

    TopologyManager _topoMan;//!
    topo_funcs _doToTopo;//!
 
    OutEventManager _outEvent;

  private:
      
    EventParticles* _eventParts={nullptr};//!
    const truth_ptrs* _truth={nullptr}; //!
    // std::unique_ptr<MaskedEventParticles> _maskedParticles={nullptr};//want to save this and so have moved class version to 2
    std::vector<std::unique_ptr<MaskedEventParticles>> _maskedParticles;
 
   	
    Topology *_currTopo={nullptr};//!
    ParticleIter* _currIter={nullptr};//!
      
    std::vector< ParticleConfig  > _pconfigs;//!
      
    //Final Particles vector for saving in output tree
    std::vector<BaseParticle*> _final;//!

    static constexpr Short_t _MISSING_DET =-9999;

    std::vector< TString > _usedTopos;

    std::vector<TTree*> _listOfOutTrees;//!

    std::vector<std::unique_ptr<TList> > _mergeLists;//!
    
    //action managers
    actionman_ptrs _preTopoAction;
    actionman_ptrs _postTopoAction;
    actionman_ptrs _postKinAction;


 
    
    TString _optPid;
    TString _optIncl;
    TString _workerName; //for PROOF worker ID
    TString _inputConfigFile; //where this FS was loaded from
    TString _outputDir;
    TString _finalDirectory;

    //Do not stream these
    //in future can add datamembers here
    //but to allow backward compatability with
    //final state analyssis already saved as root file
    //streaming is removed
    Int_t _currTopoID=-1;//!
    Int_t _nPerm=0;//!
    Int_t _truthMatchedCombi=0;//!
    Short_t _gotCorrectCombi=0;//!
    Short_t _isGenerated=0;//!
    Short_t _isPermutating0=0;//!
    Short_t _rejectEvent=0;//!
    Short_t _hasTruth=0;//!
    Short_t _itersConfigured=0;//!
    Short_t _ownsActions=1;//!
    Short_t _goodEvent=0;//!
    
    ClassDefOverride(chanser::FinalState,1); //class FinalState
  };

 
}
