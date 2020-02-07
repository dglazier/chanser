
{

  auto pi3=Pi3::Make("NONE","ALL");  
  pi3->AddTopology("Electron:Pip1:Pip2:Pim");
  // pi3->AddTopology("Electron:Proton:Pip");

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

  
  pi3->RegisterPostTopoAction(pcm);
  //pi3->RegisterPostTopoAction(pdm);
  
  pi3->CreateFinalHipo("/home/dglazier/fastdata/output.hipo");
  //  pi3->CreateFinalHipo("/work/Dropbox/HaSpect/dev/HASPECT6/build/output.hipo");
  // pi3->CreateFinalTree("/work/Dropbox/HaSpect/dev/HASPECT6/build/output.root");

  //pi3.RegisterPostTopoAction(pcm);
  pi3->WriteToFile("test.root");

  HipoData hd;
  hd.SetFile("/work/jlab/clas12data/v16/skim9_5038.hipo");
  FinalStateManager fsm;
  fsm.LoadData(&hd);
  fsm.LoadFinalState("Pi3","test.root");pi3.reset();
  //fsm.LoadFinalState(std::move(pi3));

  gBenchmark->Start("hd");

  fsm.ProcessAll();

  fsm.EndAndWrite();
   
  gBenchmark->Stop("hd");
  gBenchmark->Print("hd");
}
