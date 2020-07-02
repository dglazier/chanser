#include "MVASignalIDAction.h"
#include "VectorUtility.h"
#include "FinalState.h"
#include <TROOT.h>
#include <TError.h>
#include <TDataMember.h>

namespace chanser{

  MVASignalIDAction::MVASignalIDAction(const TString& dirname,const TString& meth,const TString& topo) {

    if(!dirname.BeginsWith('/'))//full path for proof
      _configs[0]._dirname=TString(gSystem->Getenv("PWD"))+"/"+dirname;
    else
      _configs[0]._dirname=dirname;

    _configs[0]._method=meth;
    _configs[0]._topo=topo;

    //name will be used as tree variable
    auto name=dirname.Strip(TString::kTrailing,'/');
    TString namest=gSystem->BaseName(name.String());
    namest+=meth;
    SetName(namest);
  }
  
  void MVASignalIDAction::PrintAction(){
    std::cout<<" MVASignalIDAction::Print() "<<std::endl;
    for( const auto& data : _pdgToData ){
      std::cout<<" for particle "<<data.first<<" will use data : "<<std::endl;
      data.second->Print();
    }
    if( _defData.get()){
      std::cout<<std::endl<<" for default   will use data : "<<std::endl;
      _defData->Print();
    }
    for(auto& conf:_configs){
      std::cout<<" Using training weights from "<<conf._dirname<<" for method "<<conf._method<<" for topology "<<conf._topo<<std::endl<<std::endl<<std::endl;
    }
  }
  ///////////////////////////////////////////////////////////////
  void MVASignalIDAction::Configure(FinalState* fs){
    //Here we need to configure the particle data
    //and the mva result
    
    auto topos=fs->TopoManager().ObserveTopos();
    auto topostrings= fs->GetUsedTopos(); //should be in synch with ObserveTopos
				       
    //loop over all particles and get their assigned ParticleData class
    _mvaResult.resize(topos.size()); //create space for each topology, if topology not added it will be ignored in event loop, via ResultInterface::Method() check
    
    //loop over all topologies and create the mva result
    UInt_t itopo=0;
    for(auto const& topo : topos){
      //check to see if this topogy has an assigned TMVA result
      auto strtopo=topostrings[itopo];//this topology string must have been assinged in constructor or AddTopology function
      auto neededTopo=ranges::filter(_configs,[&strtopo](const TopoConfig& conf){return conf._topo==strtopo;});
      if(neededTopo.empty()) continue; //no assigned tmva for this topo
      auto conf=std::move(neededTopo[0]);
      
      auto topo_parts=topo.GetParticles();

      //Add particle data if not got already
      UInt_t ip=0;
      for(auto const& particle : topo_parts){
	Int_t pdg = particle->PDG();
	if(_pdgToData.find(pdg)==_pdgToData.end()){//if not use default
	  // std::cout<<"MVASignalIDAction::Configure "<<_defData->Class()->GetName()<<_defData->Class_Name()<<" "<<_defData->ClassName()<<std::endl;
	  
	  AddParticleData(_defData.get(),particle,topo.GetPartName(ip++));
	}
	else{ 
	  AddParticleData(_pdgToData[pdg].get(),particle,topo.GetPartName(ip++));
	}

      }
      //now have all particle data we need for this topo
      //auto res=
      _mvaResult[itopo]=mva::ResultByRefLink{conf._dirname,conf._method,_particleLinks};
      //_mvaResult[itopo]=std::move(res);
      ++itopo;
    }

  }
  void MVASignalIDAction::AddParticleData(const ParticleOutEvent* pdata,BaseParticle* particle,const TString& name){

    if(pdata==nullptr) return;//particle data not defined for this particle
    
    for(auto& pexists : _particleData)
      if(TString(pexists->GetName())==name)
	return; //already got it
    
    auto newdata=pdata->clone(); //clones into unique_ptr
    newdata->SetName(name);
    newdata->SetParticle(particle);
    SetVarLinks(newdata.get());
    _particleData.push_back(std::move(newdata)); 
  }
  
  void MVASignalIDAction::SetVarLinks(const ParticleOutEvent* pdata){
    std::map<TString,TString> typelabel;
    typelabel["Double_t"]="/D";typelabel["double"]="/D";
    typelabel["Float_t"]="/F";typelabel["float"]="/F";
    typelabel["Int_t"]="/I";typelabel["int"]="/I";
    typelabel["Long64_t"]="/L";typelabel["long64"]="/L";
    typelabel["Short_t"]="/S"; typelabel["short"]="/S";
    
    const Short_t sizeOfShort=2;
  
    //auto dmList= pdata->Class()->GetListOfDataMembers();
    auto dmList= gROOT->GetClass(pdata->ClassName())->GetListOfDataMembers();
    auto pname=pdata->GetName();
    std::cout<<" MVASignalIDAction::SetVarLinks "<<pname<< " "<<dmList->GetEntries()<<" "<<pdata->Class()->GetName()<<" "<<dmList->At(0)->GetName()<<std::endl;
    
    for(Int_t i=0;i<dmList->GetEntries();i++){ //loop over data members
      auto member=dynamic_cast<TDataMember*> (dmList->At(i));
      auto mtype=TString(member->GetFullTypeName());
      if(typelabel.find(mtype)==typelabel.end())
	continue; //only include basic types
      
      auto moffset = member->GetOffset()/sizeOfShort;
      TString mname=member->GetName();
      auto pvname=pname+mname;
      //add to map connect var name and reference
      _particleLinks[pvname]=reinterpret_cast<Float_t*>(reinterpret_cast<Short_t*>(&(*const_cast<ParticleOutEvent*>(pdata)))+moffset);//static_cast<Float_t*>((Short_t*)(&(pdata)))+moffset);
      //std::cout<<"Adding "<<pvname<<" "<<*_particleLinks[pvname]<<" "<<moffset<<" "<<*(((Short_t*)&(pdata))+moffset)<<std::endl;

    }

  }
  Bool_t MVASignalIDAction::Execute(UInt_t ti){
    //check cut for topology number ti if Method exists
    if(_mvaResult[ti].Method()==nullptr) return kTRUE;

    //we have an initialised TMVA classifier for this topology
    //loop over all particle and set their data variables
    for(auto& p : _particleData)
      p->FillData();

    //Check classifier response for these variables
    _passCut=_mvaResult[ti].Eval();
    return _forReal ? _passCut : kTRUE;
  }
  
  void MVASignalIDAction::SetParticleOut(TString type,ParticleOutEvent* out){
    if(TDatabasePDG::Instance()->GetParticle(type)){
      SetParticleOut(TDatabasePDG::Instance()->GetParticle(type)->PdgCode(),out);
    }
  }
  void MVASignalIDAction::SetParticleOut(Int_t type,ParticleOutEvent* out){
    Int_t pdg=0;
    if(TDatabasePDG::Instance()->GetParticle(type))
      pdg=TDatabasePDG::Instance()->GetParticle(type)->PdgCode();
      
    _pdgToData[type]=poutev_uptr{out};
    //data now belongs to me, do not delete it elsewhere
    
  }
  /////////////////////////////////////////////////////////////////
  void MVASignalIDAction::PostConfigure(FinalState* fs){
    //can now add branches to particle data trees
    Branches(fs);
  }
  /////////////////////////////////////////////////////////////////
  ///Branches same as for ParticleCutsManager, but floats not ints
  void MVASignalIDAction::Branches( FinalState* fs){
    auto trees=fs->GetOutTrees();
    //add cuts to all output tree (particle, final, ...)
    for(auto tree:trees){
      tree->Branch(GetName(),&_passCut,TString(GetName())+"/F");
    }
    if(fs->FinalHipo()){
      fs->FinalHipo()->bank(GetName(),"Pass/F");
      fs->FinalHipo()->linkItemFunc(GetName(),"Pass/F",&_passCut);
    }
  }
  void MVASignalIDAction::AddTopology(const TString& dirname,const TString& meth,const TString& topo){
    
    auto existingTopo=ranges::filter(_configs,[&topo](const TopoConfig& conf){return conf._topo==topo;});
    if(existingTopo.empty()==false) {Fatal("MVASignalIDAction::AddTopology",Form("already assigned this topo %s",topo.Data()),"");}
    
      if(_configs[0]._topo.Length()==0){
	
	if(!dirname.BeginsWith('/')) //full path for proof
	  _configs[0]._dirname=TString(gSystem->Getenv("PWD"))+"/"+dirname;
	else
	  _configs[0]._dirname=dirname;

	_configs[0]._method=meth;
 	_configs[0]._topo=topo;
	
	auto name=dirname.Strip(TString::kTrailing,'/');
	TString namest=gSystem->BaseName(name.String());
	namest+=meth;
	SetName(namest);
	
	return;
      }
      TopoConfig config;
      if(!dirname.BeginsWith('/'))//full path for proof
	config._dirname=TString(gSystem->Getenv("PWD"))+"/"+dirname;
      else
	config._dirname=dirname;

      config._dirname=dirname;
      config._method=meth;
      config._topo=topo;
      _configs.push_back(std::move(config));

   
  }
 
}
