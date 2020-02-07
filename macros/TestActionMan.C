
{

  auto pi2=anaPi2::Make("NONE","ALL");  
  pi2->AddTopology("Electron:Proton:Pip:Pim");
  pi2->AddTopology("Electron:Proton:Pip");

  ParticleCutsManager pcm{"EBCuts",1};
  // HS::FINAL::ParticleCutsManager pcm;
  EventBuilderCut dtc(11);
  pcm.AddParticleCut("e-",dtc);
  pcm.AddParticleCut("proton",dtc);
  pcm.AddParticleCut("pi+",dtc);
  pcm.AddParticleCut("pi-",dtc);

  ParticleDataManager pdm{"particle","./particleData",1};
  ParticleOutEvent pout;
  pdm.SetParticleOut(&pout);

  
  pi2->RegisterPostTopoAction(pcm);
  //pi2->RegisterPostTopoAction(pdm);
  
  pi2->CreateFinalHipo("/home/dglazier/fastdata/output.hipo");
  //  pi2->CreateFinalHipo("/work/Dropbox/HaSpect/dev/HASPECT6/build/output.hipo");
  // pi2->CreateFinalTree("/work/Dropbox/HaSpect/dev/HASPECT6/build/output.root");

  //pi2.RegisterPostTopoAction(pcm);
  pi2->WriteToFile("test.root");

  HipoData hd;
  hd.SetFile("/work/jlab/clas12data/v16/skim9_5038.hipo");
  FinalStateManager fsm;
  fsm.LoadData(&hd);
  fsm.LoadFinalState("Pi2","test.root");pi2.reset();
  //fsm.LoadFinalState(std::move(pi2));

  gBenchmark->Start("hd");

  fsm.ProcessAll();

  fsm.EndAndWrite();
   
  gBenchmark->Stop("hd");
  gBenchmark->Print("hd");
}
