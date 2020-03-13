#pragma once

#include "ParticleIter.h"
#include "BaseParticle.h"
#include "Selection.h"

namespace chanser{

  //////////////////////////////////////////////////
  ///Interface to ParticleIter
  ///Add selected iterator
  ParticleIter* AddSelectX(Int_t nSelected,vector<BaseParticle*> *parts){
    return new ParticleIter(nSelected,*parts);
     
  }
  //////////////////////////////////////////////////
  ///Interface to ParticleIter
  ///Add selected iterator to previously selected particles
  ///nGroups of nSelected from parts
  ParticleIter* AddSelectToSelected(ParticleIter* diter,Int_t nSelected,vector<BaseParticle*> *parts){
    if(nSelected>diter->GetNSel()) cout<<"WARNING AutoIterUtils::AddSelectToSelected : trying to select more particles than exist in original iterator "<<" of "<<nSelected<<" "<< diter->GetNSel()<<endl;
    cout<<" AddSelectToSelected groups "<<" selected "<<nSelected<<" particles "<<parts->size()<<endl;
    ParticleIter *new_iter=AddSelectX(nSelected,parts);
    //new_iter->SetName(new_iter->GetName()+" sel parent is "+diter->GetName());
    //if(parts->at(0)) new_iter->SetPDG(parts->at(0)->PDG());
      
    diter->SetSelIter(new_iter);
    return new_iter;
  }
  //////////////////////////////////////////////////
  ///Interface to ParticleIter
  ///Add selected iterator to remaining particles
  ParticleIter* AddSelectToRemainder(ParticleIter* diter,Int_t nSelected){
    cout<<" AddSelectToRemainder groups "<<" selected "<<nSelected<<endl;
    ParticleIter *new_iter=new ParticleIter(nSelected);
    
    diter->SetRemIter(new_iter);
    return new_iter;
  }

  
  //filter vectors via a lambda function
  //see for example clas12reader::getByID()
  template <typename Cont, typename Pred>
    Cont container_filter(const Cont &container, Pred predicate){
    Cont result;
    std::copy_if(container.begin(),container.end(),std::back_inserter(result), predicate);
    return std::move(result);
  }
  ///////////////////////////////////////////////////////////
  std::vector<ParticleConfig> filterDetParticleConfigsByPid(const std::vector<ParticleConfig>& configs, Short_t id){
    return container_filter(configs, [id](ParticleConfig conf)
			    {return conf.PID()==id&&(conf.GetNChild()==0);});
  }
  ////////////////////////////////////////////////////////////
  std::vector<ParticleConfig> filterDetParticleConfigsByPdg(const std::vector<ParticleConfig>& configs, Short_t pdg){
    return container_filter(configs, [pdg](ParticleConfig conf)
			    {return conf.PDG()==pdg&&(conf.GetNChild()==0);});
  }
   ////////////////////////////////////////////////////////////
  std::vector<ParticleConfig> filterDetParticleConfigsByParent(const std::vector<ParticleConfig>& configs, const TString& name){
    return container_filter(configs, [name](ParticleConfig conf)
			    {return conf.Parent()!=nullptr ? conf.Parent()->GetName()==name&&(conf.GetNChild()==0) : 0;});
  }
  ////////////////////////////////////////////////////////////
  std::vector<ParticleConfig> filterDetParticleConfigsByParent(const std::vector<ParticleConfig>& configs){
    return container_filter(configs, [](ParticleConfig conf)
			    {return conf.Parent()!=nullptr&&(conf.GetNChild()==0);});
  }
  ///////////////////////////////////////////////////////////
  std::vector<Short_t> whichParticleConfigPDGs(const std::vector<ParticleConfig>& configs){
    std::vector<Short_t> pdgs;
    for(const auto &conf :configs)
      if( std::count(pdgs.begin(),pdgs.end(),conf.PDG())==0 )
	pdgs.push_back(conf.PDG());

    return std::move(pdgs);
  }
  /////////////////////////////////////////////////////////////
  std::vector<Short_t> whichParticleConfigPIDs(const std::vector<ParticleConfig>& configs){
    std::vector<Short_t> pdgs;
    for(const auto &conf :configs)
      if( std::count(pdgs.begin(),pdgs.end(),conf.PID())==0 )
	pdgs.push_back(conf.PID());

    return std::move(pdgs);
  }
  /////////////////////////////////////////////////////////////
  std::vector<TString> whichParticleConfigParents(const std::vector<ParticleConfig>& configs){
    std::vector<TString> names;
    for(const auto &conf :configs){
      if(conf.Parent()==nullptr) continue;
      if( std::count(names.begin(),names.end(),conf.Parent()->GetName())==0 )
	names.push_back(conf.Parent()->GetName());
    }
    return std::move(names);
  }
}

