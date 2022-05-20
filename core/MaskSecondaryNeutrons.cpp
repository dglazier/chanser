#include "MaskSecondaryNeutrons.h"
#include "VectorUtility.h"
#include "CLAS12FinalState.h"
  


namespace chanser{

  MaskSecondaryNeutrons::~MaskSecondaryNeutrons(){
  }
  
  void MaskSecondaryNeutrons::AssignVectors(EventParticles* ep){
    //Just Copy all EventParticle vectors
    MaskedEventParticles::AssignVectors(ep);

    SetMapVector(0,&_vec0);
    SetMapVector(22,&_vecGams);
    SetMapVector(2112,&_vecNeutrons);

    _original0=ep->GetParticleVector(0);
    _originalGams=ep->GetParticleVector(22);
    _originalNeutrons=ep->GetParticleVector(2112);
    
    

     //so we don't have to use the map in the event loop
    //Must call this at the end of any derived class AssignVectors
    SetPidVectors();
    
  }
  Bool_t MaskSecondaryNeutrons::ReReadEvent(){
 
    MaskedEventParticles::ReReadEvent(); //set counters to 0

    _vec0.clear();
    _vecGams.clear();
    _vecNeutrons.clear();

    if(_nID==2112){
      auto fdNs=ranges::filter(*_originalNeutrons,CheckForFD);
      //Keep all non FD 
      ranges::append(ranges::filter(*_originalNeutrons,CheckForNotFD),_vecNeutrons);
      ranges::append(ranges::filter(*_originalNeutrons,CheckForNotFD),_vec0);
      //keep all photons
      ranges::append(*_originalGams,_vecGams);
      ranges::append(*_originalGams,_vec0);
      doCorrection(fdNs);
    } else {
      auto fd0s=ranges::filter(*_original0,CheckForFD);
      //Keep all non FD 
      ranges::append(ranges::filter(*_original0,CheckForNotFD),_vec0);

      //should I also be adding the notFD to neutrons, gammas?
      doCorrection(fd0s);
    }
    return kTRUE;
  }

  /* Function responsible for flagging secondary neutrons.
   */
  void MaskSecondaryNeutrons::doCorrection(std::vector<chanser::BaseParticle*> fd0s){

    //Match time to index in fd0s, create vector of these.
    using pairTimeIndex = std::pair<Double_t,Int_t>;
    using vecTimeIndex = std::vector<pairTimeIndex>;

    vector<vecTimeIndex> sectTimeIndex(6); //1 vector for each sector

    int index=0;
    //First we loop over neutrals to find the time of each one produced
    for(auto const& neutral:fd0s){
      auto c12N=static_cast<CLAS12Particle*>(neutral);
      
      //ordered PCAL if exists, then ECIN, then ECOUT
      Double_t time=c12N->Time();
      //if a neutral doesn't have a hit or a bad hit in the calorimeter.
      if(time<=0){time=9999;}

      int sector=c12N->CLAS12()->getSector();

      sectTimeIndex[sector-1].push_back(std::make_pair(time,index));
      index++;
    }//loop over neutrals
    
    auto isector=1;

    //Loop over sectors and rank potential neutron clusters
    for(auto& veci:sectTimeIndex){
      
      if(veci.empty()==true) continue; //nothing to do in this sector

      //Neutral particle, contains all secondaries in a sector
      auto c12Neutral=NextNeutralFromPool();

      //will sort based on pair.first, now ranking==order in vector
      std::sort(veci.begin(), veci.end());

      //Have at least one entry, start with that
      //This has earliest time so we always keep it
      auto index = veci[0].second;

      c12Neutral->AddCandidate(static_cast<CLAS12Particle*>(fd0s.at(index)));
      c12Neutral->UseCandidate(0);

      //If we're not masking secondaries
      if(_maskSecondaries==0){
	//then loop over and add them
	for(UInt_t entry=1;entry<veci.size();++entry) {
	  auto index = veci[entry].second;
	  c12Neutral->AddCandidate(static_cast<CLAS12Particle*>(fd0s.at(index)));
	}
      }

      _vec0.push_back(c12Neutral);
      

      //Which do I push to? 
      //If NID!=2112 then using NONE option so might have
      //both n and gammas as candidates
      //In that case I think we're only using vec0 anyways?
      
      _vecNeutrons.push_back(c12Neutral);
      /*if(_nID!=2112){
	_vecGams.push_back(c12Neutral);
	}*/

      isector++;
    }

  }


  void MaskSecondaryNeutrons::PrintMask() const{
    Info("MaskSecondaryNeutrons::PrintMask() ",Form("Masking EventParticles with  = %s",Class_Name()),"");
    Info("MaskSecondaryNeutrons::PrintMask() ",Form("Flagging secondary neutrons, will I remove these? %d",(Int_t)_maskSecondaries),"");
   
  }

   /* Get particle ID info from topology manager
   */
  void MaskSecondaryNeutrons::UseTopoInfo(TopologyManager& topoInfo, TString pidInfo, TString incInfo){

    _nID=topoInfo.ParticleID(2112);
 
  }
  
}
