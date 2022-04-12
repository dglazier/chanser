#include "MaskSecondaryNeutrons.h"
#include "VectorUtility.h"
#include "CLAS12FinalState.h"
  


namespace chanser{

  MaskSecondaryNeutrons::~MaskSecondaryNeutrons(){
  }
  
  void MaskSecondaryNeutrons::AssignVectors(EventParticles* ep){
    //Just Copy all EventParticle vectors
    MaskedEventParticles::AssignVectors(ep);
    // return;

    //SetMapVector(1E4,&_vecPlus);
    //SetMapVector(-1E4,&_vecMinus);
    SetMapVector(0,&_vec0);

    _original0=ep->GetParticleVector(0);
    _originalPlus=ep->GetParticleVector(-11);
    _originalMinus=ep->GetParticleVector(11);
    //_originalPlus=ep->GetParticleVector(1E4);
    //_originalMinus=ep->GetParticleVector(-1E4);

     //so we don't have to use the map in the event loop
    //Must call this at the end of any derived class AssignVectors
    SetPidVectors();
    
  }
  Bool_t MaskSecondaryNeutrons::ReReadEvent(){
 
    MaskedEventParticles::ReReadEvent(); //set counters to 0

    _vec0.clear();
    //_vecMinus.clear();
    //_vecPlus.clear();
    //ranges::copy(*_originalPlus,_vecMinus);
    //ranges::copy(*_originalMinus,_vecPlus);
    //ranges::copy(*_original0,_vec0);

    
    auto fd0s=ranges::filter(*_original0,CheckForFD);
   //Keep all non FD 
    ranges::append(ranges::filter(*_original0,CheckForNotFD),_vec0);

    /*auto fdNs=ranges::filter(fd0s,CheckPassBetaCut);
    //Keep all that pass neutron ID
    ranges::append(ranges::filter(fd0s,CheckNotPassBetaCut),_vec0);*/

    std::cout<<"Sec N size of neutrals pre cut: "<<_original0->size()<<std::endl;

    doCorrection(fd0s);

    std::cout<<"Sec N size of neutrals post cut: "<<_vec0.size()<<std::endl;

    return kTRUE;
  }

  /* Function responsible for flagging secondary neutrons.
   */
  void MaskSecondaryNeutrons::doCorrection(std::vector<chanser::BaseParticle*> fd0s){

    /*Don't want to mask neutrals in the same sector as charged
      particles.*/
    vector<Int_t> chargedParticleSectors;

     //Loop over positively charged particles
    for(auto const& pos:*_originalPlus){
      auto c12Pos=static_cast<CLAS12Particle*>(pos);
      //if(c12Pos->Detector()>=20){
	//cout<<"e+ with rad photon"<<endl;
	//chargedParticleSectors.push_back(c12Pos->CLAS12()->getSector());
	//}
    }

    //Loop over negatively charged particles
    for(auto const& neg:*_originalMinus){
      auto c12Neg=static_cast<CLAS12Particle*>(neg);
      ///if(c12Neg->Detector()>=20){
	//cout<<"e- with rad photon"<<endl;
	//chargedParticleSectors.push_back(c12Neg->CLAS12()->getSector());
      //}
    }

    //Match time to index in fd0s, create vector of these.
    using pairTimeIndex = std::pair<Double_t,Int_t>;
    using vecTimeIndex = std::vector<pairTimeIndex>;

    vector<vecTimeIndex> sectTimeIndex(6); //1 vector for each sector

    //Skip particles from ranking if Beta is nan or IDed as photons
     vector<Int_t> skipParticles;

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
      /*if(PCal_Edep>0.01){time=PCal_Time;}
	else if (ECin_Edep>0.01){time=ECin_Time;}
	else if (ECout_Edep>0.01){time=ECout_Time;}*/

      if(PCal_Time>50){time=PCal_Time;}
      else if (ECin_Time>50){time=ECin_Time;}
      else if (ECout_Time>50){time=ECout_Time;}

      /*The below seems a bit weird but we want to use
	the beta value from DSTs to calculate momentum
	then recalculate beta assuming we have a neutron*/
      Double_t betaFromDST=c12N->par()->getBeta();
      Double_t P=0.93957*betaFromDST/sqrt(1-betaFromDST*betaFromDST);
      Double_t beta=P/sqrt(P*P+0.93957*0.93957);
      Bool_t passCut=false;
      if(!std::isnan(beta) && beta<=_betaCut){
	sectTimeIndex[sector-1].push_back(std::make_pair(time,index));
      } else{
	skipParticles.push_back(index);
      }
      
      index++;
    }//loop over neutrals

    //Add particles skipped from ranking
    for (auto& ind:skipParticles){
      _vec0.push_back(fd0s.at(ind));
    }

    
    auto isector=1;

    for(auto& veci:sectTimeIndex){
      
      if(veci.empty()==true) continue; //nothing to do in this sector

      //If we have a charged particle in this sector, don't mask neutrals
      if(vectorContains(chargedParticleSectors,isector)==kTRUE){
	for(auto& vecj:veci){_vec0.push_back(fd0s.at(vecj.second));}
	continue; 
      }

      //will sort based on pair.first, now ranking==order in vector
      std::sort(veci.begin(), veci.end());

      //Have at least one entry, start with that
      //This has earliest time so we always keep it
      auto index = veci[0].second;
      static_cast<CLAS12Particle*>(fd0s.at(index))->SetTimeRanking(1);
      _vec0.push_back(fd0s.at(index));

      //only 1 particle, nothing else to do
      if(veci.size()==1) continue; 

      //If we're not masking secondaries
      if(_maskSecondaries==0){
	//then loop over and add them
	for(UInt_t entry=1;entry<veci.size();++entry) {
	  auto index = veci[entry].second;
	  static_cast<CLAS12Particle*>(fd0s.at(index))->SetTimeRanking(entry+1);
	  _vec0.push_back(fd0s.at(index));
	}
      }
      isector++;
    }
    

    //check ranking has propagated -- it has last tested
    /*auto vec=GetParticleVector(0);
    for(auto const& p:*vec){
      //auto test=static_cast<CLAS12Particle*>(_vec0.back());
      std::cout<<"Set ranking: "<<p->TimeRanking()<<std::endl;
      
      }*/

  }

  /*Check if an Int_t vector contains a given key. */
  bool MaskSecondaryNeutrons::vectorContains(std::vector<Int_t> v, Int_t key){
    bool found=false;
    if (std::find(v.begin(), v.end(), key) != v.end()) {
      found=true;
    }
    return found;
  }

/*Check if an Double_t vector contains a given key. */
  bool MaskSecondaryNeutrons::vectorContainsD(std::vector<Double_t> v, Double_t key){
    bool found=false;
    if (std::find(v.begin(), v.end(), key) != v.end()) {
      found=true;
    }
    return found;
  }

  void MaskSecondaryNeutrons::PrintMask() const{
    Info("MaskSecondaryNeutrons::PrintMask() ",Form("Masking EventParticles with  = %s",Class_Name()),"");
    Info("MaskSecondaryNeutrons::PrintMask() ",Form("Flagging secondary neutrons, will I remove these? %d",(Int_t)_maskSecondaries),"");
   
  }
  
}
