//////////////////////////////////////////////////////////////
///
///Class:		anaPi2
///Description:
///
#pragma once

#include <utility>

#include "CLAS12FinalState.h"

#include "TreeDataPi2.h"



class anaPi2 : public chanser::CLAS12FinalState{

 
    public :
      anaPi2()=default;
 anaPi2(TString ch,TString inc) : chanser::CLAS12FinalState(std::move(ch),std::move(inc)){
	SetName(chanser::Archive::BareClassName(ClassName()));
	Define();
      }
      
      static std::unique_ptr<anaPi2> Make(TString ch,TString inc) {
	return std::unique_ptr<anaPi2>{new anaPi2{ch,inc}};
      }
      
      ~anaPi2() final =default;

      void Define() final;
      
 

      /* void OutTree(TTree* tree) final; */
      /* void OutHipo(hipo::ntuple_writer* writer) final; */
      
       BaseOutEvent* GetOutEvent() noexcept final{return &TD;}
       
    protected :
      void Kinematics() final;
      void UserProcess() final;
 
    private:

       //Final Particles Detected
      BaseParticle _Electron=BaseParticle("e-");
      BaseParticle _Proton=BaseParticle("proton");
      BaseParticle _Pip=BaseParticle("pi+");
      BaseParticle _Pim=BaseParticle("pi-");

      //Initial state
      HSLorentzVector _beam{0,0,10.6,10.6};
      HSLorentzVector _target{0,0,0,0.938272};


      //HSLorentzVector _miss;
      //  HSLorentzVector _meson;
      
   
      //Tree Output Data
      TreeDataPi2 TD;

   
    
      ClassDefOverride(anaPi2,1); //class anaPi2
    };
  
