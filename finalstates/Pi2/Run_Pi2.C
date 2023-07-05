{

  clas12databases::SetCCDBLocalConnection("ccdb.sqlite");
  clas12databases::SetRCDBRootConnection("rcdb.root");

   HipoData hdata;
   hdata.AddFile("/hdd/jlab/clas12data/pass2/DVPipPimP_*.hipo");

   hdata.LoadAnaDB("$CHANSER/anadbs/RunPeriodPass1.db");
   hdata.LoadAnaDB("$CHANSER/rga_actions/anadb/RGA_ACTIONS_PASS1.db");
   hdata.SetRunPeriod("fall_2018");

   hdata.Reader()->useFTBased();	


   ////create FinalStateManager
  ////we can load as many saved final states as we like
  FinalStateManager fsm;
  fsm.SetBaseOutDir("/hdd/dump/Test");
  ////Connect the data to the manager
  fsm.LoadData(&hdata);

  ////load one or more FinalStates 
  fsm.LoadFinalState("Pi2","Pi2_ALL_ALL.root");
  fsm.LoadFinalState("Pi2","Pi2_NONE_ALL.root");

  //Max number of particles of any 1 species
  //Whole event disgarded if this not met.
  fsm.GetEventParticles().SetMaxParticles(10);

  // fsm.CheckCombitorials();
  
  ////Run through all events
  fsm.ProcessAll();


}
