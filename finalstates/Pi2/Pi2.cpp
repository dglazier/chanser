  
#include "Pi2.h"
 
namespace username{
  
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void Pi2::Define(){
    //Set final state detected particles
    //Note if particle is added to final with a valid genID it will be used
    //to determine the correct permutation of the simulated event
      
    AddParticle("Electron",&_electron,kTRUE,3);
    AddParticle("Proton",&_proton,kTRUE,2);
    AddParticle("Pip",&_pip,kTRUE,0);
    AddParticle("Pim",&_pim,kTRUE,1);
    //AddParticle("Name",particle,true/false you want to write in final vector, genID for linking to generated truth value)
    // AddParticle("PARTICLE",&_PARTICLE,kTRUE,-1);

    //Set final state parents


    //_doToTopo["TOPOLOGY"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
    ////auto miss= _beam + _target - _electron.P4() - _proton.P4()
    ////  -_pip1.P4()-_pim1.P4();
    ////TD->MissMass=miss.M();
    ////TD->MissMass2=miss.M2();
    
      ///////------------------------------------///////
    // };

    //Set Possible Topologies
    _doToTopo["Electron:Proton:Pip:Pim"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
      //should be nothing missing here
      auto miss= _beam + _target - _electron.P4() - _proton.P4() -_pip.P4()-_pim.P4();
      //potential cut on FT only electrons
      // if(_electron.CLAS12()->getRegion()!=1000){RejectEvent(); return;}

      TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();

      //loose missing mass cut
      if(TMath::Abs(TD->MissMass2)>1) {RejectEvent(); return;}
      
      TD->MissP=miss.P();
      TD->MissE=miss.E();
      miss= _beam + _target -_electron.P4() -_pip.P4() -_pim.P4();
      TD->MissMass2Pi=miss.M();
      miss	= _beam + _target -_electron.P4() -_pip.P4() -_proton.P4();
      TD->MissMassPPip=miss.M2();
      miss= _beam + _target -_electron.P4() -_pim.P4() -_proton.P4();
      TD->MissMassPPim=miss.M2();

      ///////------------------------------------///////
    };


    //Set Possible Topologies
    _doToTopo["Electron:Pip:Pim"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
      //miss is the proton
      auto miss= _beam + _target - _electron.P4() -_pip.P4()-_pim.P4();
      _proton.FixP4(miss);
      //potential cut on FT only electrons
      // if(_electron.CLAS12()->getRegion()!=1000){RejectEvent(); return;}

      TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();

       //loose missing mass cut
      if(TMath::Abs(TD->MissMass-Mprot)>1) {RejectEvent(); return;}
      
      TD->MissP=miss.P();
      TD->MissE=miss.E();
      TD->MissMass2Pi=miss.M();
      TD->MissMassPPip=0; //does not exist
      TD->MissMassPPim=0; //does not exist

    };
    //Set Possible Topologies
    _doToTopo["Electron:Proton:Pip"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
      //miss is the pi-
      auto miss= _beam + _target - _electron.P4() -_pip.P4()-_proton.P4();
      _pim.FixP4(miss);
      //potential cut on FT only electrons
      // if(_electron.CLAS12()->getRegion()!=1000){RejectEvent(); return;}

      TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();

       //loose missing mass cut
      if(TMath::Abs(TD->MissMass2)>1) {RejectEvent(); return;}
      
      TD->MissP=miss.P();
      TD->MissE=miss.E();
      TD->MissMass2Pi=0;//does not exist
      TD->MissMassPPip=miss.M(); 
      TD->MissMassPPim=0; //does not exist

    };
  //Set Possible Topologies
    _doToTopo["Electron:Proton:Pim"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
      //miss is the pi+
      auto miss= _beam + _target - _electron.P4() -_pim.P4()-_proton.P4();
      _pip.FixP4(miss);
      //potential cut on FT only electrons
      // if(_electron.CLAS12()->getRegion()!=1000){RejectEvent(); return;}

      TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();

       //loose missing mass cut
      if(TMath::Abs(TD->MissMass2)>1) {RejectEvent(); return;}
      
      TD->MissP=miss.P();
      TD->MissE=miss.E();
      TD->MissMass2Pi=0;//does not exist
      TD->MissMassPPip=0; //does not exist
      TD->MissMassPPim=miss.M();

    };


   }

 
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void Pi2::Kinematics(){
    //Define reaction specific kinematic calculations here
    //Assign to tree data TD.var=

    //Use Kinematics to calculate electron variables
    //Note this assumes you called your electron "electron" or "Electron"
    _kinCalc.SetElecsTarget(_beam,_electron.P4(),_target);
    TD->W=_kinCalc.W(); //photon bem energy
    TD->Q2=_kinCalc.Q2();
    TD->Pol=_kinCalc.GammaPol();
    TD->Egamma=_kinCalc.GammaE();
    
   //Calculate possible resonances
    HSLorentzVector pMeson=_pip.P4()+_pim.P4();
    HSLorentzVector pDpp=_proton.P4()+_pip.P4();
    HSLorentzVector pD0=_proton.P4()+_pim.P4();
    //invariant masses
    TD->MesonMass=pMeson.M();
    TD->DppMass=pDpp.M();
    TD->D0Mass=pD0.M();
 
    _kinCalc.SetMesonBaryon(pMeson,_proton.P4());
    _kinCalc.ElectroCMDecay();
    TD->ProdCosThGJ=_kinCalc.CosTheta();
    TD->ProdPhiGJ=_kinCalc.Phi();
 

    _kinCalc.SetMesonDecay(_pip.P4() , _pim.P4());
    _kinCalc.MesonDecayGJ();
    TD->MesonCosThGJ=_kinCalc.CosTheta();
    TD->MesonPhiGJ=_kinCalc.Phi();
    //  cout<<"TrigElect "<<GetEventInfo()->CLAS12()->checkTriggerBit()<<endl;
  }
    
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void Pi2::UserProcess(){
    //Optional additional steps
    //This is called after the Topo function
    //and before the kinematics function
    _counter++;


    //Must call the following to fill Final trees etc.
    //Fill Final data output at the end
    FinalState::UserProcess();

  }
  
  
}
