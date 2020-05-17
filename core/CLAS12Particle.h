//////////////////////////////////////////////////////////////
///
///Class:		CLAS12Particle
///Description:
///
#pragma once

#include "BaseParticle.h"
#include "region_particle.h"

namespace chanser{


  class CLAS12Particle : public BaseParticle {


  public :
    CLAS12Particle()=default;
    virtual ~CLAS12Particle()=default;
  CLAS12Particle(TString pdgname):BaseParticle(pdgname){}  	        //Constructor
    
    void SetCLAS12Particle(clas12::region_particle *p);
    clas12::region_particle* CLAS12() const {return _c12Particle;}
    void MinorClear() override;
    void CopyParticle(const BaseParticle* part,Bool_t andPDG) override;
    void CopyTransient(const BaseParticle* part) override;

  private :
    clas12::region_particle* _c12Particle{nullptr};//!
    
  };
  inline void chanser::CLAS12Particle::MinorClear(){
     _c12Particle=nullptr;
     BaseParticle::MinorClear();
  }
  inline void chanser::CLAS12Particle::CopyParticle(const BaseParticle* part,Bool_t andPDG){
    
    _c12Particle=static_cast<const CLAS12Particle*>(part)->CLAS12();
     BaseParticle::CopyParticle(part,andPDG);
      
    }
  inline void chanser::CLAS12Particle::CopyTransient(const BaseParticle* part){
    // std::cout<<"chanser::CLAS12Particle::CopyParticle("<<std::endl;
    _c12Particle=static_cast<const CLAS12Particle*>(part)->CLAS12();
    BaseParticle::CopyTransient(part);
  }
    inline void chanser::CLAS12Particle::SetCLAS12Particle(clas12::region_particle *p){
    _c12Particle=p;
    SetPDGcode(p->getPid());//must take region_particle in case useFTbased

    //Get momentum and vertex from pbank
    auto pbank=p->par();
    _charge=pbank->getCharge();
    SetXYZT(pbank->getPx(),pbank->getPy(),pbank->getPz(),pbank->getP());
    SetVertex(pbank->getVx()/100,pbank->getVy()/100,pbank->getVz()/100);//change to m

    //take time and path as defined by region_particle
    _time=p->getTime();
    _path=p->getPath()/100; //change to m
  
  }
}
