#include "HipoData.h"

namespace chanser{
    
  ////////////////////////////////////////////////////////////////////////
  ///Initialise clas12reader from hipo filename
  Bool_t HipoData::SetFile(const TString filename){
    _c12=nullptr;

    //current hack for finding if simulated data
    //Only works if run number from gemc ==11  !!!!
    if(clas12::clas12reader::readQuickRunConfig(filename.Data())==11){
      _dataType=static_cast<Short_t> (chanser::DataType::Sim);
    }

    _myC12.reset(new clas12::clas12reader(filename.Data(),{0})); //for ownership
    _c12=_myC12.get(); //for using
    if(!_c12) return kFALSE; 
    Init();
    return kTRUE;
  }
  ////////////////////////////////////////////////////////////////////////
  ///Initialise clas12reader from hipo filename
  Bool_t HipoData::Init(){
    
    _eventInfo.SetCLAS12( _c12 );
    _runInfo.SetCLAS12( _c12 );
    if( _myWriter.get()&& (_c12!=nullptr) ) _myWriter->assignReader(*_c12);
 
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
