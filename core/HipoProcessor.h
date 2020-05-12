//////////////////////////////////////////////////////////////
///
///Class:		HipoProcessor
///Description:
///
#pragma once

#include "FinalStateManager.h"
#include "HipoData.h"
#include "HipoSelector.h"


namespace chanser{

  class HipoProcessor : public clas12root::HipoSelector {
  public :


  HipoProcessor(): _listOfFinalStates(nullptr) {};
    // HipoProcessor(TString filename) ;
    HipoProcessor(clas12root::HipoChain *chain,TString fsfile,TString base) ;
    virtual ~HipoProcessor();
    void    Begin(TTree *tree) final;
    void    SlaveBegin(TTree *tree) final;
      
    Bool_t ProcessEvent() final;
      
    void    SlaveTerminate() final;
    void    Terminate() final;

    Bool_t Notify() override;


    void GetFinalStates(TString fsfile);
      
    FinalStateManager& Manager(){return _fsm;}

  private:
    FinalStateManager _fsm;
    HipoData _hipo;

    TList* _listOfFinalStates{nullptr};
    TString _baseDir;
    
    ClassDefOverride(chanser::HipoProcessor,0);

  };

}

