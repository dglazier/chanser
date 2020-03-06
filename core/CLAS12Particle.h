//////////////////////////////////////////////////////////////
///
///Class:		CLAS12Particle
///Description:
///
#pragma once

#include "BaseParticle.h"
#include "region_particle.h"

namespace chanzer{


  class CLAS12Particle : public BaseParticle {


  public :
    CLAS12Particle()=default;
    virtual ~CLAS12Particle()=default;
  CLAS12Particle(TString pdgname):BaseParticle(pdgname){}  	        //Constructor
    
    void SetCLAS12Particle(clas12::region_particle *p);
    clas12::region_particle* CLAS12() const {return _C12Particle;}
    void MinorClear() override;
    void CopyParticle(const BaseParticle* part,Bool_t andPDG) override;
    void CopyTransient(const BaseParticle* part) override;

  private :
    clas12::region_particle* _C12Particle{nullptr};
    
  };
  inline void chanzer::CLAS12Particle::MinorClear(){
     _C12Particle=nullptr;
     BaseParticle::MinorClear();
  }
  inline void chanzer::CLAS12Particle::CopyParticle(const BaseParticle* part,Bool_t andPDG){
    
    _C12Particle=static_cast<const CLAS12Particle*>(part)->CLAS12();
     BaseParticle::CopyParticle(part,andPDG);
      
    }
  inline void chanzer::CLAS12Particle::CopyTransient(const BaseParticle* part){
    // std::cout<<"chanzer::CLAS12Particle::CopyParticle("<<std::endl;
    _C12Particle=static_cast<const CLAS12Particle*>(part)->CLAS12();
    BaseParticle::CopyTransient(part);
  }
    inline void chanzer::CLAS12Particle::SetCLAS12Particle(clas12::region_particle *p){
    _C12Particle=p;
    auto pbank=p->par();
    SetPDGcode(pbank->getPid());
    _Charge=pbank->getCharge();
    SetXYZT(pbank->getPx(),pbank->getPy(),pbank->getPz(),pbank->getP());
    SetVertex(pbank->getVx()/100,pbank->getVy()/100,pbank->getVz()/100);//change to m
    _Time=p->getTime();
    _Path=p->getPath()/100; //change to m
  
  }
}
