//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///
#pragma once

#include "BaseRunInfo.h"
#include "clas12reader.h"
#include <Rtypes.h>

namespace chanser{

  class RunInfo : public BaseRunInfo{

   public :
    
    void SetCLAS12(clas12::clas12reader* c12){_c12=c12;}
    clas12::clas12reader* CLAS12() const{return _c12;}
    
    Double_t _TargetCentre={0};
    Double_t _BeamEnergy={0};
    Double_t _rfBucketLength={0};

    Int_t _runNumber={0};
    
    std::string _dataType;
    std::string _runPeriod;
    std::string  _fieldSetting;
     
            
  private :
    
    clas12::clas12reader* _c12={nullptr};


  };//class RunInfo
}
