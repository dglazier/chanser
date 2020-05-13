{
  clas12root::HipoChain chain;
  chain.Add("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
  chain.SetReaderTags({0});
  chanser::HipoProcessor processor(&chain,"finalstates.txt","/work/dglazier/dump/data/");
  chain.GetNRecords();
  gProof->Process(&processor,chain.GetNRecords());
  //gProof->Process(&processor,200);
 
}
