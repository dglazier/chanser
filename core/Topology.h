//////////////////////////////////////////////////////////////
///
///Class:		Topology
///Description:
///
#pragma once

#include <vector>
#include <functional>

#include "BaseParticle.h"
#include "ParticleConfig.h"
#include "ParticleIter.h"
#include "FinalState.h"
#include "TopologyManager.h"


namespace chanser{

  class ParticleIter;

  using std::vector;
  using std::cout;
  using std::endl;
  using VoidFuncs= std::function<void()>;

  class Topology{

  public :
    
    Topology()=default;
    Topology(const TString topo,const VoidFuncs funcE,TopologyManager* tm);
    virtual ~Topology()=default;
    
    Topology(const Topology& other) = default; //Copy Constructor
    Topology(Topology&& other) = default; //Move Constructor
      
    Topology& operator=(const Topology& other)=default;
    Topology& operator=(Topology&& other)=default;
 
    VoidFuncs Exec;//!
      
  public:
 
    
    //    void SetIter(ParticleIter* iter){fIter=iter;};
    ParticleIter& Iter(){return _particleIter;}
    
    const vector<Short_t>& Definition() const {return _actualDefinition;}
    const vector<Short_t>& True() const {return _trueDefinition;}
    
    void SetID(Int_t id){_ID=id;}
    Int_t ID() const {return _ID;}
    
      
    const vector<ParticleConfig>& GetParticleConfigs() const {return _pconfigs;}
    const vector<BaseParticle*>& GetParticles() const {return _particles;}
    const BaseParticle* GetParticle(UInt_t ip) const {return _particles[ip];}
    void SetParticles(const vector<BaseParticle*> parts){_particles=parts;};
    void SetParticleConfigs(const vector<ParticleConfig> parts){_pconfigs=parts;};

      
    void SetPartNames(const vector<TString> names){_partNames=names;}
    const vector<TString> GetPartNames()const {return _partNames;}
    const TString GetPartName(UInt_t i) const {if(_partNames.size()>i) return _partNames[i]; cout<<"Warning Topology PartName not enough particles"<<endl; return TString();}
    
    UInt_t NParts() const {return _trueDefinition.size();}
    
    UInt_t HowManyTrue(Short_t pdg){
      return std::count(_trueDefinition.begin(),_trueDefinition.end(),pdg);
    }
    
    void Print(Int_t verbose);
    
    Bool_t FirstParticles(){
      //if(_isGenerated) return; //Truth generated analysis
      //_nPerm++;
      //	auto piter=_currTopo->Iter();
      //	_particleIter.GotoStart();
      return _particleIter.FirstCombitorial0();
      //	_isPermutating0=0;//no other combination
      //return moreToCome; 
    }
 
  private:
    
    ParticleIter _particleIter; //!

      
    vector<BaseParticle*> _particles; //!contains particles from FinalState class
    vector<ParticleConfig> _pconfigs; //!contains particles from FinalState class
    vector<TString> _partNames; //!
    
      
    vector<Short_t> _trueDefinition; //pdg codes needed for this topology
    vector<Short_t> _actualDefinition; //pids needed for this topology
 
    
    Int_t _ID; //reference number
    Short_t _isInclusive=0;
    Short_t _useChargePID=0;

    const Short_t _noID=1E4;//!
    
    ClassDef(chanser::Topology,1); //class Topology
  }; //class Topology
    
  
}

