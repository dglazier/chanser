#include "LundData.h"
#include <sstream>

namespace chanser{

  Bool_t LundData::Init(){
     if(_chainOfFiles.GetListOfFiles()->GetEntries()==0){
      Fatal("LundData::Init","No files given");
      return kFALSE;
    }
    NextFile();

    
    return kTRUE;
  }
  
  Bool_t LundData::NextFile(){
    
    if(_chainOfFiles.GetListOfFiles()->GetEntries()<=_nFile){
      return kFALSE;
    }
    
    TString filename=_chainOfFiles.GetListOfFiles()->At(_nFile)->GetTitle();
    _nFile++;

    if(filename.Length()) _txtIn.open(filename.Data());
    if(!_txtIn.is_open()) {Fatal("LundData::NextFile","file not found %s",filename.Data());return kFALSE;}
    //Get first event header for Nparticles.
    //We will assume for now all events have same final state
    _txtIn>>_Nparts;
    Info("LundData::NextFile","Opened file with %d particles",_Nparts);
    _txtIn.close();
    _txtIn.open(filename.Data());//move back to start
    return kTRUE;
  }
  
  Bool_t LundData::InitEvent(){
    //   std::cout<<"LundData::InitEvent()"<<std::endl;
    getline(_txtIn,_sline);
    
    if(_txtIn.eof()){
      //std::cout<<"LundData::InitEvent() EOF"<<std::endl;
      //End of file look for new one
      if(NextFile()==kFALSE)
	return kFALSE;
      //get line of new file
      getline(_txtIn,_sline);
    }
    //std::cout<<"LundData::InitEvent() "<<_sline<<std::endl;
 
    return kTRUE;
  }
  
  Bool_t LundData::ReadEvent(Long64_t entry){
    // LUND format:
    // Header (Event Info):
    // # of Particles, # of e, particle id, parent, daughter, p_x, p_y, p_z, E, mass, x vertex, y vertex, z vertex
    // type is 1 for particles in the detector
    Double_t dummy;
    if(_txtIn.eof()) return kFALSE;
    ++_entry;
    
    std::stringstream streamline(_sline);
    streamline>>_Nparts>>dummy>>dummy>>dummy>>dummy>>dummy>>_Ebeam>>dummy>>dummy>>dummy;
    //   std::cout<<"LundData::ReadEvent "<<_Nparts<<std::endl;
    //now read in each particle and assign to Particles
    Int_t index,type,pid,parent,daughter;
    Float_t charge,px,py,pz,energy,mass,vx,vy,vz;
    std::stringstream streamlinep;
    _eventTruth.clear();//reset fParticles
    _eventTruth.reserve(_Nparts);//reset fParticles    
    while(_truthPool.size()<_Nparts)
      _truthPool.push_back(std::move(TruthParticle()));

    for(UInt_t i=0;i<_Nparts;i++){
      getline(_txtIn,_sline);
      streamlinep.str(_sline);//set sline as string in stream
      streamlinep>>index>>charge>>type>>pid>>parent>>daughter>>px>>py>>pz>>energy>>mass>>vx>>vy>>vz; //get values from stream
      //std::cout<<"LundData::ReadEvent "<<index<<" "<<charge<<" "<<type <<" "<<pid<<" "<<energy<<std::endl;
      TruthParticle* particle= &_truthPool.at(i);
      particle->_p4.SetXYZT(px,py,pz,energy);
      particle->_vertex.SetXYZ(vx,vy,vz);
      particle->_pdgCode=pid;
      _eventTruth.emplace_back(particle);
      streamlinep.clear();
    }
    
    return kTRUE;
  }
  void LundData::FillRunInfo(){
    _runInfo._BeamEnergy=_Ebeam;
  }
}
