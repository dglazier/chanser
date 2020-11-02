#include "Cut_RefineElectronID.h"

namespace chanser{

  void Cut_RefineElectronID::ChangeRun(){

   if(GetCLAS12()==nullptr) return;
    //get the sampling fraction parameters
    _sfracPars=GetCLAS12()->ccdb()->requestTableDoubles("/calibration/eb/electron_sf");

  }

}
 
