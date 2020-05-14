{
  
 clas12root::HipoChain chain;
 chain.SetReaderTags({0});
 //chain.Add("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
 chain.Add("/work/jlab/clas12data/v16/skim9_5038.hipo");
 // chain.GetNRecords();
 chanser::HipoProcessor processor(&chain,"finalstates.txt");
 processor.AddOption("HIPOPROCESSOR_MAXPARTICLES","5");

 gBenchmark->Start("hd");
 gProof->Process(&processor,chain.GetNRecords());
 gBenchmark->Stop("hd");
 gBenchmark->Print("hd");

}
