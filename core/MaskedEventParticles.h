//////////////////////////////////////////////////////////////
///
///Class:		MaskedEventParticles
///Description:
///    Class responsible for sorting particles into type vectors
///

#pragma once

#include "EventParticles.h"
#include "CLAS12Particle.h"
#include "CLAS12Neutral.h"

#include <TFile.h>

namespace chanser{

  class TopologyManager;
  
  class MaskedEventParticles : public EventParticles{
      
  public :
    MaskedEventParticles()=default;
    virtual ~MaskedEventParticles()=default;
    MaskedEventParticles(const MaskedEventParticles& other) = delete; //Copy Constructor
    MaskedEventParticles(MaskedEventParticles&& other) = default; //Move Constructor
      
    MaskedEventParticles& operator=(const MaskedEventParticles& other)=delete;
    MaskedEventParticles& operator=(MaskedEventParticles&& other)=default;
 
    virtual TString ClassName() const{return "MaskedEventParticles";}

    virtual void AssignVectors(EventParticles* ep);
    virtual void PrintMask() const;
    virtual Bool_t ReReadEvent(){
      _nFromPool=0; //local particle object counter
      return kTRUE;
    };
    virtual void SetPidVectors();
    void  AddPids(const Short_t pid,const particles_ptrs* vec);
    void  PidCounter();
    
    const std::vector<short>& Pids()const noexcept{return _pidCounts;}

    void ReadyFile(TString outDir);

    virtual void UseTopoInfo(TopologyManager& topoInfo, TString pidInfo, TString incInfo){};

    virtual void ChangeRun(FinalState* fs) {};

  protected:
    
    particle_ptr NextParticle(Short_t pid,UInt_t& entry);
    
    CLAS12Particle* NextFromPool(){
      while(_particlePool.size()==_nFromPool){
	_particlePool.push_back(std::unique_ptr<CLAS12Particle>{new CLAS12Particle()});
      }
      
      auto next=_particlePool.at(_nFromPool).get();
      ++_nFromPool;
      next->Clear();
      return next;
    }
    
    CLAS12Neutral* NextNeutralFromPool(){
      while(_neutralParticlePool.size()==_nbNFromPool){
	_neutralParticlePool.push_back(std::unique_ptr<CLAS12Neutral>{new CLAS12Neutral()});
      }
      
      auto next=_neutralParticlePool.at(_nbNFromPool).get();
      ++_nbNFromPool;
      next->Clear();
      return next;
    }

    CLAS12Particle* ReplaceParticlePtr(Short_t pdg,CLAS12Particle* p0,CLAS12Particle* p1){
      //Replace with a p0 with a copy of p0 
      auto vec=GetParticleVector(pdg);
      auto it = std::find(vec->begin(), vec->end(), p0 );
      p1->CopyParticle(p0); //including pdg and charge
      *it=p1;
      return p1;
    }

    //Varition of ReplaceParticlePtr, which does not copy
    void ReplaceOnlyParticlePtr(Short_t pdg,CLAS12Particle* p0,CLAS12Particle* p1){
      //Replace with a p0 with a copy of p0 
      auto vec=GetParticleVector(pdg);
      auto it = std::find(vec->begin(), vec->end(), p0 );

      *it=p1;
      return;
    }
 
    void Write(TObject& obj );
    
  private:
    std::vector<std::unique_ptr<CLAS12Particle>> _particlePool; //! pool of particle objects can use for each event
    std::vector<std::unique_ptr<CLAS12Neutral>> _neutralParticlePool; //! same but for neutral particles

    std::vector<particles_ptrs*> _pidParticles;//!
    std::vector<Short_t> _pidCounts;//!


    std::unique_ptr<TFile> _outFile={nullptr};//!

    
    UInt_t _nFromPool={0};
    UInt_t _nbNFromPool={0};

    ClassDef(chanser::MaskedEventParticles,1); //class MaskedEventParticles
    
  };



}
