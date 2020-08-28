  
#include "KLambda.h"
 
namespace dglazier{
  
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void KLambda::Define(){
    //Set final state detected particles
    //Note if particle is added to final with a valid genID it will be used
    //to determine the correct permutation of the simulated event
      
    AddParticle("Electron",&_electron,kTRUE,-1);
    AddParticle("Proton",&_proton,kTRUE,-1);
    AddParticle("Pim",&_pim,kTRUE,-1);
    AddParticle("Kp",&_kp,kTRUE,-1);
    //AddParticle("Name",particle,true/false you want to write in final vector, genID for linking to generated truth value)
    // AddParticle("PARTICLE",&_PARTICLE,kTRUE,-1);

    //Set final state parents
    AddParticle("Lambda",&_lambda,kTRUE,-1);
    ConfigParent(&_lambda,&_proton);
    ConfigParent(&_lambda,&_pim);


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
    _doToTopo["Electron:Proton:Pim:Kp"]=[&](){
      //TOPOLOGY Define your topology dedendent code in here
      ///////+++++++++++++++++++++++++++++++++++///////
      if(!(_kp.CLAS12()->getP()<2.0&&_kp.CLAS12()->getRegion()==clas12::FD))
	{RejectEvent();return;}
      
      auto p4lambda = _proton.P4() + _pim.P4();
      TD->Massppim=p4lambda.M();
      _lambda.FixP4(p4lambda);
      auto miss= _beam + _target - _electron.P4() - _kp.P4() - p4lambda;
      TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();
      TD->MissP=miss.P();
      
      //FT electron cut
      if(_electron.CLAS12()->getRegion()==clas12::FT){
	TD->Cut=(TMath::Abs(TD->MissMass2)<0.05 && TD->MissP<2&&_electron.P4().P()>0.7);
      }
     //FD electron cut
      if(_electron.CLAS12()->getRegion()==clas12::FD){
	TD->Cut=(TMath::Abs(TD->MissMass2)<0.05 && TD->MissP<0.1);
      }
     
      //Now miss Lambda
      miss= _beam + _target - _electron.P4() - _kp.P4();
      TD->MissMasseKp=miss.M();
      ///////------------------------------------///////
    };
      
  }

 
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void KLambda::Kinematics(){
    //Define reaction specific kinematic calculations here
    //Assign to tree data TD.var=

    //Use Kinematics to calculate electron variables
    //Note this assumes you called your electron "electron" or "Electron"
    // _kinCalc.SetElecsTarget(_beam,_electron.P4(),_target);
    // TD->W=_kinCalc.W(); //photon bem energy
    // TD->Q2=_kinCalc.Q2();
    // TD->Pol=_kinCalc.GammaPol();
    // TD->Egamma=_kinCalc.GammaE();
    
  }
    
  ///////////////////////$$$$$$$$$$$$$$$$$$$$$$$$$$//////////////////////  
  void KLambda::UserProcess(){
    //Optional additional steps
    //This is called after the Topo function
    //and before the kinematics function
    _counter++;


    //Must call the following to fill Final trees etc.
    //Fill Final data output at the end
    FinalState::UserProcess();

  }
  
  
}
