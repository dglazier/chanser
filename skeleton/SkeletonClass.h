//////////////////////////////////////////////////////////////
///
///Class:		SkeletonClass
///Description:
///           Classes for producing skeleton action classes

#pragma once

#include "Skeleton.h"

namespace chanser{
  
  class SkeletonClass   : public Skeleton {
    
    
  public :
    
  SkeletonClass(TString cls,TString temp):_className{cls},_templateName{temp}{}
    virtual ~SkeletonClass()=default;
    
    void MakeCode() final;
    
 

  protected:
    TString _className;
    TString _templateName;

  };
}

