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
  //EventBuilderCut* ebc=new EventBuilderCut(); //just use even  builder PID
  //pcm.AddParticleCut("e-",ebc);
  //pcm.AddParticleCut("proton",ebc);
  //pcm.AddParticleCut("pi+",ebc);
  //DeltaTimeCut* dtc=new DeltaTimeCut(2); //Apply a Delta Time cut of 2ns
  //pcm.AddParticleCut("pi-",dtc);
  // FS->RegisterPostTopoAction(pcm);
 
  ////Write to file for later processing
  FS->WriteToFile("/dir/fs/file_name_for_this_configuration.root");

  FS->Print();


  //Delete the final state rather than let ROOT try
  FS.reset();
}
