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

    const clas12::clas12reader* GetCLAS12()const noexcept {
     return  _finalstate!=nullptr ?  _finalstate->GetEventInfo()->CLAS12() : nullptr;
    }

    const clas12::ccdb_reader* CCDB()const noexcept {return GetCLAS12()->ccdb();}
    const clas12::rcdb_reader* RCDB()const noexcept {return GetCLAS12()->rcdb();}
    const clas12::qadb_reader* QADB()const noexcept {return GetCLAS12()->qadb();}

    const RunInfo* GetRunInfo()const {return _finalstate->GetRunInfo();}
    const EventInfo* GetEventInfo()const {return _finalstate->GetEventInfo();}
    const AnaDB& GetAnaDB() const {return GetRunInfo()->GetAnaDB();}
    
    virtual void ChangeRun(){};
    
  private :

    CLAS12FinalState* _finalstate={nullptr};

    
    ClassDef(chanser::CLAS12Base,1);

  };

}
