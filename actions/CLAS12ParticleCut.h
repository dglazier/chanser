//////////////////////////////////////////////////////////////
///
///Class:              CLAS12Particle		
///Description:        Base cut class for cuts that need data
///                    From CLAS12FinalState
///                    Inherit from this rather than BaseCut
///                    You may redefine CLAS12Base::SetC12(CLAS12FinalState* c12) function
///                    To get exaclty the infor you want
///                    Or just use CLAS12Base::GetFinalState() to get the final
///                    state pointer
#pragma once

#include "BaseCut.h"
#include "CLAS12Base.h"

namespace chanser{


  class CLAS12ParticleCut : public BaseCut , public CLAS12Base {
      
  public:
    CLAS12ParticleCut()=default; // must have default constructor
    
       
 
  private:
      
    ClassDefOverride(chanser::CLAS12ParticleCut,1);

  };//class CLAS12ParticleCut

}
