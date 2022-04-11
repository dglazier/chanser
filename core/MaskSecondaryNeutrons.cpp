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
	chargedParticleSectors.push_back(c12Pos->CLAS12()->getSector());
	//}
    }

    //Loop over negatively charged particles
    for(auto const& neg:*_originalMinus){
      auto c12Neg=static_cast<CLAS12Particle*>(neg);
      ///if(c12Neg->Detector()>=20){
	//cout<<"e- with rad photon"<<endl;
	chargedParticleSectors.push_back(c12Neg->CLAS12()->getSector());
      //}
    }

    /*maps are ordered by key, meaning we can use this to sort
      our list of neutrons by the shortest time. 
      Make a map per sector as we want the earliest neutral per sector.*/
    std::map<Double_t,int> timeIndexMap_S1;
    std::map<Double_t,int> timeIndexMap_S2;
    std::map<Double_t,int> timeIndexMap_S3;
    std::map<Double_t,int> timeIndexMap_S4;
    std::map<Double_t,int> timeIndexMap_S5;
    std::map<Double_t,int> timeIndexMap_S6;

    /*Maps can only contain one entry per key, if two neutrals have
      the same time, the second will be assigned that time+time/1000
      If two neutrals have the exact same time they will have similar 
      values of momentum anyways so no need to worry about this.*/
    std::vector<Double_t> timePEvent;

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
      Double_t time=9999+index;

      /*Check which calorimeter layers have a hit.
	Take preference for layers closest to target.*/
      if(PCal_Edep>0.01){time=PCal_Time;}
      else if (ECin_Edep>0.01){time=ECin_Time;}
      else if (ECout_Edep>0.01){time=ECout_Time;}

      if(vectorContainsD(timePEvent,time)){time=time+index*time/1000;}

      if(c12N->getSector()==1){timeIndexMap_S1[time]=index;}
      else if(c12N->getSector()==2){timeIndexMap_S2[time]=index;}
      else if(c12N->getSector()==3){timeIndexMap_S3[time]=index;}
      else if(c12N->getSector()==4){timeIndexMap_S4[time]=index;}
      else if(c12N->getSector()==5){timeIndexMap_S5[time]=index;}
      else if(c12N->getSector()==6){timeIndexMap_S6[time]=index;}
      

      index++;
    }//loop over neutrals

    
    
    for (std::map<Double_t, int>::iterator it = timeIndexMap_S1.begin(); it != timeIndexMap_S1.end(); it++){
      int indexR=it->second;
      Double_t ranking=distance(timeIndexMap_S1.begin(),it)+1;
      //don't want to remove neutrals in same sector as charged particles.
      if(vectorContains(chargedParticleSectors,1)){_vec0.push_back(fd0s.at(indexR));}
      else{
	//std::cout<<"neutron S1 not in same sector as charged particle"<<std::endl;
	//std::cout<<"Ranking: "<<ranking<<" index "<<indexR<<endl;
	auto neutralW=static_cast<CLAS12Particle*>(fd0s.at(indexR));
	neutralW->SetTimeRanking(ranking);
	//if the ranking is 1 then keep that particle
	if(ranking == 1){_vec0.push_back(fd0s.at(indexR));}
	//if the user chooses not to remove secondaries
	else if(_maskSecondaries==0){_vec0.push_back(fd0s.at(indexR));}
	else{std::cout<<"SecN removed neutral"<<std::endl;}
	/*auto test=static_cast<CLAS12Particle*>(_vec0.back());
	  std::cout<<"Set ranking: "<<test->TimeRanking()<<std::endl;*/
      }
    }//loop over map

    //same for other sectors
    for (std::map<Double_t, int>::iterator it = timeIndexMap_S2.begin(); it != timeIndexMap_S2.end(); it++){
      int indexR=it->second;
      Double_t ranking=distance(timeIndexMap_S2.begin(),it)+1;
      if(vectorContains(chargedParticleSectors,2)){_vec0.push_back(fd0s.at(indexR));}
      else{
	auto neutralW=static_cast<CLAS12Particle*>(fd0s.at(indexR));
	neutralW->SetTimeRanking(ranking);
	if(ranking == 1){_vec0.push_back(fd0s.at(indexR));}
	else if(_maskSecondaries==0){_vec0.push_back(fd0s.at(indexR));}
	else{std::cout<<"SecN removed neutral"<<std::endl;}
      }
    }//loop over map S2

    for (std::map<Double_t, int>::iterator it = timeIndexMap_S3.begin(); it != timeIndexMap_S3.end(); it++){
      int indexR=it->second;
      Double_t ranking=distance(timeIndexMap_S3.begin(),it)+1;
      if(vectorContains(chargedParticleSectors,3)){_vec0.push_back(fd0s.at(indexR));}
      else{
	auto neutralW=static_cast<CLAS12Particle*>(fd0s.at(indexR));
	neutralW->SetTimeRanking(ranking);
	if(ranking == 1){_vec0.push_back(fd0s.at(indexR));}
	else if(_maskSecondaries==0){_vec0.push_back(fd0s.at(indexR));}
	else{std::cout<<"SecN removed neutral"<<std::endl;}
      }
    }//loop over map S3

    for (std::map<Double_t, int>::iterator it = timeIndexMap_S4.begin(); it != timeIndexMap_S4.end(); it++){
      int indexR=it->second;
      Double_t ranking=distance(timeIndexMap_S4.begin(),it)+1;
      if(vectorContains(chargedParticleSectors,4)){_vec0.push_back(fd0s.at(indexR));}
      else{
	auto neutralW=static_cast<CLAS12Particle*>(fd0s.at(indexR));
	neutralW->SetTimeRanking(ranking);
	if(ranking == 1){_vec0.push_back(fd0s.at(indexR));}
	else if(_maskSecondaries==0){_vec0.push_back(fd0s.at(indexR));}
	else{std::cout<<"SecN removed neutral"<<std::endl;}
      }
    }//loop over map S4

    for (std::map<Double_t, int>::iterator it = timeIndexMap_S5.begin(); it != timeIndexMap_S5.end(); it++){
      int indexR=it->second;
      Double_t ranking=distance(timeIndexMap_S5.begin(),it)+1;
      if(vectorContains(chargedParticleSectors,5)){_vec0.push_back(fd0s.at(indexR));}
      else{
	auto neutralW=static_cast<CLAS12Particle*>(fd0s.at(indexR));
	neutralW->SetTimeRanking(ranking);
	if(ranking == 1){_vec0.push_back(fd0s.at(indexR));}
	else if(_maskSecondaries==0){_vec0.push_back(fd0s.at(indexR));}
	else{std::cout<<"SecN removed neutral"<<std::endl;}
      }
    }//loop over map S5

    for (std::map<Double_t, int>::iterator it = timeIndexMap_S6.begin(); it != timeIndexMap_S6.end(); it++){
      int indexR=it->second;
      Double_t ranking=distance(timeIndexMap_S6.begin(),it)+1;
      if(vectorContains(chargedParticleSectors,6)){_vec0.push_back(fd0s.at(indexR));}
      else{
	auto neutralW=static_cast<CLAS12Particle*>(fd0s.at(indexR));
	neutralW->SetTimeRanking(ranking);
	if(ranking == 1){_vec0.push_back(fd0s.at(indexR));}
	else if(_maskSecondaries==0){_vec0.push_back(fd0s.at(indexR));}
	else{std::cout<<"SecN removed neutral"<<std::endl;}
      }
    }//loop over map S6

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
