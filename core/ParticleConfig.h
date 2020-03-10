//////////////////////////////////////////////////////////////
///
///Class:		ParticleConfig
///Description:
///
#pragma once

#include "BaseParticle.h"
#include <vector>


namespace chanser{
    
  class ParticleConfig{

  public:
     
    ParticleConfig()=default;
    ParticleConfig(BaseParticle* part,Int_t genID){
      _particle=part;_geni=genID;
    }
    ParticleConfig(TString name,BaseParticle* part,Int_t genID){
      _name=name;_particle=part;_geni=genID;
    }
    virtual ~ParticleConfig()=default;
    ParticleConfig(const ParticleConfig& other) = default; //Copy Constructor
    ParticleConfig(ParticleConfig&& other) = default; //Move Constructor
      
    ParticleConfig& operator=(const ParticleConfig& other)=default;
    ParticleConfig& operator=(ParticleConfig&& other)=default;

    BaseParticle* Particle() const {return _particle;}
      
    std::vector<BaseParticle* > Children(Int_t pdg);
    void AddChild(BaseParticle* child){_children.push_back(child);}
      
    ParticleConfig* Parent() const{return _parent;}
    void SetParent(ParticleConfig* parent){_parent=parent;}
      
    void SetParticleVal(const BaseParticle* part){
      _particle->SetP4(part->P4());
      _particle->SetPDGcode(part->PDG());
      _particle->SetVertex(part->Vertex());
    }
    UInt_t GetNChild() const {return _children.size();};
    Int_t PDG() const {return _particle->PDG();}
    Int_t GenID() const {return _geni;}
    TString GetName() const {return _name;}
      
  private:
      
    TString _name;
    BaseParticle* _particle=nullptr;
    ParticleConfig* _parent=nullptr;
    std::vector<BaseParticle*> _children;
      
    Int_t _geni=-1; //index in generated events if has one
  };
}
   
