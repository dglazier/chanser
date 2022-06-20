#include "TopologyManager.h"
#include "BaseParticle.h"
#include "FinalState.h"
#include "Topology.h"
#include <TDatabasePDG.h>
#include <TObjString.h>


namespace chanser{
    
  using std::vector;
    
  //////////////////////////////////////////////////////////////////
  ///

  ////////////////////////////////////////////////////////////////////
  ///Add a new topology
  ///requires particle string of names defining particles to be detected
  /// a function intialising the particle iterator
  /// a function defining the specific behaviour for this topology
  /// a string if want to use charge as identiification
  /// a tring if want to look for inclusive topologies
  Topology* TopologyManager::AddTopology(TString names,VoidFuncs funcE){

    TString topoPDGs;
    vector<BaseParticle* > parts;
    vector<ParticleConfig > pconfigs;
    vector<TString> partnames;
      
    //convert the string of particle names into a string of particle types
    //Save the BaseParticle pointer for each particle in this topology
    auto listofparts=names.Tokenize(":");
    for(Int_t ip = 0; ip<listofparts->GetEntries();ip++){
      partnames.push_back(listofparts->At(ip)->GetName());
      // cout<<ip<<" TopologyManager::AddTopology "<<listofparts->At(ip)->GetName()<<endl;
	
      auto part=_fs->GetParticle(listofparts->At(ip)->GetName());
      parts.push_back(part);
      auto partc=_fs->GetParticleConfig(listofparts->At(ip)->GetName());
      pconfigs.push_back(partc);
      topoPDGs+=TDatabasePDG::Instance()->GetParticle(part->PDG())->GetName();
      if(ip!=listofparts->GetEntries())	topoPDGs+=":";
    }
      
    //create new topology	
    Topology topo(topoPDGs,funcE,this);
    topo.SetID(_nTopo++);
    topo.SetParticles(parts);
    topo.SetParticleConfigs(pconfigs);
    topo.SetPartNames(partnames);

    _actualTopos.push_back(topo.Definition()); //just the particle IDs
    _topos.push_back(std::move(topo));
       
    return &(_topos.back()); //return pointer to last topo
    //  fCurrIter=nullptr;
    // funcI();//call the assigned particle iterator initialiser
    // fCurrIter->ConfigureIters();

    //if(!fCurrIter)cout<<"HS::FinalState::AddTopology warning no Init_Iter working for topology "<<topo<<endl;
  }
  /////////////////////////////////////////////////////////////////////
  ///Note particles with TruthOnly are reserved for undetected particles
  ///which may be kept in simulated data, but shouldn't be included
  ///in final state check
  Bool_t TopologyManager::CheckForValidTopos(const std::vector<short>& pids){
    // std::cout<<"TopologyManager::CheckForValidTopos "<<_nTopo<<std::endl;
    _eventTopo.clear();
    _eventTopo.reserve(pids.size());
    //loop over event particle ids and reconfigure them
    //for these topologies
    // std::cout<<"DEBUG  TopologyManager::CheckForValidTopos ";
    for(const auto& pid:pids){
      // if(_useChargePID==kFALSE&&(pid==UndefinedPDG||pid==-UndefinedPDG||pid==0))
      // 	 continue;
      
      _eventTopo.emplace_back(ParticleID(pid));
      // std::cout<<" ( "<<pid<<" "<<ParticleID(pid)<< " ) ";//DEBUG
    }
    //std::cout<<std::endl;//DEBUG
    
    std::sort(_eventTopo.begin(),_eventTopo.begin()+_eventTopo.size());
 
    //now check to see if this topology matches any of our predefines ones
    _validTopos.clear();
    for(size_t i=0;i<_nTopo;++i){
      if(!CheckTopo(i))
	continue; //This event does not apply to this topology
      _validTopos.push_back(&_topos[i]);    
    }
    if( _validTopos.size()) return kTRUE;
    return kFALSE;
  }
  ////////////////////////////////////////////////////////////////////
  void TopologyManager::ConfigParticleID(const Short_t pdg){
    //First check the case of no defined PDG code
    //i.e. Rootinos, these are always converted to charge
    _confPids.push_back(pdg);
    if(pdg==_noID){ _pdg2pid[pdg]=_noID;}
    else if(pdg==-_noID){ _pdg2pid[pdg]=-_noID; }
    else if(pdg==0){ _pdg2pid[pdg]=0;}

    //requested to change all Ids to charge
    else if(_chargeParts.size()==0)
      _pdg2pid[pdg]=PDGtoCharge(pdg)*_noID;
  
    //check if this is assigned charge particle and change it
    // else if(std::count(_chargeParts.begin(), _chargeParts.end(), pdg))
    //	_pdg2pid[pdg]=PDGtoCharge(pdg)*_noID;

    else _pdg2pid[pdg]=pdg;
  }
  //////////////////////////////////////////////////////////////////////
  ///Check if the topology itopo exactly matches the current detected one
  Bool_t TopologyManager::CheckTopo(const size_t itopo) const{
    // cout<<"CheckTopo "<<fID<<endl;
    // for(Int_t i=0;i<fActualDefinition.size();i++)
    //   cout<<fActualDefinition[i]<<" ";
    // cout<<endl;
    //If want to use charge ID convert this topo from PDG to charge
    //if(_useChargePID) TopoToCharge(detTopo);
    
    if(_isInclusive) return CheckInclusiveTopo(itopo);
    return CheckExclusiveTopo(itopo);
  }

  Bool_t TopologyManager::CheckExclusiveTopo(const size_t itopo) const{
    //cout<<"CheckExclusiveTopo "<<endl;
    auto actual = _actualTopos[itopo];
    if(actual.size()!=_eventTopo.size())
      return kFALSE;
    if(_eventTopo==actual){

      // for(auto& ai:actual)
      //   cout<<ai<<" ";
      // cout<<endl;
      return kTRUE;
    }
    //	cout<<":CheckExclusiveTopo( failed "<<endl;
    return kFALSE;
  }
  //////////////////////////////////////////////////////////////////////
  ///Check if the topology itopo matches inclusively the current detected one
  Bool_t TopologyManager::CheckInclusiveTopo(const size_t itopo) const{
    //cout<<"CheckInclusiveTopo "<<endl;
 
      
    auto actual = _actualTopos[itopo];


    std::vector<Short_t> ptypes; //counter for different particle types
    //in eventTopo

    for(const auto& acti:actual){
      //check if ptypes already includes this type fDefinition[ipart]
      if(std::find(ptypes.begin(), ptypes.end(), acti)==ptypes.end())
	ptypes.push_back(acti);//Not there so add it
      //Now see if sufficient number in thisTopo
	
      Int_t thiscount = std::count (_eventTopo.begin(), _eventTopo.end(), acti);
      Int_t topocount = std::count (actual.begin(), actual.end(), acti);
 
      if(thiscount<topocount)//Not sufficient of this type, not this topo
	return kFALSE;
      //Check if this is an allowed inclusive particle
      if(_incParts.size()){//if size=0 assume all particles are allowed inclusive
	if(std::count (_incParts.begin(), _incParts.end(), acti))
	  continue; //it is so OK
	else  //it is not so need exact match
	  if(thiscount!=topocount)
	    return kFALSE;
      }
    }
      
    return kTRUE;
  }
    
  ////////////////////////////////////////////////////////////////
  ///Look for topologies by charge rather than pdg code
  ///if the string parts is given with PDG names, only those particle
  ///types will use  pdg code others will still use charge
  ///"ALL" means all particles will use pdg code
  ///"NONE" means all particles use charge
  ///"e-:proton" means use electron and proton pdg code all else charge
  void TopologyManager::SetChargePID(TString parts){

    if(parts==TString("ALL")) return;   
    _useChargePID=1;

    return;
      
    if(parts==TString("NONE")) return;

 
    _chargeParts.clear();
    //if specified which particles can be inclusive
    //save their pdg in fIncParts
    TObjArray* OptList = parts.Tokenize(":");
    for(Int_t i=0;i<OptList->GetEntries();i++){
      Int_t pdg=0;
      Int_t charge=0;
      if(((TObjString*)OptList->At(i))->String()==TString("Rootino+")) {pdg=_noID;charge=1;}
      else if(((TObjString*)OptList->At(i))->String()==TString("Rootino-")) {pdg=-_noID;charge=-1;}
      else if(((TObjString*)OptList->At(i))->String()==TString("Beam")) {pdg=-22;charge=0;}
      else if(!TDatabasePDG::Instance()->GetParticle(((TObjString*)OptList->At(i))->String()))Error("FinalState::SetChargePID","Particle not found = %s",((TObjString*)OptList->At(i))->String().Data());
      else{
	pdg=TDatabasePDG::Instance()->GetParticle(((TObjString*)OptList->At(i))->String())->PdgCode();
      }
      _chargeParts.push_back(pdg);
    }
    std::sort(_chargeParts.begin(),_chargeParts.begin()+_chargeParts.size());
    cout<<"Charge ID particle are  ";
    if(_chargeParts.size()==0)cout <<" ALL ";
    for(UInt_t i=0;i<_chargeParts.size();i++)
      cout<<_chargeParts[i]<<" ";
    cout<<endl;
  
  }
  ////////////////////////////////////////////////////////////////
  ///Look for inclusive topologies
  ///if the string parts is given with PDG names, only those particle
  ///types are allowed to have more than their topology defined number
  void TopologyManager::SetInclusive(TString parts){
    if(parts==TString("")) return;
    if(parts==TString("NONE")) return;
      
    _isInclusive=1;
    if(parts==TString("ALL")) return;
    _incParts.clear();
    //if specified which particles can be inclusive
    //save their pdg in fIncParts
    TObjArray* OptList = parts.Tokenize(":");
    for(Int_t i=0;i<OptList->GetEntries();i++){
      Int_t pdg=0;
      Int_t charge=0;
      if(((TObjString*)OptList->At(i))->String()==TString("Rootino+")) {pdg=_noID;charge=1;}
      else if(((TObjString*)OptList->At(i))->String()==TString("Rootino-")) {pdg=-_noID;charge=-1;}
      else if(((TObjString*)OptList->At(i))->String()==TString("Beam")) {pdg=-22;charge=0;}
      else if(!TDatabasePDG::Instance()->GetParticle(((TObjString*)OptList->At(i))->String()))Error("FinalState::SetInclusive","Particle not found = %s",((TObjString*)OptList->At(i))->String().Data());
      else{
	pdg=TDatabasePDG::Instance()->GetParticle(((TObjString*)OptList->At(i))->String())->PdgCode();
      }
      //add this ParticleID if not there already (in case of charge ID)
      if(std::count (_incParts.begin(), _incParts.end(), ParticleID(pdg))==0) _incParts.push_back(ParticleID(pdg));
    }
    std::sort(_incParts.begin(),_incParts.begin()+_incParts.size());
    cout<<"Incusive particle are  ";
    if(_incParts.size()==0)cout <<" ALL ";
    for(UInt_t i=0;i<_incParts.size();i++)
      cout<<_incParts[i]<<" ";
    cout<<endl;
  
  }
  /////////////////////////////////////////////////////////////////////////
  void TopologyManager::Print() const {
    if(_isInclusive){
      cout<<"You can have any number of the following particles : "<<endl;
      if(_incParts.size()){
	for(UInt_t i=0;i<_incParts.size();i++)
	  cout<<_incParts[i]<<" ";
	cout<<endl;
      }
      else cout<<" ALL "<<endl;
    }
    if(_useChargePID){
      cout<<"The following particle are identified by pdg code : "<<endl;
      if(_chargeParts.size()){
	for(UInt_t i=0;i<_chargeParts.size();i++)
	  cout<<_chargeParts[i]<<" ";
	cout<<endl;
      }

       }
    else cout<<" ALL "<<endl;
    for(auto& topo: _topos)
      topo.Print(1);

  }
    
}
