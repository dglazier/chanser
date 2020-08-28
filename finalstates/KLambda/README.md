Example of creating clas12 KLambda analysis

### 1) Specify the particles you would like in the final state

Create Skeleton code maker Skeleton.C (see main README for details)

       {
	   //skeleton code maker for exclusive K+Lambda
       
           FSSkeleton s;
 	   s.SetFinalState("KLambda");
  	   s.SetFinalStateParts("Electron:e-,Proton:proton,Pim:pi-,Kp:K+");
  	   s.SetFinalStateTopo("Electron:Proton:Pim:Kp");
  	   s.SetFinalStateParents("Lambda:Lambda0;Proton;Pim");
  	   s.MakeCode();
	}

Create code

        chanser_skeleton Skeleton.C
	ls
	Create_KLambda.C  KLambda.cpp  KLambda.h  Run_KLambda.C  Skeleton.C  TreeDataKLambda.h

### 2) Code the calculations you would like with these particles in KLambda.cpp Define() function

    _doToTopo["Electron:Proton:Pim:Kp"]=[&](){

      ///////+++++++++++++++++++++++++++++++++++///////
      if(!(_kp.CLAS12()->getP()<2.0&&_kp.CLAS12()->getRegion()))
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



### 3) Create configured analysis object

Note here we include the FT energy correction.

     {
        auto FS = dglazier::KLambda::Make("ALL","ALL");
  	FS->AddTopology("Electron:Proton:Pim:Kp");
  
	////Save TreeDataKLambda
     	FS->UseOutputRootTree();
  
	//e- energy correction
  	ParticleCorrectionManager pcorrm{"FTelEnergyCorrection"};
  	pcorrm.AddParticle("e-",new FTel_pol4_ECorrection());
  	FS->RegisterPreTopoAction(pcorrm); //PRETOPO

  	////Write to file for later processing
  	FS->WriteToFile("KLambda_ALL_ALL.root");
 	//Delete the final state rather than let ROOT try
  	FS.reset();
     }

To creat obeject and save to named ROOT file

      chanser_root KLambda:KLambda.cpp Create_KLambda.C

### 4) Run on PROOF

Create/Edit finalstate.txt

     KLambda /work/Dropbox/HaSpect/dev/chanser/finalstates/KLambda/KLambda_ALL_ALL.root

Create/Edit Processor.C

      {
  
	clas12root::HipoChain chain;
  	//chain.Add("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
  	chain.Add("/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v0/dst/train/skim14/skim14_0050*.hipo");
  	chain.Add("/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v0/dst/train/skim14/skim14_0051*.hipo");
  	chain.Add("/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v0/dst/train/skim14/skim14_0052*.hipo");
  	chain.Add("/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v0/dst/train/skim14/skim14_0053*.hipo");
  
	chain.SetReaderTags({0});
  	// chain.GetC12Reader()->useFTBased();
  	chain.GetC12Reader()->addAtLeastPid(11,1);
  	chain.GetC12Reader()->addExactPid(2212,1);
  	chain.GetC12Reader()->addExactPid(321,1);   
  	chain.GetC12Reader()->addExactPid(-211,1);

  	chanser::HipoProcessor processor(&chain,"finalstates.txt","/work/clas12/dglazier/chanser_out");
  	gProof->Process(&processor,chain.GetNRecords());
  
     }

Then to run on 4 cores

     chanser_proof 4 Processor.C

