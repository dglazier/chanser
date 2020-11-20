{
  clas12databases::SetCCDBLocalConnection("/home/dglazier/clas12root/RunRoot/ccdb.sqlite");
  clas12databases::SetQADBConnection("/home/dglazier/clas12root/RunRoot/qaDB.json");
  //clas12databases::SetRCDBRootConnection("/home/dglazier/clas12root/RunRoot/rcdb.root");

  clas12root::HipoChain chain;
  chain.Add("/home/dglazier/fastdata/skim3_005652.hipo");
  //chain.Add("/work/jlab/clas12data/adamtsim/EptoXto2pi_54.hipo");
  chain.SetReaderTags({0});
  chain.GetC12Reader()->useFTBased();
    
  chanser::HipoProcessor processor(&chain,"finalstates.txt","/work/dglazier/dump/data/");
  //processor.AddOption("HIPOPROCESSOR_MAXPARTICLES","8");
  //processor.AddOption("HIPOPROCESSOR_FILTERHIPO","filtered.hipo");
  processor.AddOption("HIPOPROCESSOR_ANADB","$CHANSER/rga_actions/anadb/RGA_ACTIONS_PASS1.db:$CHANSER/anadbs/RunPeriodPass1.db");
  //chain.GetNRecords();
  gProof->Process(&processor,chain.GetNRecords());
  //gProof->Process(&processor,1000);
 
}
