//////////////////////////////////////////////////////////////
///
///Class:		templateFS
///Description:
///
#pragma once

#include <utility>

#include "CLAS12FinalState.h"
#include "CLAS12Particle.h"

#include "TreeDatatemplateFS.h"

#pragma link C++ namespace USERNAME;

namespace USERNAME{

  class templateFS : public chanser::CLAS12FinalState{

       
  public :
    templateFS()=default;
      

    //create an instance of the class
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
    //constructor private so only create unique_ptr
    //using templateFS::Make(...)
    //auto fs = USERNAME::templateFS::Make("NONE","ALL");
  templateFS(TString ch,TString inc) : chanser::CLAS12FinalState(std::move(ch),std::move(inc)){
      //Give object class name - namespace
      //Used for compiling and loading
      SetName(chanser::Archive::BareClassName(ClassName()));
      Define();
    }

    //Final Particles Detected
    //BaseParticle _PARTICLE=BaseParticle("PDG");//!
    
    //Final Parents


    //Initial state
    HSLorentzVector _beam{0,0,10.6,10.6};//!
    HSLorentzVector _target{0,0,0,0.938272};//!


    //Tree Output Data
    TreeDatatemplateFS TD;

   
    
    ClassDefOverride(templateFS,1); //class templateFS
  }; //end templateFS
  
}
