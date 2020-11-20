//////////////////////////////////////////////////////////////
///
///Class:		BaseRunInfo
///Description:
///
#pragma once

#include "AnaDB.h"

namespace chanser{
    
  class BaseRunInfo{
      
  public :
    BaseRunInfo()=default;
    virtual ~BaseRunInfo()=default;


    const AnaDB& GetAnaDB() const {return _db;}
    void LoadAnaDB( const string& name){_db.LoadTopFile(name);}

  private:
      
    AnaDB _db;

    
  };
}
