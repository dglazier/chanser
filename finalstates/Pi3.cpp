  
#include "Pi3.h"
 
    
    void Pi3::Define(){
      //Set final state detected particles
      //Note if particle is added to final with a valid genID it will be used
      //to determine the correct permutation of the simulated event
      
      //AddParticle("Name",particle,true/false you want to write in final vector, genID for linking to generated truth value)
      AddParticle("Electron",&_Electron,kTRUE,-1);
      AddParticle("Pip1",&_Pip1,kTRUE,-1);
      AddParticle("Pip2",&_Pip2,kTRUE,-1);
      AddParticle("Pim",&_Pim,kTRUE,-1);

      _linkToTopo["Electron:Pip1:Pip2:Pim"]=[&](){
	auto miss= _beam + _target - _Electron.P4()  -_Pip1.P4()-_Pip2.P4() -_Pim.P4();
    	TD.MissMass=miss.M();
	TD.MissMass2=miss.M2();
	
      };

  
      
    }

   
 
    void Pi3::Kinematics(){
      
   }
    
    void Pi3::UserProcess(){
 
      
      _counter++;

      //Fill at the end
     FinalState::UserProcess();

    }
  
  
