#include <iostream>
#include "BaseParticle.h"

namespace chanser{

  using std::cout;
  using std::endl;
  
  BaseParticle::BaseParticle(int code) {
    if(!TDatabasePDG::Instance()->GetParticle(code))
      cout<<"BaseParticle::BaseParticle, sorry particle "<<code<<" does not exist in ROOT PDG table"<<endl;
    SetPDGcode(code);
     
  }
  
  BaseParticle::BaseParticle(TString pdgname){
    if(!TDatabasePDG::Instance()->GetParticle(pdgname))
      cout<<"BaseParticle::BaseParticle, sorry particle "<<pdgname<<" does not exist in ROOT PDG table"<<endl;
    SetPDGcode(TDatabasePDG::Instance()->GetParticle(pdgname)->PdgCode());
  }
  void BaseParticle::Print(Option_t *) const{
    cout<<"Printing BaseParticle of type "<<TDatabasePDG::Instance()->GetParticle(_pdgCode)->GetName()<<endl;
  }
}

