//////////////////////////////////////////////////////////////
///
///Class:		templateFS
///Description:
///
#pragma once

#include <utility>

#include "CLAS12FinalState.h"

#include "TreeDatatemplateFS.h"



class templateFS : public CLAS12FinalState{

       
    public :
      templateFS()=default;
      
 templateFS(TString ch,TString inc) : CLAS12FinalState(std::move(ch),std::move(inc)){SetName("templateFS");Define();}
      
      static std::unique_ptr<templateFS> Make(TString ch,TString inc) {
	return std::unique_ptr<templateFS>{new templateFS{ch,inc}};
      }
      
      ~templateFS() final =default;

      void Define() final;
      
      BaseOutEvent* GetOutEvent() noexcept final{return &TD;}
      
 protected :
      void Kinematics() final;
      void UserProcess() final;
      
      
   
    private:

      //Final Particles Detected
      BaseParticle _PARTICLE=BaseParticle("PDG");
    
      //Initial state
      HSLorentzVector _beam{0,0,10.6,10.6};
      HSLorentzVector _target{0,0,0,0.938272};


      //Tree Output Data
      TreeDatatemplateFS TD;

   
    
      ClassDefOverride(templateFS,1); //class templateFS
    };
  
