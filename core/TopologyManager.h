//////////////////////////////////////////////////////////////
///
///Class:               TopologyManager
///Description:
///

#pragma once

#include <TString.h>
#include <TDatabasePDG.h>
#include <vector>
#include <map>
#include <functional>
#include <iostream>


namespace chanser{

  class Topology;
  class FinalState;
    
  //Typedef for pointer to function
  //used to link a derived class function with using its data members
  //To a function call in AddTopology and link to a call in Topology 
  using VoidFuncs= std::function<void()>;
     
  class TopologyManager{

  public:
     
    TopologyManager()=default;
  TopologyManager(const FinalState* fs): _fs(fs){};
  TopologyManager(const FinalState* fs,const TString chPid,const TString incl): _fs(fs)
    {SetChargePID(chPid);SetInclusive(incl);};
     
    virtual ~TopologyManager()=default;
    TopologyManager(const TopologyManager& other) = default; //Copy Constructor
    TopologyManager(TopologyManager&& other) = default; //Move Constructor
      
    TopologyManager& operator=(const TopologyManager& other)=default;
    TopologyManager& operator=(TopologyManager&& other)=default;
 
    Topology*  AddTopology(TString names,VoidFuncs funcE);

    void SetFinalState(const FinalState* fs){_fs=fs;}
    const FinalState* GetFinalState() const {return _fs;}
      
    void Print()const;

    void ClearTopos(){_topos.clear();}
      
    const std::vector<Topology>& ObserveTopos() const  {return _topos;}
    std::vector<Topology>& Topos() {return _topos;}
      
    Bool_t CheckForValidTopos(const std::vector<Short_t>& pids);
    //////////////////////////////////////////////////////////////////////
    ///Convert particle PDG code to the particle charge
    Short_t ParticleID(const Short_t pdg) {
      if(!_useChargePID) return pdg;
      if(std::find(_confPids.begin(), _confPids.end(), pdg) == _confPids.end()) ConfigParticleID(pdg); //could remove this and just configure know particles

      return _pdg2pid.at(pdg);
    }
      
    const std::vector<Topology*>& ValidTopos() const {return _validTopos;}
      
    void SetChargePID(TString parts);
    void SetInclusive(TString parts);

  private :

    void ConfigParticleID(const Short_t pdg);
    Bool_t CheckTopo(const size_t itopo) const;
    Bool_t CheckExclusiveTopo(const size_t itopo) const;
    Bool_t CheckInclusiveTopo(const size_t itopo) const;
       
    Short_t PDGtoCharge(Short_t pdg) const{
      TParticlePDG* pp=TDatabasePDG::Instance()->GetParticle(pdg);
      if(!pp)return 0;
      Short_t charge=pp->Charge();
      if(charge!=0)charge=charge/TMath::Abs(charge); //just get sign not mag.
      return charge;
    }


    std::vector<Topology> _topos; 
    const FinalState* _fs=nullptr; //!

      
    std::map<Short_t,Short_t>_pdg2pid;
      
    std::vector<std::vector<Short_t > > _actualTopos;
    std::vector<Short_t > _eventTopo;

    std::vector<Short_t > _confPids;
    std::vector<Short_t> _incParts; //particle allowed to be inclusive
    std::vector<Short_t> _chargeParts; //particle allowed be IDed by charge

    std::vector<Topology*> _validTopos; //Topologies good for current event
      
    UInt_t _nTopo=0;
    Short_t _isInclusive=0;
    Short_t _useChargePID=0;

    // TString _chPid;
    //TString _incl;
      
    static constexpr Short_t _noID=1E4;
 
    
        
  };
}

