{
   HipoData hdata;
   hdata.SetFile("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");
   // hdata.SetFile("/work/jlab/clas12data/adamtsim/EptoXto2pi_54.hipo");
   // hdata.Reader()->useFTBased();	
  ////create FinalStateManager
  ////we can load as many saved final states as we like
  FinalStateManager fsm;
  fsm.SetBaseOutDir("/work/dump/mvaresult");
  ////Connect the data to the manager
  fsm.LoadData(&hdata);

  ////load one or more FinalStates
  fsm.LoadFinalState("Pi2",
		     "Pi2_NONE_ALL.root");
  // fsm.LoadFinalState("K2",
  //		     "/dir/fs/file_name_for_this_configuration_2.root");

  //Max number of particles of any 1 species
  //Whole event disgarded if this not met.
  //fsm.GetEventParticles().SetMaxParticles(8);

  ////Run through all events
  fsm.ProcessAll(1E5);


}
