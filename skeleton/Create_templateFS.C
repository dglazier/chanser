{
  auto FS = USERNAME::templateFS::Make("","");
  FS->AddTopology(TOPOLOGY);

  ////Save TreeDatatemplateFS
  //FS->CreateFinalTree("/dir/file.root");
  //FS->CreateFinalHipo("/dir/file.hipo");

  ////
  //ParticleCutsManager pcm{"EBCuts",1};
  //EventBuilderCut dtc; //just use even  builder PID
  //pcm.AddParticleCut("e-",dtc);
  //pcm.AddParticleCut("proton",dtc);
  //pcm.AddParticleCut("pi+",dtc);
  //DeltaTimeCut dtc(2); //Apply a Delta Time cut of 2ns
  //pcm.AddParticleCut("pi-",dtc);
  // FS->RegisterPostTopoAction(pcm);
 
  //ParticleDataManager pdm{"particle","/outdir/particleData",1};
  //ParticleOutEvent pout;
  //pdm.SetParticleOut(&pout);
  //FS->RegisterPostTopoAction(pdm);
  
  FS->WriteToFile("/dir/fs/file_name_for_this_configuration.root");



  //Delete the final state rather than let ROOT try
  FS.reset();
}
