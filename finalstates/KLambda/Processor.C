{
  
  clas12root::HipoChain chain;
  //chain.Add("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
  chain.Add("/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v0/dst/train/skim14/skim14_0050*.hipo");
  chain.Add("/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v0/dst/train/skim14/skim14_0051*.hipo");
  chain.Add("/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v0/dst/train/skim14/skim14_0052*.hipo");
  chain.Add("/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v0/dst/train/skim14/skim14_0053*.hipo");
  
  chain.SetReaderTags({0});
  // chain.GetC12Reader()->useFTBased();
  chain.GetC12Reader()->addAtLeastPid(11,1);
  chain.GetC12Reader()->addExactPid(2212,1);
  chain.GetC12Reader()->addExactPid(321,1);   
  chain.GetC12Reader()->addExactPid(-211,1);

  chanser::HipoProcessor processor(&chain,"finalstates.txt","/work/clas12/dglazier/chanser_out");
  //processor.AddOption("HIPOPROCESSOR_MAXPARTICLES","8");
  //processor.AddOption("HIPOPROCESSOR_FILTERHIPO","filtered.hipo");
  //chain.GetNRecords();
  gProof->Process(&processor,chain.GetNRecords());
  //gProof->Process(&processor,1000);
 
}
