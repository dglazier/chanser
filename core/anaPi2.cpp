  
#include "anaPi2.h"
 
    
    void anaPi2::Define(){
      //Set final state detected particles
      //Note if particle is added to final with a valid genID it will be used
      //to determine the correct permutation of the simulated event
      
      //AddParticle("Name",particle,true/false you want to write in final vector, genID for linking to generated truth value)
      AddParticle("Electron",&_Electron,kTRUE,3);
      AddParticle("Proton",&_Proton,kTRUE,2);
      AddParticle("Pip",&_Pip,kTRUE,1);
      AddParticle("Pim",&_Pim,kTRUE,0);

      //Topo dependent function 0
      _linkToTopo["Electron:Proton:Pip:Pim"]=[&](){
	
      	auto _miss= _beam + _target - _Electron.P4() -_Proton.P4() -_Pip.P4() -_Pim.P4();
	TD.MissMass=_miss.M();
	TD.MissMass2=_miss.M2();

      };

      //Topo dependent function 1
      _linkToTopo["Electron:Proton:Pip"]=[&](){
	
      	auto _miss= _beam + _target -_Electron.P4() -_Proton.P4() -_Pip.P4();
	TD.MissMass=_miss.M();
	TD.MissMass2=_miss.M2();
	//note set pi- as missing particle
	_Pim.FixP4(_miss);
      };

  
      
    }

    void anaPi2::OutTree(TTree* tree) {
      //Variables held in the base HS::FinalState class
      FinalState::OutTree(tree);
      //make branches with TreeData object
      TD.Branches(tree);
    }
    void anaPi2::OutHipo(hipo::ntuple_writer* writer){
      //Variables held in the base HS::FinalState class
      FinalState::OutHipo(writer);
      //make bank from TreeData
      TD.Hipo(writer);    
       
    }
 
    void anaPi2::Kinematics(){
      
      auto _meson=_Pip.P4()+_Pim.P4();
      TD.MesonMass=_meson.M();
       _kinCalc.SetElecsTarget(_beam,_Electron.P4(),_target);
      TD.W=_kinCalc.W(); //photon bem energy
      TD.Q2=_kinCalc.Q2();
      TD.Pol=_kinCalc.GammaPol();
      TD.Egamma=_kinCalc.GammaE();
  
    //calculate CM production kinematics for meson
      _kinCalc.SetMesonBaryon(_meson,_Proton.P4());
      _kinCalc.ElectroCMDecay();
      TD.MesonCosTh=_kinCalc.CosTheta();//prefix all variables to be saved wiht TM.
      TD.MesonPhi=_kinCalc.Phi();
}
    
    void anaPi2::UserProcess(){
 
      
      _counter++;

      //Fill at the end
     FinalState::UserProcess();

    }
  
  
