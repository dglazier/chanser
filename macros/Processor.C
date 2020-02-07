{
  
 clas12root::HipoChain chain;
 chain.Add("/work/jlab/clas12data/v16/skim9_5038.hipo");
 // chain.GetNRecords();
 HS::FINAL::HipoProcessor processor(&chain,"finalstates.txt");
 // processor.LoadFinalStates("finalstates.txt");
 // processor.Begin(nullptr);
 // processor.SlaveBegin(nullptr);
 // processor.Process(1);
 // processor.Process(2);
 gBenchmark->Start("hd");
 gProof->Process(&processor,chain.GetNRecords());
 gBenchmark->Stop("hd");
 gBenchmark->Print("hd");

}
