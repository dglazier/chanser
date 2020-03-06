  
#include "templateFS.h"
 
namespace USERNAME{
  
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void templateFS::Define(){
    //Set final state detected particles
    //Note if particle is added to final with a valid genID it will be used
    //to determine the correct permutation of the simulated event
      
    //AddParticle("Name",particle,true/false you want to write in final vector, genID for linking to generated truth value)
    AddParticle("PARTICLE",&_PARTICLE,kTRUE,-1);

    _linkToTopo["TOPOLOGY"]=[&](){
      //TOPOLOGY Define you topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
	
    
      ///////------------------------------------///////
    };

  
      
  }

 
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void templateFS::Kinematics(){
    //Define reaction specific kinematic calculations here
    //Assign to tree data TD.var=

    //Use Kinematics to calculate electron variables
    _kinCalc.SetElecsTarget(_beam,_Electron.P4(),_target);
    TD.W=_kinCalc.W(); //photon bem energy
    TD.Q2=_kinCalc.Q2();
    TD.Pol=_kinCalc.GammaPol();
    TD.Egamma=_kinCalc.GammaE();
    
  }
    
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void templateFS::UserProcess(){
    //Optional additional steps
    //This is called after the Topo function
    //and before the kinematics function
    _counter++;


    //Must call the following to fill Final trees etc.
    //Fill Final data output at the end
    FinalState::UserProcess();

  }
  
  
}
