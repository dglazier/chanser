//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "CLAS12ParticleOutEvent.h"

namespace USERNAME{
   
  class templateClass : public chanser::CLAS12ParticleOutEvent {

  public :
      
    templateClass()=default;// must have default constructor
    
    chanser::partoutev_uptr clone() final{return std::unique_ptr<templateClass>(new templateClass{});}

      
    void FillData() final;

    //Function required to set tree branches
    //virtual void Branches(ttree_ptr tree) =0 ;
    //Class()->GetListOfDataMembers() needs to be
    //called in actual class
    void Branches(TTree* tree) final{
      BaseOutEvent::Branches(tree,Class()->GetListOfDataMembers());
    }
    void Hipo(hipo::ntuple_writer* writer) final{
      BaseOutEvent::Hipo(writer,Class()->GetListOfDataMembers());
    }

    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" is used to define ouput data for each particle "<<std::endl;
    }

  private :
    /////////////////////////////////////////////////
    /// data member for tree branches below here
    /// Experiment developers add your own data members
    /// if you want them in the tree
 

     
    ClassDefOverride(USERNAME::templateClass,1);

  };

  ///////////////////////////////////////////////////////////////
  inline void templateClass::FillData(){
    
    //move the c12p object to this particle for getting detector data
    auto c12p=_part->CLAS12();
 
    
    /* switch(c12p->getRegion()) { */
      
    /* case clas12::FD : */
    /*     break; */
    /* case clas12::FT : */
    /*   break; */
    /* case clas12::CD : */
    /*    break; */
    /* } */
   
  }
  /////////////////////////////////////////////////
}
