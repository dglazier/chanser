#include "DataManager.h"
#include "FinalStateManager.h"


namespace chanser{

  void DataManager::Notify(){
    if(_fsmanager)_fsmanager->Notify();
  }

}

