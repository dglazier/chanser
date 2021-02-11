#include "Cut_RefineElectronID.h"

namespace chanser{
  
  void Cut_RefineElectronID::ChangeRun()
  {
    if(GetCLAS12()==nullptr) return;

    _applicable = kTRUE;
    
    //parameters depend on run period and torus field setting
    auto period = GetRunInfo()->_runPeriod+"_" + GetRunInfo()->_fieldSetting;
 
    //get fit parameters, constants for the mean and sigma
    auto table = GetAnaDB().GetTable(period, "CUT_ELECTRON_SFRAC=MEAN", {3, 6});
    if(table.IsValid()){
      table.Fill(_fitparams_mean);
    } else {
      _applicable=kFALSE;
    } //no valid paramters so cut returns true

    table = GetAnaDB().GetTable(period, "CUT_ELECTRON_SFRAC=SIGMA", {3, 6});
    if(table.IsValid()){
      table.Fill(_fitparams_sigma);
    } else {
      _applicable=kFALSE;
    } //no valid paramters so cut returns true

    if(_applicable==kFALSE){
      std::cerr<<"Cut_RefineElectronID::ChangeRun() will not be applied to run period "<<period<<endl;
    }

  }
  Cut_RefineElectronID::Cut_RefineElectronID()
  {
    //initialse sizes of paramter tables
    _fitparams_mean = tablevals2D_t(3,tablevals_t(6));
    _fitparams_sigma = tablevals2D_t(3,tablevals_t(6));
  }  
}