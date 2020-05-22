{
  
  clas12root::HipoChain chain;
  chain.Add("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
  chain.SetReaderTags({0});
  // chain.GetC12Reader()->useFTBased();
    
  chanser::HipoProcessor processor(&chain,"finalstates.txt","/work/dglazier/dump/data/");
  processor.AddOption("HIPOPROCESSOR_MAXPARTICLES","8");
  //chain.GetNRecords();
  gProof->Process(&processor,chain.GetNRecords());
  //gProof->Process(&processor,1000);
 
}
