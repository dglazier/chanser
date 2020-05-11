{
  auto FS = someone::Pi4::Make("NONE","ALL");
  FS->AddTopology("Electron:Proton:Pip1:Pip2:Pim1:Pim2");
  FS->AddTopology("Electron:Proton:Pip1:Pip2:Pim1");
  // FS->AddTopology(OTHER_TOPOLOGY);
  
  ////Save TreeDataPi4
  FS->UseOutputRootTree();
  //FS->UseOutputHipoNtuple();

  ///StartTime
  StartTimeAction st("EBStartTime",new C12StartTimeFromParticle("Electron"));
  FS->RegisterPreTopoAction(st);
  
  /////Make particle trees first in case want to add cut flags
  ParticleDataManager pdm{"particle",1};
  CLAS12ParticleOutEvent0 pout;
  pdm.SetParticleOut(pout);
  FS->RegisterPostKinAction(pdm);

  ////
  ParticleCutsManager pcm{"DeltaTimeCuts",1};
  DeltaTimeCut dtc(2); //Apply a Delta Time cut of 2ns
  pcm.AddParticleCut("e-",dtc);
  pcm.AddParticleCut("proton",dtc);
  pcm.AddParticleCut("pi+",dtc);
  pcm.AddParticleCut("pi-",dtc);
  FS->RegisterPostTopoAction(pcm);
 
  FS->WriteToFile("NONEALL_configuration1.root");
  FS->Print();
  //Delete the final state rather than let ROOT try
  FS.reset();
}
