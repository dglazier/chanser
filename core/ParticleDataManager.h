//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///

#pragma once

#include "ActionManager.h"
#include "ParticleData.h"
#include <TDatabasePDG.h>
#include <TSystem.h>

namespace chanser{
    
     
   
  class ParticleDataManager : public ActionManager {

  public :
      
    ParticleDataManager()=default;
    ///Must give an output directory for saving trees to
  ParticleDataManager(TString name,TString outdir="./particleData",UShort_t addFinal=0):ActionManager(name),_outDir(outdir),_addFinal(addFinal){ std::cout<<"Constructing PDM "<<GetName()<<std::endl;};
      
    virtual ~ParticleDataManager()=default;
    ParticleDataManager(const ParticleDataManager& other) = default; //Copy Constructor
    ParticleDataManager(ParticleDataManager&& other) = default; //Move Constructor
      
    ParticleDataManager& operator=(const ParticleDataManager& other)=default;
    ParticleDataManager& operator=(ParticleDataManager&& other)=default;

    void PrintAction() override;
      
    Bool_t Execute(UInt_t ti)  override{
      //fill data for topology number ti
      _particleData[ti].Fill();
      //_passCut=_particleCuts[ti].PassCuts();
      //return _forReal ? _passCut==_particleCuts[ti].NParticles() : kTRUE;
      return kTRUE;
    }
      
    void End() override;
    void Configure(FinalState* fs) override;

       
    void SetParticleOut(ParticleOutEvent* out){_outData=out;}

  protected:
      
    void MakeOutDir(){
      if(!_outDir.BeginsWith("/"))
	_outDir = TString(gSystem->Getenv("PWD"))+"/"+_outDir;
      gSystem->Exec(Form("mkdir -p %s",_outDir.Data()));
    }
   
  private :

      
    std::vector< ParticleData > _particleData; //!
    //  std::map<Int_t,ParticleOutEvent*> _pdgToData;
    ParticleOutEvent* _outData{nullptr};

    TString _outDir;

    UShort_t _addFinal{0};
      
    ClassDefOverride(chanser::ParticleDataManager,1);
  };// ParticleDataManager
    
  using pdataman_uptr = std::unique_ptr<ParticleDataManager>;
  using pdataman_ptr = ParticleDataManager*;
  using pdatamans =std::vector<pdataman_ptr>;
    



}

