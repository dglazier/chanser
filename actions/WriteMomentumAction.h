//////////////////////////////////////////////////////////////
///
///Class:		TemplateAction
///Description:
///

#pragma once

#include "ActionManager.h"
#include "CLAS12Base.h"

namespace chanser{
    
  class WriteMomentumAction : public ActionManager {

  public:
    WriteMomentumAction()=default;
    ///Must give an output directory for saving trees to
    WriteMomentumAction(TString name,std::vector<string> pnames):ActionManager(name), _names{pnames} {
      _nParts = pnames.size();
      };
    
    virtual ~WriteMomentumAction()=default;
    WriteMomentumAction(const WriteMomentumAction& other) = default; //Copy Constructor
    WriteMomentumAction(WriteMomentumAction&& other) = default; //Move Constructor
      
    WriteMomentumAction& operator=(const WriteMomentumAction& other)=default;
    WriteMomentumAction& operator=(WriteMomentumAction&& other)=default;

    void PrintAction() override;
    Bool_t Execute(UInt_t ti)  override{
      //this is topology independent
      for(auto ib=0;ib<_nParts;++ib){
      	_pMag[ib] = _particles[ib]->P4p()->P();
      	_pTheta[ib] = _particles[ib]->P4p()->Theta();
      	_pPhi[ib] = _particles[ib]->P4p()->Phi();
      }
      return kTRUE;
    }
      
  
    void Configure(FinalState* fs) override;


  private:
    std::vector<Double_t> _pMag;//!
    std::vector<Double_t> _pTheta;//!
    std::vector<Double_t> _pPhi;//!
    
   
    std::vector<string> _names; //Names of triggers
    std::vector<BaseParticle*> _particles; //!pointers to particles
    UInt_t _nParts=0; //

    
    ClassDefOverride(chanser::WriteMomentumAction,1);
  };

}