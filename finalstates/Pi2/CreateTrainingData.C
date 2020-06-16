{
  auto FS = Pi2::Make("NONE","ALL");
  FS->AddTopology("Electron:Proton:Pip:Pim");
  

  FS->SetStartTimePeak(43.9);

  
  //Make particle trees first in case want to add cut flags
  ParticleDataManager pdm{"particle",1};
  pdm.SetParticleOut(new MVAParticleOutEvent);
  FS->RegisterPostKinAction(pdm);

  //// Define training data variables
  ParticleCutsManager pcm2{"DTCuts2",0};
  pcm2.AddParticleCut("e-",new EventBuilderCut); //Use EB for elecron ID
  pcm2.AddParticleCut("proton",new DeltaTimeCut(5));
  pcm2.AddParticleCut("pi+",new DeltaTimeCut(5));
  pcm2.AddParticleCut("pi-",new DeltaTimeCut(5));
  FS->RegisterPostTopoAction(pcm2);

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
  FS->WriteToFile("Pi2_Training.root");

  FS->Print();
  //Delete the final state rather than let ROOT try
  FS.reset();

  HipoData hdata;
  hdata.SetFile("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
  FinalStateManager fsm;
  fsm.SetBaseOutDir("/work/dump/mvatraining");
  fsm.LoadData(&hdata);
  fsm.LoadFinalState("Pi2",
		     "Pi2_Training.root");//from WriteToFile line

  
  fsm.ProcessAll();
  
}
