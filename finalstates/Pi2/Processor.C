{
  
  clas12databases::SetCCDBLocalConnection("/hdd/Dropbox/clas12/clas12root/RunRoot/ccdb.sqlite");
  clas12databases::SetRCDBRootConnection("/hdd/Dropbox/clas12/clas12root/RunRoot/rcdb.root");

  clas12root::HipoChain chain;
  chain.Add("/hdd/jlab/clas12data/skim14_005038.hipo");
  chain.Add("/hdd/jlab/clas12data/skim14_005038.hipo");
  chain.Add("/hdd/jlab/clas12data/skim14_005038.hipo");
  chain.Add("/hdd/jlab/clas12data/skim14_005038.hipo");
  // chain.Add("/hdd/dglazier/dump/data/worker_0.0filtered.hipo");
  // chain.Add("/hdd/dglazier/dump/data/worker_0.1filtered.hipo");
  // chain.Add("/hdd/dglazier/dump/data/worker_0.2filtered.hipo");
  // chain.Add("/hdd/dglazier/dump/data/worker_0.3filtered.hipo");
  //chain.Add("/work/jlab/clas12data/adamtsim/EptoXto2pi_54.hipo");
  chain.SetReaderTags({0});
  chain.GetC12Reader()->useFTBased();
    
  chanser::HipoProcessor processor(&chain,"finalstates.txt","/hdd/dglazier/dump/data/");

  //processor.AddOption("HIPOPROCESSOR_MAXPARTICLES","8");
  
  //Filter if any loaded finalstate returns true for WasGoodEvent()
  //WasGoodEvent is true if valid topology with no rejectEvent
  processor.AddOption("HIPOPROCESSOR_FILTERHIPO","filtered.hipo");
  //Load database for run conditions and parameters
  processor.AddOption("HIPOPROCESSOR_ANADB","$CHANSER/rga_actions/anadb/RGA_ACTIONS_PASS1.db:$CHANSER/anadbs/RunPeriodPass1.db");
  //Set run period for particular data set you are processing
  //All data must be from the same run period
  processor.AddOption("HIPOPROCESSOR_RUNPERIOD","fall_2018");

  //chain.GetNRecords();
  gProof->Process(&processor,chain.GetNRecords());
  //gProof->Process(&processor,1000);
 
}
