{
  auto FS = someone::Pi4::Make("NONE","ALL");
  FS->AddTopology("Electron:Proton:Pip1:Pip2:Pim1:Pim2");
  FS->AddTopology("Electron:Proton:Pip1:Pip2:Pim1");
  // FS->AddTopology(OTHER_TOPOLOGY);
  
  ////Save TreeDataPi4
  FS->UseOutputRootTree();
  //FS->UseOutputHipoNtuple();

  ///StartTime
  StartTimeAction st("StartTime",new C12StartTimeFromParticle("Electron"));
  FS->RegisterPreTopoAction(st);  //PRETOPO
  
  /////Make particle trees first in case want to add cut flags
  ParticleDataManager pdm{"particle",1};
  ///pdm.SetParticleOut(new MyParticleOutEvent);
  pdm.SetParticleOut(new CLAS12ParticleOutEvent0);
  FS->RegisterPostKinAction(pdm);

  ////Cuts
  ParticleCutsManager pcm{"DeltaTimeCuts",1};
  pcm.AddParticleCut("e-",new MyParticleCut(12,1));
  DeltaTimeCut* dtc=new DeltaTimeCut(2); //Apply a Delta Time cut of 2ns
  // pcm.SetDefaultCut(new DeltaTimeCut(2));
  // pcm.AddParticleCut("e-",new DeltaTimeCut(2));
  pcm.AddParticleCut("proton",new DeltaTimeCut(1));
  pcm.AddParticleCut("pi+",new DeltaTimeCut(1));
  pcm.AddParticleCut("pi-",new DeltaTimeCut(1));
   
  FS->RegisterPostTopoAction(pcm); //POSTTOPO


  ////Corrections
  ParticleCorrectionManager pcorrm{"FTelEnergyCorrection"};
  pcorrm.AddParticle("e-",new FTel_pol4_ECorrection());
  // pcorrm.AddParticle("pi+",new MyParticleCorrection(1));
  FS->RegisterPreTopoAction(pcorrm); //PRETOPO
 
  FS->WriteToFile("NONEALL_configuration1.root");
  FS->Print();
  //Delete the final state rather than let ROOT try
  FS.reset();
}

