//////////////////////////////////////////////////////////////
///
///Class:		DataManager
///Description:
///
#pragma once

#include "BaseParticle.h"
#include "BaseRunInfo.h"
#include "BaseEventInfo.h"
#include <memory>
#include <vector>

namespace chanser{

  using particle_uptr = std::unique_ptr<BaseParticle>;
  using particle_objs = std::vector<BaseParticle>;
  using particle_ptrs = std::vector<BaseParticle*>;
  using truth_objs = std::vector<TruthParticle>;
  using truth_ptrs = std::vector<TruthParticle*>;

  enum class DataType{Real,Sim,Lund};

  class FinalStateManager;
  
  class DataManager{

  public:
    DataManager()=default;
    virtual ~DataManager()=default;

    //  virtual Bool_t Init(const TString filename,const TString name="")=0;
    virtual Bool_t Init()=0;
    virtual Bool_t InitEvent()=0;
      
    virtual Bool_t ReadEvent(Long64_t entry=-1)=0;
    virtual void WriteEvent(Long64_t entry=-1){};//by default do nothing

    virtual const particle_ptrs& GetParticles() const = 0;
    virtual const std::vector<short> eventPids() = 0;
    virtual const truth_ptrs& GetTruth() const = 0;


    virtual const BaseRunInfo* GetRunInfo() const noexcept=0;
    virtual const BaseEventInfo* GetEventInfo() const noexcept=0;
      
    virtual Short_t DataType() const {return _dataType;}
      
    Long64_t NEventsRead()const {return _entry;}
      
    Bool_t IsSim() const noexcept{return (_dataType==static_cast<Short_t>(chanser::DataType::Sim))||(_dataType==static_cast<Short_t>(chanser::DataType::Lund));}
    Bool_t IsLund() const noexcept{return _dataType==static_cast<Short_t>(chanser::DataType::Lund);}

    virtual void FillRunInfo(){};
    virtual void FillEventInfo(){};

    void ConnectFinalStateManager(FinalStateManager *fsm){_fsmanager=fsm;}

    void Notify();
    
  protected :
        
    Long64_t _entry=0;
    Short_t _dataType=0;

      
  private:
      
    FinalStateManager* _fsmanager={nullptr};
  };
}

