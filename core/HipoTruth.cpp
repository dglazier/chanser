#include "HipoTruth.h"

namespace chanser{
    
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  ///Initialise clas12reader from hipo filename
  Bool_t HipoTruth::Init(){
    if(_c12->mcparts()) _dataType=static_cast<Short_t> (chanser::DataType::Lund);
    else Fatal("HipoTruth::Init()","No MC::Lund banks in hipo file");
    return kTRUE;
  }
  /////////////////////////////////////////////////////////////////////
  //Check if there is another event
  //Can now get Pid list for this event
  Bool_t HipoTruth::InitEvent(){
    //read all events
    if(_c12->getReader().next()){
      _entry++;
      return kTRUE;
    }
    return kFALSE; //all event done
  }
 
  /////////////////////////////////////////////////////////////////
  ///
  Bool_t HipoTruth::ReadEvent(Long64_t entry){
 
    //   _c12->readEvent();//only need mcparts read
    _c12->clearEvent();
    _c12->getStructure(_c12->mcparts());
    _c12->getStructure(_c12->mcevent());
    // _c12->getStructure(_c12->mcmatch());
    FillTruth();
    
    return kTRUE;
  }


}
