{
  
 clas12root::HipoChain chain;
 //chain.Add("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
 chain.Add("/work/jlab/clas12data/v16/skim9_5038.hipo");
 // chain.GetNRecords();
 chanzer::HipoProcessor processor(&chain,"finalstates.txt");

 gBenchmark->Start("hd");
 gProof->Process(&processor,chain.GetNRecords());
 gBenchmark->Stop("hd");
 gBenchmark->Print("hd");

}
