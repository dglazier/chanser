{
  auto FS = USERNAME::templateFS::Make("","");
  FS->AddTopology(TOPOLOGY);
  // FS->AddTopology(OTHER_TOPOLOGY);
  
  ////Save TreeDatatemplateFS
  //FS->UseOutputRootTree();
  //FS->UseOutputHipoNtuple();

  /////Make particle trees first in case want to add cut flags
  //ParticleDataManager pdm{"particle",1};
  //pdm.SetParticleOut(new CLAS12ParticleOutEvent0);
  //FS->RegisterPostTopoAction(pdm);

  ////
  //ParticleCutsManager pcm{"EBCuts",1};
  //pcm.AddParticleCut("e-",new EventBuilderCut());
  //pcm.AddParticleCut("proton",new EventBuilderCut());
  //pcm.AddParticleCut("pi+",new EventBuilderCut());
  //pcm.AddParticleCut("pi-", new DeltaTimeCut(2));//Apply a Delta Time cut of 2ns
  // FS->RegisterPostTopoAction(pcm);
 
  ////Write to file for later processing
  FS->WriteToFile("/dir/fs/file_name_for_this_configuration.root");

  FS->Print();


  //Delete the final state rather than let ROOT try
  FS.reset();
}
