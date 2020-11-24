#include "Cut_ZVertex.h"

namespace chanser{

 void Cut_ZVertex::ChangeRun()
 {
   /*
    * Recommended cut on the electron z-vertex position.
    * -18, 10 for outbending -ve torus
    * -13, 12 for inbending +ve torus
    */
   auto period = GetRunInfo()->_runPeriod+"_" +GetRunInfo()->_fieldSetting;

   //Get maximum parameters
   auto table=GetAnaDB().GetTable(period,"CUT_ZVERTEX",{2});
   if(table.IsValid()){
     tablevals_t val(2);
     table.Fill(val);
     _cutValLow=val[0];
     _cutValHigh=val[1];
   }
    
 }

}
