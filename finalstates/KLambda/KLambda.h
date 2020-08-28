//////////////////////////////////////////////////////////////
///
///Class:		KLambda
///Description:
///
#pragma once


#include "CLAS12FinalState.h"
#include "CLAS12Particle.h"

#include "TreeDataKLambda.h"


namespace dglazier{

  using Particle=chanser::CLAS12Particle;

  class KLambda : public chanser::CLAS12FinalState{

       
  public :
    KLambda()=default;
      
   TString GetUSER() final {return _USER;};
 
    //create an instance of the class
    static std::unique_ptr<KLambda> Make(TString ch,TString inc) {
      return std::unique_ptr<KLambda>{new KLambda{ch,inc}};
    }
    //create an instance of the treedata, should be used to init unique_ptr
    chanser::base_outevt_uptr TreeDataFactory() final {
      return chanser::base_outevt_uptr{new TreeDataKLambda{}};
    }
    void SetOutEvent(BaseOutEvent* out) final{
      TD=static_cast<TreeDataKLambda*>(out);
    }
  
    ~KLambda() final =default;

    void Define() final;
      
    BaseOutEvent* GetOutEvent() noexcept final{return TD;}
      
  protected :
    void Kinematics() final;
    void UserProcess() final;
      
      
   
  private:
    //constructor private so only create unique_ptr
    //using KLambda::Make(...)
    //auto fs = dglazier::KLambda::Make("NONE","ALL");
  KLambda(TString ch,TString inc) : chanser::CLAS12FinalState(std::move(ch),std::move(inc)){
      //Give object class name - namespace
      //Used for compiling and loading
      SetName(chanser::Archive::BareClassName(ClassName()));
      Define();
    }

    //Final Particles Detected
    Particle   _electron = Particle{"e-"};//!
    Particle   _proton = Particle{"proton"};//!
    Particle   _pim = Particle{"pi-"};//!
    Particle   _kp = Particle{"K+"};//!
    //chanser::CLAS12Particle _PARTICLE=BaseParticle("PDG");//!
    
    //Final Parents
    Particle _lambda = Particle{"Lambda0"};//!


    //Initial state
    HSLorentzVector _beam{0,0,10.6,10.6};//!
    HSLorentzVector _target{0,0,0,0.938272};//!


    //Tree Output Data
    TreeDataKLambda* TD{nullptr};//!;

   
    
    const TString _USER="dglazier";
    ClassDefOverride(dglazier::KLambda,1); //class KLambda
  }; //end KLambda
  
}
