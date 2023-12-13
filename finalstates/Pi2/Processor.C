{
  
  clas12databases::SetCCDBLocalConnection("ccdb.sqlite");
  clas12databases::SetRCDBRootConnection("rcdb.root");

  clas12root::HipoChain chain;
  // chain.Add("/hdd/jlab/clas12data/pass2/DVPipPimP_006619.hipo");
  chain.Add("/hdd/jlab/clas12data/pass2/DVPipPimP_006619.hipo");
  
  chain.SetReaderTags({0});
  chain.GetC12Reader()->useFTBased();
  cout<<"Now make processor "<<chain.db()<<endl;    
  chanser::HipoProcessor processor(&chain,"finalstates.txt","/hdd/dglazier/dump/data/");

  processor.AddOption("HIPOPROCESSOR_MAXPARTICLES","6");
  
  //Filter if any loaded finalstate returns true for WasGoodEvent()
  //WasGoodEvent is true if valid topology with no rejectEvent
  // processor.AddOption("HIPOPROCESSOR_FILTERHIPO","filtered.hipo");
  //Load database for run conditions and parameters
  processor.AddOption("HIPOPROCESSOR_ANADB","$CHANSER/rga_actions/anadb/RGA_ACTIONS_PASS2.db:$CHANSER/anadbs/RunPeriodPass2.db");
  //Set run period for particular data set you are processing
  //All data must be from the same run period
  //processor.AddOption("HIPOPROCESSOR_RUNPERIOD","fall_2018");
  processor.AddOption("HIPOPROCESSOR_RUNPERIOD","spring_2019");

  gProof->Process(&processor,chain.GetNRecords());

  //OR run it wihtout using Proof
  //processor.ProcessAll();

}
