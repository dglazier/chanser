//////////////////////////////////////////////////////////////
///
///Class:		templateFS
///Description:
///
#pragma once


#include "CLAS12FinalState.h"
#include "CLAS12Particle.h"

#include "TreeDatatemplateFS.h"


namespace USERNAME{

  using Particle=chanser::CLAS12Particle;

  class templateFS : public chanser::CLAS12FinalState{

       
  public :
    templateFS()=default;
      
   TString GetUSER() final {return _USER;};
 
    //create an instance of the class
    static std::unique_ptr<templateFS> Make(TString ch,TString inc) {
      return std::unique_ptr<templateFS>{new templateFS{ch,inc}};
    }
    //create an instance of the treedata, should be used to init unique_ptr
    chanser::base_outevt_uptr TreeDataFactory() final {
      return chanser::base_outevt_uptr{new TreeDatatemplateFS{}};
    }
    void SetOutEvent(BaseOutEvent* out) final{
      TD=static_cast<TreeDatatemplateFS*>(out);
    }
  
    ~templateFS() final =default;

    void Define() final;
      
    BaseOutEvent* GetOutEvent() noexcept final{return TD;}
      
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
    //chanser::CLAS12Particle _PARTICLE=BaseParticle("PDG");//!
    
    //Final Parents


    //Initial state
    HSLorentzVector _beam{0,0,10.6,10.6};//!
    HSLorentzVector _target{0,0,0,0.938272};//!


    //Tree Output Data
    TreeDatatemplateFS* TD{nullptr};//!;

   
    
    const TString _USER="USERNAME";
    ClassDefOverride(USERNAME::templateFS,1); //class templateFS
  }; //end templateFS
  
}
