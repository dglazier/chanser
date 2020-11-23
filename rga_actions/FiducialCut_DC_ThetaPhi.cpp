#include "FiducialCut_DC_ThetaPhi.h"
#include <TDatabasePDG.h>

namespace chanser{
  
  void FiducialCut_DC_ThetaPhi::ChangeRun()
  {
    if(GetCLAS12()==nullptr) return;

    _applicable= kTRUE;

    //parameters depend on run period and torus field setting
    auto period = GetRunInfo()->_runPeriod+"_" + GetRunInfo()->_fieldSetting;

    //Get maximum parameters
    auto table = GetAnaDB().GetTable(period,
				     "FIDUCIALCUT_DC_THETAPHI=MAX_"
				     +_pname,{_NSectors,_NLayers,_NCoefs});
    if(table.IsValid())
      table.Fill(_maxparams);
    else _applicable=kFALSE; //no valid paramters so cut returns true

    //Get minimum parameters
    table = GetAnaDB().GetTable(period,
				"FIDUCIALCUT_DC_THETAPHI=MIN_"
				+_pname,{_NSectors,_NLayers,_NCoefs});
    if(table.IsValid())
      table.Fill(_minparams);
    else _applicable=kFALSE; //no valid paramters so cut returns true

    if(_applicable==kFALSE){
      std::cerr<<"FiducialCut_DC_THETAPHI::ChangeRun() will not be applied to   "<<_pname<<" for run period "<<period<<endl;
    }

  }

  FiducialCut_DC_ThetaPhi::FiducialCut_DC_ThetaPhi(TString pid)
  {
    //initialse sizes of paramter tables
    _minparams = tablevals3D_t(_NSectors,tablevals2D_t(_NLayers,tablevals_t(_NCoefs)) );
    _maxparams = tablevals3D_t(_NSectors,tablevals2D_t(_NLayers,tablevals_t(_NCoefs)) );

    auto pdg=TDatabasePDG::Instance()->GetParticle(pid)->PdgCode();
    if(pdg == 11) {
      _partPidVal=0;
      _pname="ELECTRON";
    }
    else if (pdg==2212){ _partPidVal=1; _pname="PROTON";}
    else if (pdg==211) {_partPidVal= 2; _pname="PI+";}
    else if (pdg==-211){ _partPidVal = 3; _pname="PI-";}
    else if (pdg==321) {_partPidVal=4; _pname="K+";}
    else if (pdg==-321){ _partPidVal=5; _pname="K-";}
    else{
      std::cerr<<"FiducialCut_DC_ThetaPhi, invalid layer PID "<<pid<<" "<<pdg<<std::endl;
      exit(0);
    }          

  }
}
