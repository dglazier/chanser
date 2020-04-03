{
  ////Set hipo file to be analysed
  HipoData hdata;
  hdata.SetFile("/work/jlab/clas12data/pass1_test/skim8_005051.hipo");

  ////create FinalStateManager
  ////we can load as many saved final states as we like
  FinalStateManager fsm;
  fsm.SetBaseOutDir("/work/dglazier/dump/data/");
  ////Connect the data to the manager
  fsm.LoadData(&hdata);

  ////load one or more FinalStates
  fsm.LoadFinalState("Pi4",
		     "NONEALL_configuration1.root");
 
  //Max number of particles of any 1 species
  //Whole event disgarded if this not met.
  fsm.GetEventParticles().SetMaxParticles(6);

  ////Run through all events
  fsm.ProcessAll(1E5);
  ////Run through N events
  //fsm.ProcessAll(N);


}
