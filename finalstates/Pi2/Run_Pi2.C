{

  clas12databases::SetCCDBLocalConnection("ccdb.sqlite");
  clas12databases::SetRCDBRootConnection("rcdb.root");

   HipoData hdata;

   hdata.AddFile("/hdd/jlab/clas12data/pass2/DVPipPimP_006619.hipo");
   // /cache/clas12/rg-a/production/recon/spring2019/torus-1/pass2/dst/recon/006619/rec_clas_006619.evio.0
   hdata.LoadAnaDB("$CHANSER/anadbs/RunPeriodPass2.db");
   hdata.LoadAnaDB("$CHANSER/rga_actions/anadb/RGA_ACTIONS_PASS2.db");
   hdata.SetRunPeriod("spring_2019");

   hdata.Reader()->useFTBased();	
   hdata.Reader()->setVerbose();

   ////create FinalStateManager
  ////we can load as many saved final states as we like
  FinalStateManager fsm;
  fsm.SetBaseOutDir("/hdd/dump/Test/DVPipPimP_006619");
  ////Connect the data to the manager
  fsm.LoadData(&hdata);

  ////load one or more FinalStates 
  //fsm.LoadFinalState("Pi2","Pi2_ALL_ALL.root");
  fsm.LoadFinalState("Pi2","Pi2_NONE_ALL.root");

  //Max number of particles of any 1 species
  //Whole event disgarded if this not met.
  fsm.GetEventParticles().SetMaxParticles(4);

  //fsm.CheckCombitorials();
  
  ////Run through all events
  fsm.ProcessAll();


}
