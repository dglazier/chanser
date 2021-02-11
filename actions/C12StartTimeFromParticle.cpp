#include "C12StartTimeFromParticle.h"

namespace chanser{

  void C12StartTimeFromParticle::ChangeRun(){
   if(GetCLAS12()==nullptr) return;
    //get the target position, EBCCDBEnum.TARGET_POSITION
   auto ccdb=GetCLAS12()->ccdb();

   if(ccdb){
     //ccdb exist so use cached values
     _targetPosition=GetRunInfo()->_TargetCentre;
     _rfBucketLength=GetRunInfo()->_rfBucketLength;
   }

   /*
   //note target postion uses variations, so this will not return
   //correct target position. In any case target shift already applied to particle vertex
   //   _targetPosition=ccdb->requestTableValueFor(0,"position","/geometry/target");
   _targetPosition=-0.03; //for fall 2018, not easily available in ccdb.
   int rfStat1=ccdb->requestTableValueFor(0,"status","/calibration/eb/rf/config");
   int rfStat2=ccdb->requestTableValueFor(1,"status","/calibration/eb/rf/config");

   // There are two rows in rf/config here we find the one with status=1
   // if (rfStat1<=0 && rfStat2<=0)
   // throw new RuntimeException("Couldn't find non-positive RF status in CCDB");
   int rfId = rfStat2>rfStat1 ? 1 : 0;
   _rfBucketLength=ccdb->requestTableValueFor(rfId,"clock","/calibration/eb/rf/config");//EBCCDBEnum.RF_BUCKET_LENGTH
   //_rfOffset=ccdb->requestTableValueFor(rfId,"offset","/calibration/eb/rf/offset");//EBCCDBEnum.RF_OFFSET
   _rfOffset=0;
   */
   std::cout<<" Target position "<<_targetPosition<<std::endl;
   std::cout<<" Bucket Length "<<_rfBucketLength<<std::endl;
   std::cout<<" Offset "<<_rfOffset<<" is already applied this value is ignored"<<std::endl;
  }

}
 
