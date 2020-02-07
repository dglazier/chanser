//////////////////////////////////////////////////////////////
///
///Class:		Pi3
///Description:
///
#pragma once

#include <utility>

#include "CLAS12FinalState.h"

#include "TreeDataPi3.h"



class Pi3 : public CLAS12FinalState{

       
    public :
      Pi3()=default;
      
 Pi3(TString ch,TString inc) : CLAS12FinalState(std::move(ch),std::move(inc)){SetName("Pi3");Define();}
      
      static std::unique_ptr<Pi3> Make(TString ch,TString inc) {
	return std::unique_ptr<Pi3>{new Pi3{ch,inc}};
      }
      
      ~Pi3() final =default;

      void Define() final;
      
      BaseOutEvent* GetOutEvent() noexcept final{return &TD;}
      
 protected :
      void Kinematics() final;
      void UserProcess() final;
      
      
   
    private:

      //Final Particles Detected
      //  BaseParticle _PARTICLE=BaseParticle("PDG");
      BaseParticle _Electron=BaseParticle("e-");
      BaseParticle _Pip1=BaseParticle("pi+");
      BaseParticle _Pip2=BaseParticle("pi+");
      BaseParticle _Pim=BaseParticle("pi-");

      //Initial state
      HSLorentzVector _beam{0,0,10.6,10.6};
      HSLorentzVector _target{0,0,0,0.938272};


      //Tree Output Data
      TreeDataPi3 TD;

   
    
      ClassDefOverride(Pi3,1); //class Pi3
    };
  
