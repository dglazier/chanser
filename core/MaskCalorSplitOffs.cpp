#include "MaskCalorSplitOffs.h"
#include "VectorUtility.h"
#include "Topology.h"
#include "TopologyManager.h"


namespace chanser{

  MaskCalorSplitOffs::~MaskCalorSplitOffs(){
    Write(_hR0);
    Write(_hRp);
    Write(_hRm);
  }
  
  void MaskCalorSplitOffs::AssignVectors(EventParticles* ep){
    //Just Copy all EventParticle vectors
    MaskedEventParticles::AssignVectors(ep);

    if(_gamID==22){
      //use my own gamma
      SetMapVector(_gamID,&_vecGams);
      //which I am going to make from EventBuilder IDed gammas
      _originalGams=ep->GetParticleVector(_gamID);
    }
    else{
      //use my own all neutral vector for gammas
      SetMapVector(_gamID,&_vec0);
      //which I am going to make from event particle, all neutrals
      _originalGams=ep->GetParticleVector(_gamID);
      
    }
    
    //so we don't have to use the map in the event loop
    //Must call this at the end of any derived class AssignVectors
    SetPidVectors();
    
  }
  Bool_t MaskCalorSplitOffs::ReReadEvent(){
    using  Position= ROOT::Math::XYZPointF; //floating point position 

    MaskedEventParticles::ReReadEvent(); //set counters to 0
    
    if(_gamID==22)_vecGams.clear();
    else _vec0.clear();


    //remove photons with no PCAL hit
    auto pcalGams=ranges::filter(*_originalGams,CheckForPCAL);
    
    //Loop over remaining gammas 
    for(auto const& gam:pcalGams){
      
      auto c12gam=static_cast<CLAS12Particle*>(gam)->CLAS12();
      Position hitPos(c12gam->cal(clas12::PCAL)->getX(),
		      c12gam->cal(clas12::PCAL)->getY(),
		      c12gam->cal(clas12::PCAL)->getZ());

      Bool_t maskIt=kFALSE;
      particles_ptrs maskedParticles;
      
      ////////////////////////////////////////////////////////
      auto compareToOther =[&maskIt,&maskedParticles,&hitPos,gam,this]
	(const Short_t charge=0,const Float_t rmin,TH1F& hR){

	CLAS12Particle*  other{nullptr};
	UInt_t entry=0;
	while((other=static_cast<CLAS12Particle*>
	       (NextParticle(charge,entry)))!=nullptr){
	  
	  if(other==gam) continue; //don't compare to myself!
	  if(charge==0 && ranges::contains(maskedParticles,other) ) continue;//don't compare to an already rejected cluster
	  
	  Position otherPos(other->CLAS12()->cal(clas12::PCAL)->getX(),
			    other->CLAS12()->cal(clas12::PCAL)->getY(),
			    other->CLAS12()->cal(clas12::PCAL)->getZ());
	  
	  auto diff= otherPos - hitPos; //now difference relative to gamma
	  if( diff.R()<rmin ){
	    maskIt=kTRUE;
	    maskedParticles.push_back(gam); //so don't remove both

	    if(charge==0&&_addSplits){

	      //modifying vector, so create new one so other finalstates not effected
	      if(ranges::contains(maskedParticles,other) == false){
		other =  ReplaceParticlePtr(22,other,NextFromPool()); 		
		maskedParticles.push_back(other); 
	      }
 
	      //give my energy to the other
	      other->SetP4(gam->P4()+other->P4());
	      other->SetDetector(1);
	    }
	  }
	  hR.Fill(diff.R()); //histogram distance between neutral clusters


	}
      };/////////////////////////////////////////////////////////

      //note all particles are contained in 0, -ve or _ve vecs
      compareToOther(0,_r0min,_hR0);
      
      compareToOther(PosID(),_rpmin,_hRp);

      compareToOther(NegID(),_rnmin,_hRm);
      
 
      if( maskIt == kFALSE){
	if(_gamID==22)_vecGams.push_back(gam);
	else _vec0.push_back(gam);
      }
    }
  

    return kTRUE;
  }

  
  void MaskCalorSplitOffs::PrintMask() const{
    Info("MaskCalorSplitOffs::PrintMask() ",Form("Masking EventParticles with  = %s",Class_Name()),"");
    Info("MaskCalorSplitOffs::PrintMask() ",Form("   cutting Gamma candidates at r0=%f , r+=%f , r-=%f .\n Will I combine splitoff neutrals ?  %d",_r0min,_rpmin,_rnmin,(Int_t)_addSplits),"");
    Info("MaskCalorSplitOffs::PrintMask() ",Form("   using gamma PID value %d",_gamID),"");

  }
  /* Get particle ID info from topology manager
   */
  void MaskCalorSplitOffs::UseTopoInfo(TopologyManager& topoInfo, TString pidInfo, TString incInfo){
    _gamID=topoInfo.ParticleID(22);
  }

}
