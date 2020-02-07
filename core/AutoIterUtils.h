#pragma once

#include "ParticleIter.h"
#include "BaseParticle.h"
#include "Selection.h"

namespace chanzer{

  //////////////////////////////////////////////////
  ///Interface to ParticleIter
  ///Add selected iterator
  ParticleIter* AddSelectXofY(Int_t ni,Int_t nsel,vector<BaseParticle*> *parts){
    return new ParticleIter(ni,nsel,*parts);
     
  }
  //////////////////////////////////////////////////
  ///Interface to ParticleIter
  ///Add selected iterator to previously selected particles
  ParticleIter* AddSelectToSelected(ParticleIter* diter,Int_t ni,Int_t nsel,vector<BaseParticle*> *parts){
    if(nsel*ni>diter->GetNSel()) cout<<"WARNING HS::FinalState::AddSelectToSelected : trying to select more particles than exist in original iterator "<<nsel*ni<<" "<< diter->GetNSel()<<endl;

    ParticleIter *new_iter=AddSelectXofY(ni,nsel,parts);
    new_iter->SetName(new_iter->GetName()+" sel parent is "+diter->GetName());
    if(parts->at(0)) new_iter->SetPDG(parts->at(0)->PDG());
      
    diter->SetSelIter(new_iter);
    return new_iter;
  }
  //////////////////////////////////////////////////
  ///Interface to ParticleIter
  ///Add selected iterator to remaining particles
  ParticleIter* AddSelectToRemainder(ParticleIter* diter,Int_t ni,Int_t nsel){
    ParticleIter *new_iter=new ParticleIter(ni,nsel);
    
    diter->SetRemIter(new_iter);
    return new_iter;
  }

}

