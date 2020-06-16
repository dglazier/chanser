{
  auto FS = dglazier::Pi2::Make("NONE","ALL");
  FS->AddTopology("Electron:Proton:Pip:Pim");
  
  FS->SetStartTimePeak(43.9);

 //  Make particle trees first in case want to add cut flags
  ParticleDataManager pdm{"particle",1};
  pdm.SetParticleOut(new MVAParticleOutEvent);
  FS->RegisterPostKinAction(pdm);

  MVASignalIDAction mva_bdt;
  mva_bdt.AddTopology("mva2","BDT","Electron:Proton:Pip:Pim");
  mva_bdt.SetParticleOut(new MVAParticleOutEvent);
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
  //  pcm2.AddParticleCut("e-",new EventBuilderCut);
  pcm1.AddParticleCut("e-",new  DeltaTimeCut(1));
  pcm1.AddParticleCut("proton",new DeltaTimeCut(1));
  pcm1.AddParticleCut("pi+",new DeltaTimeCut(1));
  pcm1.AddParticleCut("pi-",new DeltaTimeCut(1));
  FS->RegisterPostTopoAction(pcm1);

   //Truth Matching, before ParticleData so can add to that tree
  EventTruthAction etra("EventTruth");
  FS->RegisterPostKinAction(etra); //PostKin
  
 
 
  ///StartTime
  StartTimeAction st("StartTime",new C12StartTimeFromParticle("Electron"));
  FS->RegisterPreTopoAction(st);  //PRETOPO


  //e- energy correction
  ParticleCorrectionManager pcorrm{"FTelEnergyCorrection"};
  pcorrm.AddParticle("e-",new FTel_pol4_ECorrection());
  FS->RegisterPreTopoAction(pcorrm); //PRETOPO

  ////Write to file for later processing
  FS->WriteToFile("Pi2_BDT.root");
  FS->Print();
  //Delete the final state rather than let ROOT try
  FS.reset();

  //Run the data
  HipoData hdata;
  hdata.SetFile("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
  FinalStateManager fsm;
  fsm.SetBaseOutDir("/work/dump/mvabdt");
  fsm.LoadData(&hdata);
  fsm.LoadFinalState("Pi2",
		     "Pi2_BDT.root");//from WriteToFile line

  
  fsm.ProcessAll();
  
}
