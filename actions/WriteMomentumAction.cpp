#include "WriteMomentumAction.h"
#include "FinalState.h"

namespace chanser{
    
  void WriteMomentumAction::PrintAction(){
    std::cout<<" WriteMomentumAction::Print() "<<std::endl;
   
  }
  ///////////////////////////////////////////////////////////////
  void WriteMomentumAction::Configure(FinalState* fs){
    //only streaming names from root file
     
    auto tree=fs->FinalTree();

    _nParts = _names.size();

    _pMag.resize(_nParts);
    _pTheta.resize(_nParts);
    _pPhi.resize(_nParts);
    _pDeltaTime.resize(_nParts);
    _pRegion.resize(_nParts);
    _particles.resize(_nParts);
 
    for(int ib =0;ib<_names.size();++ib){ //loop over requested particles and make branches
      //store 4-vector
      _particles[ib]=(fs->GetParticle(_names[ib]));
    
      tree->Branch(TString(_names[ib])+"_P",&_pMag[ib],TString(_names[ib])+"_P" + "/D");
      tree->Branch(TString(_names[ib])+"_Th",&_pTheta[ib],TString(_names[ib])+"_Th" + "/D");
      tree->Branch(TString(_names[ib])+"_Ph",&_pPhi[ib],TString(_names[ib])+"_Ph" + "/D");
      tree->Branch(TString(_names[ib])+"_Dt",&_pDeltaTime[ib],TString(_names[ib])+"_Dt" + "/D");
      tree->Branch(TString(_names[ib])+"_Region",&_pRegion[ib],TString(_names[ib])+"_Region" + "/I");
      
    }
  }


}
