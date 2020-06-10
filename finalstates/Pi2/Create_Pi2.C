{
  auto FS = dglazier::Pi2::Make("NONE","ALL");
  FS->AddTopology("Electron:Proton:Pip:Pim");
  

  ////Save TreeDataK2
  FS->UseOutputRootTree();
  //FS->UseOutputHipoNtuple();


  //Truth Matching, before ParticleData so can add to that tree
  EventTruthAction etra("EventTruth");
  //FS->RegisterPostKinAction(etra); //PostKin
  
  //  Make particle trees first in case want to add cut flags
  ParticleDataManager pdm{"particle",1};
  pdm.SetParticleOut(new MyParticleOutEvent);
  pdm.SetParticleOut("e-",new MVA_El_Data);
  FS->RegisterPostKinAction(pdm);

  MVASignalIDAction mva_mlp{"mva1","MLP","Electron:Proton:Pip:Pim"};
  mva_mlp.SetParticleOut(new MyParticleOutEvent);
  mva_mlp.SetParticleOut("e-",new MVA_El_Data);
  FS->RegisterPostTopoAction(mva_mlp);
  MVASignalIDAction mva_bdt;
  mva_bdt.AddTopology("mva1","BDT","Electron:Proton:Pip:Pim");
  mva_bdt.SetParticleOut(new MyParticleOutEvent);
  mva_bdt.SetParticleOut("e-",new MVA_El_Data);
  FS->RegisterPostTopoAction(mva_bdt);
  
   ////
  ParticleCutsManager pcm2{"DTCuts2",1};
  //  pcm2.AddParticleCut("e-",new EventBuilderCut);
  pcm2.AddParticleCut("e-",new  DeltaTimeCut(2));
  pcm2.AddParticleCut("proton",new DeltaTimeCut(2));
  pcm2.AddParticleCut("pi+",new DeltaTimeCut(2));
  pcm2.AddParticleCut("pi-",new DeltaTimeCut(2));
  FS->RegisterPostTopoAction(pcm2);
  ParticleCutsManager pcm1{"DTCuts1",0};
  pcm1.AddParticleCut("e-",new DeltaTimeCut(1));
  pcm1.AddParticleCut("proton",new DeltaTimeCut(1));
  pcm1.AddParticleCut("pi+",new DeltaTimeCut(1));
  pcm1.AddParticleCut("pi-",new DeltaTimeCut(1));
  FS->RegisterPostTopoAction(pcm1);
  ParticleCutsManager pcm05{"DTCuts05",0};
  pcm05.AddParticleCut("e-",new DeltaTimeCut(0.5));
  pcm05.AddParticleCut("proton",new DeltaTimeCut(0.5));
  pcm05.AddParticleCut("pi+",new DeltaTimeCut(0.5));
  pcm05.AddParticleCut("pi-",new DeltaTimeCut(0.5));
  FS->RegisterPostTopoAction(pcm05);
  ParticleCutsManager pcmEB{"EBCuts",0};
  pcmEB.AddParticleCut("e-",new EventBuilderCut);
  pcmEB.AddParticleCut("proton",new EventBuilderCut);
  pcmEB.AddParticleCut("pi+",new EventBuilderCut);
  pcmEB.AddParticleCut("pi-",new EventBuilderCut);
  FS->RegisterPostTopoAction(pcmEB);
 
 
  ///StartTime
  StartTimeAction st("StartTime",new C12StartTimeFromParticle("Electron"));
  FS->RegisterPreTopoAction(st);  //PRETOPO


  //e- energy correction
  ParticleCorrectionManager pcorrm{"FTelEnergyCorrection"};
  pcorrm.AddParticle("e-",new FTel_pol4_ECorrection());
  FS->RegisterPreTopoAction(pcorrm); //PRETOPO

  ////Write to file for later processing
  FS->WriteToFile("Pi2_NONE_ALL.root");

  FS->Print();
  //Delete the final state rather than let ROOT try
  FS.reset();
}
