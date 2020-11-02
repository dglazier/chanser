#include "ZVertexCut.h"

namespace chanser{

 void ZVertexCut::ChangeRun()
  {
 /*
   * Recommended cut on the electron z-vertex position.
   * -18, 10 for outbending -ve torus
   * -13, 12 for inbending +ve torus
   */
    auto fieldVal =  GetCLAS12()->runconfig()->getTorus();
    if(fieldVal>0){
      _cutValLow=-13.;
      _cutValHigh=12.;
    }
    else{
      _cutValLow=-18.;
      _cutValHigh=10.;
    }

    
  }

}
