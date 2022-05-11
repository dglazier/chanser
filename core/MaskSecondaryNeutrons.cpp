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
      auto c12N=static_cast<CLAS12Particle*>(neutral)->CLAS12();

      Double_t PCal_Time = c12N->cal(clas12::PCAL)->getTime();
      Double_t ECin_Time = c12N->cal(clas12::ECIN)->getTime();
      Double_t ECout_Time = c12N->cal(clas12::ECOUT)->getTime();

      Double_t PCal_Edep = c12N->cal(clas12::PCAL)->getEnergy();
      Double_t ECin_Edep = c12N->cal(clas12::ECIN)->getEnergy();
      Double_t ECout_Edep = c12N->cal(clas12::ECOUT)->getEnergy();

      //if a neutral doesn't have a hit in the calorimeter.
      Double_t time=9999;
      int sector=c12N->getSector();

      /*Check which calorimeter layers have a hit.
	Take preference for layers closest to target.*/
      if(PCal_Edep>0.01){time=PCal_Time;}
      else if (ECin_Edep>0.01){time=ECin_Time;}
      else if (ECout_Edep>0.01){time=ECout_Time;}

      sectTimeIndex[sector-1].push_back(std::make_pair(time,index));
      
      index++;
    }//loop over neutrals
    
    auto isector=1;

    for(auto& veci:sectTimeIndex){
      
      if(veci.empty()==true) continue; //nothing to do in this sector

      //Neutral particle, contains all secondaries in a sector
      auto c12Neutral=NextNeutralFromPool();

      //will sort based on pair.first, now ranking==order in vector
      std::sort(veci.begin(), veci.end());

      //Have at least one entry, start with that
      //This has earliest time so we always keep it
      auto index = veci[0].second;
      static_cast<CLAS12Particle*>(fd0s.at(index))->SetTimeRanking(1);

      c12Neutral->AddCandidate(static_cast<CLAS12Particle*>(fd0s.at(index)));
      c12Neutral->UseCandidate(0);

      //If we're not masking secondaries
      if(_maskSecondaries==0){
	//then loop over and add them
	for(UInt_t entry=1;entry<veci.size();++entry) {
	  auto index = veci[entry].second;
	  static_cast<CLAS12Particle*>(fd0s.at(index))->SetTimeRanking(entry+1);
	  c12Neutral->AddCandidate(static_cast<CLAS12Particle*>(fd0s.at(index)));
	}
      }

      _vec0.push_back(c12Neutral);
      _vecNeutrons.push_back(c12Neutral);
      if(_nID!=2112){
	_vecGams.push_back(c12Neutral);
      }

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