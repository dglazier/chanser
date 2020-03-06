//////////////////////////////////////////////////////////////
///
///Class:		DataManager
///Description:
///
#pragma once

#include "BaseParticle.h"
#include "BaseRunInfo.h"
#include "BaseEventInfo.h"
#include "CLAS12Particle.h"
#include <memory>
#include <vector>

namespace chanzer{

  using particle_uptr = std::unique_ptr<BaseParticle>;
  using particle_objs = std::vector<BaseParticle>;
  using particle_ptrs = std::vector<BaseParticle*>;
  using truth_objs = std::vector<TruthParticle>;
  using truth_ptrs = std::vector<TruthParticle*>;

  enum class DataType{Real,Sim,Lund};
    
  class DataManager{

  public:
    DataManager()=default;
    virtual ~DataManager()=default;

    //  virtual Bool_t Init(const TString filename,const TString name="")=0;
    virtual Bool_t Init()=0;
    virtual Bool_t InitEvent()=0;
      
    virtual Bool_t ReadEvent(Long64_t entry=-1)=0;

    virtual const particle_ptrs& GetParticles() const = 0;
    virtual const std::vector<short> eventPids() = 0;
    virtual const truth_ptrs& GetTruth() const = 0;


    virtual const BaseRunInfo* GetRunInfo() const noexcept=0;
    virtual const BaseEventInfo* GetEventInfo() const noexcept=0;
      
    virtual Short_t DataType() const {return _dataType;}
      
    Long64_t NEventsRead()const {return _entry;}
      
    Bool_t IsSim() const noexcept{return _dataType==static_cast<Short_t>(DataType::Sim);}

  protected :
        
    Long64_t _entry=0;
    Short_t _dataType=0;

      
  private:
      
       
  };
}

