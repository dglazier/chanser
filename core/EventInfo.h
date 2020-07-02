//////////////////////////////////////////////////////////////
///
///Class:		EventInfo
///Description:
///

#pragma once

#include "BaseEventInfo.h"
#include "clas12reader.h"
#include <Rtypes.h>

namespace chanser{

    class EventInfo : public BaseEventInfo{

    public :
      // EventInfo()=default;
      //virtual ~EventInfo()=default;
  
      Long_t _VTPTrigBit{0}; //clas12 vtp trigger bit
      Long_t _TrigBit{0}; //clas12 trigger bit
      Float_t _StartTime{0};//EB defined starttime
      Float_t _FTBStartTime{0};//EB FT based defined starttime
      Float_t _RFTime{0};//RF Time (ns)
      Float_t _BeamPol{0}; //helicity polarisation of beam
      Float_t _TarPol{0}; //!polarisation of target
      Int_t _NEvent{0};//Event Number
      Short_t _BeamHel{0}; //helcity direction of beam
      Short_t _TarPolDir{0}; //!direction of target polarisation

      void SetCLAS12(clas12::clas12reader* c12){_c12=c12;}
      clas12::clas12reader* CLAS12() const{return _c12;}
      
    private :
      
      clas12::clas12reader* _c12{nullptr};
           
    };//class EventInfo
}
