#include "HipoData.h"

namespace chanser{
    
  ////////////////////////////////////////////////////////////////////////
  ///Initialise clas12reader from hipo filename
  Bool_t HipoData::SetFile(const TString filename){
    _c12=nullptr;
    
    _myC12.reset(new clas12::clas12reader(filename.Data(),{0})); //for ownership
    _c12=_myC12.get(); //for using
    if(!_c12) return kFALSE; 
    Init();
    return kTRUE;
  }
  ////////////////////////////////////////////////////////////////////////
  ///Initialise clas12reader from hipo filename
  ///this gets called in HipoProcessor, unlike SetFile
  Bool_t HipoData::Init(){
    
    //current hack for finding if simulated data
    //Only works if run number from gemc ==11  !!!!
    if(clas12::clas12reader::readQuickRunConfig(_c12->getFilename())==11){
      _dataType=static_cast<Short_t> (chanser::DataType::Sim);
    }

    //On PROOF databases will be set from HipoSelector to HipoChain db
    if(&(_c12->db())==nullptr)_c12->connectDataBases(&_c12db);

    _eventInfo.SetCLAS12( _c12 );
    _runInfo.SetCLAS12( _c12 );
    
    if( _myWriter.get()&& (_c12!=nullptr) ) _myWriter->assignReader(*_c12);

    return kTRUE;
  }
  /////////////////////////////////////////////////////////////////////
  //Check if there is another event
  //Can now get Pid list for this event
  Bool_t HipoData::InitEvent(){

      
    //keep going until we get an event that has particles
    while(_c12->getReader().next()){

      if(FetchPids())
	return kTRUE;
    }
    return kFALSE; //all event done
  }
  /////////////////////////////////////////////////////////////////////
  //Check if there is another event
  //Can now get Pid list for this event
  Bool_t HipoData::FetchPids(){

    _c12->clearEvent();
    _entry++;
   
    if(_entry%100000==0) std::cout<<"HipoData::InitEvent() "<<_entry<<std::endl;
      
    if(!_c12->preCheckPids().empty()){ //got one
      return kTRUE;
    }
    return kFALSE;    
  }
  /////////////////////////////////////////////////////////////////
  ///
  Bool_t HipoData::ReadEvent(Long64_t entry){
      
    _c12->readEvent();//OK to read event from disk
    //_dataType=_c12->runconfig()->getType(); //needs written in gemc
      
    _c12->sort();
      
    FillParticles(); //create BaseParticles
    if(_dataType==static_cast<Short_t> (chanser::DataType::Sim))FillTruth();
    FillEventInfo();
      
    return kTRUE;
  }
  //////////////////////////////////////////////////////////////////
  ///
  void HipoData::FillParticles(){
    //Note this function add 10% time to just _C12->next();
      
    const UInt_t Nin=_c12->getNParticles();
    _eventParticles.clear();
    _eventParticles.reserve(Nin);

     
    //std::cout<<" HipoData::FillParticles() "<<Nin<<std::endl;
    auto c12particles= _c12->getDetParticles();
    UInt_t Nparts=0;

    //make sure enough particles in the pool
    //having particle in the pool stops them
    //being contructed every event
    while(_particlePool2.size()<Nin)
      _particlePool2.push_back(std::move(CLAS12Particle()));

    for(auto& c12p : c12particles){
      Nparts++;
      CLAS12Particle* particle= (&_particlePool2.at(Nparts-1));
      particle->Clear();//clear pervious data

      //attach this particle
      particle->SetCLAS12Particle(c12p);

      //include partice in event
      _eventParticles.emplace_back(static_cast<BaseParticle*>(particle));
    }
  }
  //////////////////////////////////////////////////////////////////
  ///
  void HipoData::FillTruth(){
 
    auto mcpbank=_c12->mcparts();
      
    const Int_t  Ngen=mcpbank->getRows();
    
    _eventTruth.clear();
    _eventTruth.reserve(Ngen);

    while(_truthPool.size()<Ngen)
      _truthPool.push_back(std::move(TruthParticle()));

    for(Int_t i=0;i<Ngen;i++){
      mcpbank->setEntry(i);

      TruthParticle* particle= &_truthPool.at(i);
      auto px=mcpbank->getPx();
      auto py=mcpbank->getPy();
      auto pz=mcpbank->getPz();
      auto pm=mcpbank->getMass();
      particle->_p4.SetXYZT(px,py,pz,sqrt(static_cast<Double_t>((px*px+py*py+pz*pz+pm*pm))));
      particle->_vertex.SetXYZ(mcpbank->getVx() ,mcpbank->getVy() ,mcpbank->getVz());
      particle->_pdgCode=mcpbank->getPid();
      _eventTruth.emplace_back(particle);
    }

  }
  ///////////////////////////////////////////////////////////////
  void HipoData::FillRunInfo(){
    if(IsSim()){
      FillRunInfoSim();
    }
    else{
      //Normal experimental data, use databases
      FillRunInfoExp();
    }

    if(_c12->runconfig()->getTorus()<0)
      _runInfo._fieldSetting="INBEND";
    else if(_c12->runconfig()->getTorus()>0)
      _runInfo._fieldSetting="OUTBEND";
    else  _runInfo._fieldSetting="NOBEND";


 
  }
  ///////////////////////////////////////////////////////////////
  void HipoData::FillRunInfoSim(){
    
    _runInfo._runPeriod="fall_2018";
    _runInfo._dataType="SIM";
    
    auto period = _runInfo._runPeriod + "_" + _runInfo._dataType;

  _runInfo._BeamEnergy  =  _c12->mcevent()->getEbeam();
    
    //target position in simulation
    auto table = _runInfo.
      GetAnaDB().GetTable(period,
			  "TARGET_POSITION"
			  ,{3}); //x,y,z pos
    std::vector<double> tarPos(3);
    table.Fill(tarPos);
    _runInfo._TargetCentre=tarPos[2];


    //Beam bucket
    table = _runInfo.
      GetAnaDB().GetTable(period,
			  "RF_BUCKET_LENGTH"
			  ,{1}); //1 time
    std::vector<double> bucket(1);
    table.Fill(bucket);
    _runInfo._rfBucketLength=bucket[0];

  }
  ///////////////////////////////////////////////////////////////
  void HipoData::FillRunInfoExp(){
    //cache data from rcdb
    auto rcdb=_c12->rcdb();
    if(rcdb){
    _runInfo._BeamEnergy  = rcdb->current().beam_energy/1000;//to GeV
    }
    //cache data from ccdb
    auto ccdb=_c12->ccdb();
    if(ccdb){
      /////////////////////////////////////////////////
      //target
      _runInfo._TargetCentre=ccdb->requestTableValueFor(0,"position","/geometry/target")/100;
      _runInfo._TargetCentre=-0.03;
      /////////////////////////////////////////////////
      //rf
      int rfStat1=ccdb->requestTableValueFor(0,"status","/calibration/eb/rf/config");
      int rfStat2=ccdb->requestTableValueFor(1,"status","/calibration/eb/rf/config");

      // There are two rows in rf/config here we find the one with status=1
      // if (rfStat1<=0 && rfStat2<=0)
      // throw new RuntimeException("Couldn't find non-positive RF status in CCDB");
      int rfId = rfStat2>rfStat1 ? 1 : 0;
      _runInfo._rfBucketLength=ccdb->requestTableValueFor(rfId,"clock","/calibration/eb/rf/config");//EBCCDBEnum.RF_BUCKET_LENGTH
 
    }
    _runInfo._runPeriod="fall_2018";
    _runInfo._dataType="EXP";

    
  }
  ///////////////////////////////////////////////////////////////
  void HipoData::FillEventInfo(){
    _eventInfo._TrigBit=_c12->runconfig()->getTrigger();
    _eventInfo._StartTime=_c12->event()->getStartTime();
    _eventInfo._FTBStartTime=_c12->event()->getFTBStartTime();
    _eventInfo._RFTime=_c12->event()->getRFTime();
    _eventInfo._BeamHel=_c12->event()->getHelicity();
    _eventInfo._NEvent=_c12->runconfig()->getEvent();
  }
    
}
