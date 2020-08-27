{
  ////Set hipo file to be analysed
  HipoData hdata;
  hdata.SetFile("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");

  ////create FinalStateManager
  ////we can load as many saved final states as we like
  FinalStateManager fsm;
 fsm.SetBaseOutDir("/work/dump/klambda");
  ////Connect the data to the manager
  fsm.LoadData(&hdata);

  ////load one or more FinalStates
  fsm.LoadFinalState("KLambda",
		     "KLambda_ALL_ALL.root");
  // fsm.LoadFinalState("KLambda",
  //		     "/dir/fs/file_name_for_this_configuration_2.root");

  //Max number of particles of any 1 species
  //Whole event disgarded if this not met.
  //fsm.GetEventParticles().SetMaxParticles(6);

  ////Run through all events
  fsm.ProcessAll();
  ////Run through N events
  //fsm.ProcessAll(N);


}
