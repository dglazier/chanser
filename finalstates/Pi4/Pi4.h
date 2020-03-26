//////////////////////////////////////////////////////////////
///
///Class:		Pi4
///Description:
///
#pragma once


#include "CLAS12FinalState.h"
#include "CLAS12Particle.h"

#include "TreeDataPi4.h"


namespace someone{

  using Particle=chanser::CLAS12Particle;

  class Pi4 : public chanser::CLAS12FinalState{

       
  public :
    Pi4()=default;
      
   TString GetUSER() final {return _USER;};
 
    //create an instance of the class
    static std::unique_ptr<Pi4> Make(TString ch,TString inc) {
      return std::unique_ptr<Pi4>{new Pi4{ch,inc}};
    }
    //create an instance of the treedata, should be used to init unique_ptr
    chanser::base_outevt_uptr TreeDataFactory() final {
      return chanser::base_outevt_uptr{new TreeDataPi4{}};
    }
    void SetOutEvent(BaseOutEvent* out) final{
      TD=static_cast<TreeDataPi4*>(out);
    }
  
    ~Pi4() final =default;

    void Define() final;
      
    BaseOutEvent* GetOutEvent() noexcept final{return TD;}
      
  protected :
    void Kinematics() final;
    void UserProcess() final;
      
      
   
  private:
    //constructor private so only create unique_ptr
    //using Pi4::Make(...)
    //auto fs = someone::Pi4::Make("NONE","ALL");
  Pi4(TString ch,TString inc) : chanser::CLAS12FinalState(std::move(ch),std::move(inc)){
      //Give object class name - namespace
      //Used for compiling and loading
      SetName(chanser::Archive::BareClassName(ClassName()));
      Define();
    }

    //Final Particles Detected
    Particle   _electron = Particle{"e-"};//!
    Particle   _proton = Particle{"proton"};//!
    Particle   _pip1 = Particle{"pi+"};//!
    Particle   _pip2 = Particle{"pi+"};//!
    Particle   _pim1 = Particle{"pi-"};//!
    Particle   _pim2 = Particle{"pi-"};//!
    //chanser::CLAS12Particle _PARTICLE=BaseParticle("PDG");//!
    
    //Final Parents
    Particle _k1 = Particle{"K0"};//!
    Particle _k2 = Particle{"K0"};//!


    //Initial state
    HSLorentzVector _beam{0,0,10.6,10.6};//!
    HSLorentzVector _target{0,0,0,0.938272};//!


    //Tree Output Data
    TreeDataPi4* TD{nullptr};//!;

   
    
    const TString _USER="someone";
    ClassDefOverride(someone::Pi4,1); //class Pi4
  }; //end Pi4
  
}
