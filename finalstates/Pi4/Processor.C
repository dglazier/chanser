{
  clas12root::HipoChain chain;
  chain.Add("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
  chanser::HipoProcessor processor(&chain,"finalstates.txt","/work/dglazier/dump/data/");
  gProof->Process(&processor,chain.GetNRecords());
  
}
