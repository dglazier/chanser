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


    HipoProcessor()=default;
    HipoProcessor(clas12root::HipoChain *chain,TString fsfile,TString base) ;
    virtual ~HipoProcessor()=default;

    void    Begin(TTree *tree) final;
    void    SlaveBegin(TTree *tree) final;
      
    Bool_t ProcessEvent() final;
      
    void    SlaveTerminate() final;
    void    Terminate() final;

    Bool_t Notify() override;


    void GetFinalStates(TString fsfile);
      
    FinalStateManager& Manager(){return _fsm;}

    void AddOption(TString opt,TString val){_options->Add(new TNamed(opt,val));};
    
  protected:
    
    void    MergeFinalOutput();
    void    ApplyOptions();
    
  private:

    //Data members should be objects
    //or unique_ptrs
    //Don't stream i.e. use //!
    //members will be initialised on workers
    //Use unique_ptrs for members that are given to fInputList
    //but pass fInputList the RAW ptr
    //on the worker just get and use the RAW ptr locally
    //i.e. do not make it a data member

    FinalStateManager _fsm; //!
    HipoData _hipo;//!

    //Note the Unique_ptrs should not be used on workers
    //i.e. only in contructor, Begin, Terminate
    std::unique_ptr<TList> _listOfFinalStates={nullptr};//!
    std::unique_ptr<TList> _options={nullptr};//!
    std::unique_ptr<TNamed> _baseDir={nullptr};//!

    Bool_t _boss{kFALSE};//!
    Bool_t _FSNotInitialsed{kTRUE};//!
 
    ClassDefOverride(chanser::HipoProcessor,0);

  };

}

