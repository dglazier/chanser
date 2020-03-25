//////////////////////////////////////////////////////////////
///
///Class:		HipoData
///Description:
///
#pragma once

#include "HipoData.h"


namespace chanser{

  class HipoTruth : public HipoData{

  public :
    HipoTruth()=default;
    virtual ~HipoTruth()=default;

    //DataManager
    Bool_t Init() override;
    virtual Bool_t InitEvent() override;
    Bool_t ReadEvent(Long64_t entry=-1)override;
      
       
  
  private :

   
  };
}
