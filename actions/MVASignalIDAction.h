//////////////////////////////////////////////////////////////
///
///Class:		MVASignalIDAction
///Description:
///

#pragma once

#include "ActionManager.h"
#include "ParticleOutEvent.h"
#include "ResultInterface.h"
#include "BaseParticle.h"
#include <TDatabasePDG.h>
#include <TString.h>
#include <TSystem.h>

namespace chanser{
  
  using poutev_uptr=std::unique_ptr<ParticleOutEvent>;
  //using mvaresult_uptr= std::unique_ptr<ResultByRefLink>;
  
   class MVASignalIDAction : public ActionManager {

   public:

     //necesary configuation info for each topology
     struct TopoConfig{
       
       TString _topo;
       TString _method;
       TString _dirname;
     };
     
    MVASignalIDAction()=default;
    ///Must give an output directory for saving trees to
    MVASignalIDAction(const TString& dirname,const TString& meth,const TString& topo);
    
    virtual ~MVASignalIDAction()=default;
    MVASignalIDAction(const MVASignalIDAction& other) = default; //Copy Constructor
    MVASignalIDAction(MVASignalIDAction&& other) = default; //Move Constructor
      
    MVASignalIDAction& operator=(const MVASignalIDAction& other)=default;
    MVASignalIDAction& operator=(MVASignalIDAction&& other)=default;

    void PrintAction() override;
    Bool_t Execute(UInt_t ti)  override;
  
    void Configure(FinalState* fs) override;
    void SetParticleOut(TString type,ParticleOutEvent* out);
    void SetParticleOut(Int_t type,ParticleOutEvent* out);
    
    //set default (keep function name for backward compatability)
    void SetParticleOut(ParticleOutEvent* out){_defData.reset(out);}

    void PostConfigure(FinalState* fs) override;
    void Branches(FinalState* fs);
    void InitDataEvent() override{ _passCut=0; }

    void AddTopology(const TString& dirname,const TString& meth,const TString& topo);
    
   protected:
    void AddParticleData(const ParticleOutEvent* pdata,BaseParticle* particle,const TString& name);
    void SetVarLinks(const ParticleOutEvent* pdata);
    
   private :
    std::vector<mva::ResultByRefLink> _mvaResult;//!Create in Configure
    std::vector< poutev_uptr >_particleData;
    
    mva::nameToFloatRef_t _particleLinks;//!Create in Configure
    
    std::map<Int_t,poutev_uptr> _pdgToData;//
    std::unique_ptr<ParticleOutEvent> _defData; //default

    std::vector<TopoConfig> _configs{1}; //must have at least 1
    
    TString _dirName;
    TString _methodName;
    
    Float_t _passCut{0};//!calculated each event
    Short_t _forReal{0};

    ClassDefOverride(chanser::MVASignalIDAction,1);
  };

}
