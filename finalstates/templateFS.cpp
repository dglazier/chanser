  
#include "templateFS.h"
 
    
    void templateFS::Define(){
      //Set final state detected particles
      //Note if particle is added to final with a valid genID it will be used
      //to determine the correct permutation of the simulated event
      
      //AddParticle("Name",particle,true/false you want to write in final vector, genID for linking to generated truth value)
      AddParticle("PARTICLE",&_PARTICLE,kTRUE,-1);

      _linkToTopo["TOPOLOGY"]=[&](){
	
    
      };

  
      
    }

 
    void templateFS::Kinematics(){
      
   }
    
    void templateFS::UserProcess(){
 
      
      _counter++;

      //Fill at the end
     FinalState::UserProcess();

    }
  
  
