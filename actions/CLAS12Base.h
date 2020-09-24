//////////////////////////////////////////////////////////////
///
///Class:		CLAS12Base
///Description:    Can be used to interface the CLAS12FinalState
///                via inhertiance or multiple inheritance of this class


#pragma once


#include "CLAS12FinalState.h"

namespace chanser{
  class CLAS12Base {

  public:
    virtual ~CLAS12Base()=default;
  
    virtual void SetC12(CLAS12FinalState* c12){_finalstate = c12;}

    CLAS12FinalState* GetFinalState()const noexcept {return _finalstate;}
  
  private :

    CLAS12FinalState* _finalstate={nullptr};

  };

}
