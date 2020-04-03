  
#include "Pi4.h"
 
namespace someone{
  
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void Pi4::Define(){
    //Set final state detected particles
    //Note if particle is added to final with a valid genID it will be used
    //to determine the correct permutation of the simulated event
      
    AddParticle("Electron",&_electron,kTRUE,-1);
    AddParticle("Proton",&_proton,kTRUE,-1);
    AddParticle("Pip1",&_pip1,kTRUE,-1);
    AddParticle("Pip2",&_pip2,kTRUE,-1);
    AddParticle("Pim1",&_pim1,kTRUE,-1);
    AddParticle("Pim2",&_pim2,kTRUE,-1);
    //AddParticle("Name",particle,true/false you want to write in final vector, genID for linking to generated truth value)
    // AddParticle("PARTICLE",&_PARTICLE,kTRUE,-1);

    //Set final state parents
    AddParticle("K1",&_k1,kTRUE,-1);
    ConfigParent(&_k1,&_pip1);
    ConfigParent(&_k1,&_pim1);
    AddParticle("K2",&_k2,kTRUE,-1);
    ConfigParent(&_k2,&_pip2);
    ConfigParent(&_k2,&_pim2);


    //_doToTopo["TOPOLOGY"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
    ////auto miss= _beam + _target - _electron.P4() - _proton.P4()
    ////  -_pip1.P4()-_pim1.P4();
    ////TD.MissMass=miss.M();
    ////TD.MissMass2=miss.M2();
    
      ///////------------------------------------///////
    // };

    //Set Possible Topologies
    _doToTopo["Electron:Proton:Pip1:Pip2:Pim1:Pim2"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      // auto startime=StartTimeFromTimeAndVertex(_electron.DeltaTime());
      // //auto startime=StartTimeFromEB();
      // for(auto& p:CurrentTopo()->GetParticles())
      // 	p->ShiftTime(-startime);
      ///////+++++++++++++++++++++++++++++++++++///////
      auto miss= _beam + _target - _electron.P4() - _proton.P4()
      -_pip1.P4()-_pip2.P4() -_pim1.P4() -_pim2.P4();
      
      TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();

      _k1.FixP4(_pip1.P4()+_pim1.P4());
      _k2.FixP4(_pip2.P4()+_pim2.P4());

      ///////------------------------------------///////
    };
    _doToTopo["Electron:Proton:Pip1:Pip2:Pim1"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
      ///////+++++++++++++++++++++++++++++++++++///////
      auto miss= _beam + _target - _electron.P4() - _proton.P4()
      -_pip1.P4()-_pip2.P4() -_pim1.P4();
      
      TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();
      
      //note set pi- 2 as missing particle
      _pim2.FixP4(miss);
      
      _k1.FixP4(_pip1.P4()+_pim1.P4());
      _k2.FixP4(_pip2.P4()+_pim2.P4());
 
      ///////------------------------------------///////
    };
    _doToTopo["Electron:Proton:Pip1:Pim1"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
       ///////+++++++++++++++++++++++++++++++++++///////
      auto miss= _beam + _target - _electron.P4() - _proton.P4()
      -_pip1.P4() -_pim1.P4();
      
      TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();
  
      //note set K0 2 as missing particle
      _k2.FixP4(miss);
      
      
      _k1.FixP4(_pip1.P4()+_pim1.P4());

      ///////------------------------------------///////
    };
      
  }

 
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void Pi4::Kinematics(){
    //Define reaction specific kinematic calculations here
    //Assign to tree data TD.var=

    //Use Kinematics to calculate electron variables
    //Note this assumes you called your electron "electron" or "Electron"
    _kinCalc.SetElecsTarget(_beam,_electron.P4(),_target);
    TD->W=_kinCalc.W(); //photon bem energy
    TD->Q2=_kinCalc.Q2();
    TD->Pol=_kinCalc.GammaPol();
    TD->Egamma=_kinCalc.GammaE();

    //calculate meson Lorentz Vector
    auto meson = _k1.P4() + _k2.P4();
    TD->MesonMass = meson.M(); 
    
    //Caclulate X->2K0 decay angles
    _kinCalc.SetMesonBaryon(meson,_proton.P4());
    _kinCalc.SetMesonDecay(_k1.P4() , _k2.P4());
    _kinCalc.MesonDecayGJ();
    TD->MesonCosThGJ=_kinCalc.CosTheta();
    TD->MesonPhiGJ=_kinCalc.CosTheta();
  }
    
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void Pi4::UserProcess(){
    //Optional additional steps
    //This is called after the Topo function
    //and before the kinematics function
    _counter++;


    //Must call the following to fill Final trees etc.
    //Fill Final data output at the end
    FinalState::UserProcess();

  }
  
  
}
