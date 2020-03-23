{
  ////Set hipo file to be analysed
  HipoData hdata;
  hdata.SetFile("/input/dir/file.hipo");

  ////create FinalStateManager
  ////we can load as many saved final states as we like
  FinalStateManager fsm;
 fsm.SetBaseOutDir("/output/directory");
  ////Connect the data to the manager
  fsm.LoadData(&hdata);

  ////load one or more FinalStates
  fsm.LoadFinalState("templateFS",
		     "/dir/fs/file_name_for_this_configuration_1.root");
  // fsm.LoadFinalState("templateFS",
  //		     "/dir/fs/file_name_for_this_configuration_2.root");

  //Max number of particles of any 1 species
  //Whole event disgarded if this not met.
  fsm.GetEventParticles().SetMaxParticles(6);

  ////Run through all events
  fsm.ProcessAll();
  ////Run through N events
  //fsm.ProcessAll(N);


}
