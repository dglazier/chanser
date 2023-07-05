//////////////////////////////////////////////////////////////
///
///Class:		Pi2
///Description:
///
#pragma once


#include "CLAS12FinalState.h"
#include "CLAS12Particle.h"

#include "TreeDataPi2.h"


namespace username{

  using Particle=chanser::CLAS12Particle;

  class Pi2 final : public chanser::CLAS12FinalState{

    static constexpr Double_t Mprot=0.938272;
    static constexpr Double_t Mpion=0.13957000;
    static constexpr Double_t Melec=0.00051099891;
    
  public :
    Pi2()=default;
      
   TString GetUSER() final {return _USER;};
 
    //create an instance of the class
    static std::unique_ptr<Pi2> Make(TString ch,TString inc) {
      return std::unique_ptr<Pi2>{new Pi2{ch,inc}};
    }
    //create an instance of the treedata, should be used to init unique_ptr
    chanser::base_outevt_uptr TreeDataFactory() final {
      return chanser::base_outevt_uptr{new TreeDataPi2{}};
    }
    void SetOutEvent(BaseOutEvent* out) final{
      TD=static_cast<TreeDataPi2*>(out);
    }
  
    ~Pi2() final =default;

    void Define() final;
      
    BaseOutEvent* GetOutEvent() noexcept final{return TD;}


    void DerivedChangeRun() final {
      auto ebeam=GetRunInfo()->_BeamEnergy;
       //     ebeam=10.6; //Fall 2018
       //     ebeam=10.2; //Spring 2019
      std::cout<<"Change beam energy to :"<<ebeam<<std::endl;
      _beam.SetXYZT(0,0,ebeam,TMath::Sqrt(ebeam*ebeam + Melec*Melec));
    }

  protected :
    void Kinematics() final;
    void UserProcess() final;
      
      
   
  private:
    //constructor private so only create unique_ptr
    //using Pi2::Make(...)
    //auto fs = username::Pi2::Make("NONE","ALL");
  Pi2(TString ch,TString inc) : chanser::CLAS12FinalState(std::move(ch),std::move(inc)){
      //Give object class name - namespace
      //Used for compiling and loading
      SetName(chanser::Archive::BareClassName(ClassName()));
      Define();
    }

    //Final Particles Detected
    Particle   _electron = Particle{"e-"};//!
    Particle   _proton = Particle{"proton"};//!
    Particle   _pip = Particle{"pi+"};//!
    Particle   _pim = Particle{"pi-"};//!
    //chanser::CLAS12Particle _PARTICLE=BaseParticle("PDG");//!
    
    //Final Parents


    //Initial state
    HSLorentzVector _beam{0,0,10.6,10.6};//!
    HSLorentzVector _target{0,0,0,0.938272};//!


    //Tree Output Data
    TreeDataPi2* TD{nullptr};//!;

   
    
    const TString _USER="username";
    ClassDefOverride(username::Pi2,1); //class Pi2
  }; //end Pi2
  
}
