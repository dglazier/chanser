float checkFieldSetting(clas12::clas12reader* c12,RunInfo &runInfo);

void UseFiducialCuts(){

  //Set up chanser databases and cuts
  RunInfo runInfo;
  runInfo.LoadAnaDB("$CHANSER/rga_actions/anadb/RGA_ACTIONS_PASS1.db");
  runInfo._runPeriod="fall_2018";
  runInfo._dataType="EXP";
  //runInfo._fieldSetting="INBEND";
  float torus=0; //keep track of current torus field

  EventInfo evInfo;
  
  //create a chanser final state
  //this connects database entries to cuts
  CLAS12FinalState fs;
  fs.SetRunInfo(&runInfo);
  fs.SetEventInfo(&evInfo);

  //keep a track of all the cuts I use
  vector<CLAS12Base*> cuts;

  //create a fiducial cut for electrons in the DC based on XY
  FiducialCut_DC_XY e_dc_xy("e-");
  e_dc_xy.SetC12(&fs);
  //store this cut for updating with new torus field
  cuts.push_back(&e_dc_xy);


  
  CLAS12Particle piggy_back; //need this to call chanser cuts


  //Now just standard clas12root via HipoChain
  clas12root::HipoChain chain;
  chain.Add("/work/jlab/clas12data/skim14_005038.hipo");
  //chain.Add("/where/are/my/files/f2.hipo");

  chain.SetReaderTags({0});  //create clas12reader with just tag 0 events

  auto config_c12=chain.GetC12Reader();


  gBenchmark->Start("c12");
 
  //now get reference to (unique)ptr for accessing data in loop
  //this will point to the correct place when file changes
  auto& c12=chain.C12ref();
  while (chain.Next()){
   
    // get particles by type
    auto electrons=c12->getByID(11);
    if(electrons.empty()) continue;
    
    ///This section is needed to update cut parameters that
    ///depend on torus field
    if(c12->runconfig()->getTorus()!=torus){
      //update field setting
      torus = checkFieldSetting(c12.get(),runInfo);
      std::cout<<"Torus changed to "<<torus<<" "<<runInfo._fieldSetting<<" "<<electrons.size()<<endl;

      //here we update parameters
      //in everthing we stored in cuts
      //don't forget to add additional cuts
      evInfo.SetCLAS12( c12.get() );
      for(auto cut:cuts)
	cut->ChangeRun();
    }
    
    //now loop over electrons and chek if passed
    for(auto elec:electrons){
      piggy_back.SetCLAS12Particle(elec);//allow chanser access to elec
      bool passed = e_dc_xy.ParticleCut(&piggy_back);
      // cout<<"e_dc_xy "<<passed<<" "<<elec->getTheta()*TMath::RadToDeg()<<" "<<elec->getP()<<" "<<elec->getRegion()<<endl;
    }
    
  
  }
  gBenchmark->Stop("c12");
  gBenchmark->Print("c12");
}

float checkFieldSetting(clas12::clas12reader *c12,RunInfo &runInfo){

  //in case file changes to different field
  if(c12->runconfig()->getTorus()<0)
    runInfo._fieldSetting="INBEND";
  else if(c12->runconfig()->getTorus()>0)
    runInfo._fieldSetting="OUTBEND";
  else  runInfo._fieldSetting="NOBEND";

  return c12->runconfig()->getTorus();
  
}
