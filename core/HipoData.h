//////////////////////////////////////////////////////////////
///
///Class:		HipoData
///Description:
///
#pragma once

#include "DataManager.h"
#include "CLAS12Particle.h"
#include "RunInfo.h"
#include "EventInfo.h"
#include "clas12reader.h"
#include "clas12writer.h"
#include "TChain.h"


namespace chanser{

  class HipoData : public DataManager{

  public :
    HipoData()=default;
    virtual ~HipoData(){
      if(_myWriter.get())_myWriter->closeWriter();
      _myWriter.reset();
    };

    //DataManager
    Bool_t Init() override;
    virtual Bool_t InitEvent() override;
    Bool_t ReadEvent(Long64_t entry=-1)override;
    Bool_t FetchPids();
    void WriteEvent(Long64_t entry=-1) override{
      if(_myWriter.get())_myWriter->writeEvent();
    }

    //HipoData
    void FillParticles();
    void FillTruth();
    
    void FillEventInfo() override;

    void FillRunInfo() override;
    void FillRunInfoExp();
    void FillRunInfoSim();
    
    clas12::clas12reader* Reader() const {return _c12;}
    clas12::clas12writer* Writer() const {return _myWriter.get();}
    
    void SetReader(clas12::clas12reader* reader){
      _c12=reader;
      Init();
   }
    Bool_t SetFile(const TString filename);
    void AddFile(TString name){
      _chainOfFiles.Add(name);
      if(_nFile==0)//create reader etc for inits
	NextFile();
    }
    Bool_t NextFile();
 
    void SetWriteToFile(TString outfile){
      _myWriter.reset(new clas12::clas12writer(outfile.Data()));
      if(_c12)_myWriter->assignReader(*_c12);
    }
    
    const std::vector<short> eventPids() final{return _c12->preCheckPids();}
    const particle_ptrs& GetParticles() const final {return _eventParticles;};
    const truth_ptrs& GetTruth() const final {return _eventTruth;};

    const BaseRunInfo* GetRunInfo() const noexcept final{return &_runInfo;};
    const BaseEventInfo* GetEventInfo() const noexcept final{return &_eventInfo;};
      

    void LoadAnaDB(const string& name){ _runInfo.LoadAnaDB(name );}

    void SetRunPeriod(TString period){_runPeriod=period;}
    
  protected:
    clas12::clas12reader* _c12=nullptr;  //passed from myC12 or external source
    size_t _nFile={0};
    TChain _chainOfFiles;

  private :

    //clas12root
    std::unique_ptr<clas12::clas12reader> _myC12={nullptr}; //if created here

    particle_objs _particlePool; //pool of particle objects can use for each event
    std::vector<CLAS12Particle> _particlePool2; //pool of particle objects can use for each event
    truth_objs _truthPool; //pool of truth objects can use for each event

      
    particle_ptrs _eventParticles; //actual particles in event
    truth_ptrs _eventTruth; //actual truth particles in event

    RunInfo _runInfo;
    EventInfo _eventInfo;

    TString _runPeriod; //e.g. fall_2018, for getting correct parameters
    
    clas12::clas12databases _c12db;

    
    std::unique_ptr<clas12::clas12writer> _myWriter; //if created here

  };
}
