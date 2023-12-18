{
  auto useEBPidFor = "NONE"; //or "NONE"
  auto useInclusiveFilterFor = "ALL";
  
  auto FS = username::Pi2::Make(useEBPidFor,useInclusiveFilterFor);
  
  FS->AddTopology("Electron:Proton:Pip:Pim");
  FS->AddTopology("Electron:Proton:Pim");
  FS->AddTopology("Electron:Proton:Pip");
  FS->AddTopology("Electron:Pip:Pim");
  
  //FS->MaskParticles(new MaskRadPhotons(10,0.5,1,kTRUE) );//currently only works with inclusive =="ALL"
  //  FS->MaskParticles(new MaskCalorSplitOffs(50,50,50,1) );//currently only works with inclusive =="ALL"
  
  ////Save TreeDataK2
  FS->UseOutputRootTree();
  //FS->UseOutputHipoNtuple();


   ///StartTime
  StartTimeAction st("StartTime",new C12StartTimeFromParticle("Electron"));
  FS->RegisterPreTopoAction(st);  //PRETOPO

  //Truth Matching, before ParticleData so can add to that tree
  //  EventTruthAction etra("EventTruth");
  //  FS->RegisterPostKinAction(etra); //PostKin

  MCMatchAction mcmatch("MCMatch");
  FS->RegisterPostKinAction(mcmatch); //PostKin
 
  //  Make particle trees first in case want to add cut flags
  ParticleDataManager pdm{"particle",1};
  pdm.SetParticleOut(new CLAS12ParticleOutEvent0);
  //pdm.SetParticleOut("e-",new MVA_El_Data);
  FS->RegisterPostKinAction(pdm);

  
   ////
  
  ParticleCutsManager pcm2{"DTCuts2",1};
  pcm2.AddParticleCut("e-",new EventBuilderCut); //Use EB for elecron ID
  //pcm2.AddParticleCut("e-",new  DeltaTimeVerCut(2));
  pcm2.AddParticleCut("proton",new DeltaTimeVerCut(2));
  pcm2.AddParticleCut("pi+",new DeltaTimeVerCut(2));
  pcm2.AddParticleCut("pi-",new DeltaTimeVerCut(2));
  FS->RegisterPostTopoAction(pcm2);
  ParticleCutsManager pcm1{"DTCuts1",0};
  pcm1.AddParticleCut("e-",new DeltaTimeVerCut(1));
  pcm1.AddParticleCut("proton",new DeltaTimeVerCut(1));
  pcm1.AddParticleCut("pi+",new DeltaTimeVerCut(1));
  pcm1.AddParticleCut("pi-",new DeltaTimeVerCut(1));
  FS->RegisterPostTopoAction(pcm1);
  ParticleCutsManager pcm05{"DTCuts05",0};
  pcm05.AddParticleCut("e-",new DeltaTimeVerCut(0.5));
  pcm05.AddParticleCut("proton",new DeltaTimeVerCut(0.5));
  pcm05.AddParticleCut("pi+",new DeltaTimeVerCut(0.5));
  pcm05.AddParticleCut("pi-",new DeltaTimeVerCut(0.5));
  FS->RegisterPostTopoAction(pcm05);
  ParticleCutsManager pcmEB{"EBCuts",0};
  pcmEB.AddParticleCut("e-",new EventBuilderCut);
  pcmEB.AddParticleCut("proton",new EventBuilderCut);
  pcmEB.AddParticleCut("pi+",new EventBuilderCut);
  pcmEB.AddParticleCut("pi-",new EventBuilderCut);
  FS->RegisterPostTopoAction(pcmEB);
  
 
 
  //Trigger
  TriggerAction ta("Trigger",{"TrigElect","TrigMeson","TrigFTFDCD","TrigFTAll","TrigMuon1","TrigMuon2","TrigMuon3","TrigMuon4","TrigMuon5","TrigMuon6"});
  FS->RegisterPostTopoAction(ta);  //PRETOPO

  //Write the particle momentum components
  WriteMomentumAction mom("Momentum",{"Electron","Proton","Pip","Pim"});
  FS->RegisterPostKinAction(mom);

  //e- energy correction
  ParticleCorrectionManager pcorrm{"FTelEnergyCorrection"};
  pcorrm.AddParticle("e-",new FTel_pol4_ECorrection());
  FS->RegisterPreTopoAction(pcorrm); //PRETOPO
  ParticleCorrectionManager pVz{"FTelVz"};//1=> for simulation too
  pVz.AddParticle("e-",new FTel_VzCorrection());
  FS->RegisterPreTopoAction(pVz); //PRETOPO

  RGA(FS.get());
  
  ////Write to file for later processing
  FS->WriteToFile("Pi2_NONE_ALL.root");
  
  //FS->Print();
  //Delete the final state rather than let ROOT try
  FS.reset();
}
