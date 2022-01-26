#include "MaskCalorNeutronSplitOffs.h"
#include "VectorUtility.h"
#include "Topology.h"
#include "TopologyManager.h"


namespace chanser{

  MaskCalorNeutronSplitOffs::~MaskCalorNeutronSplitOffs(){
    Write(_hR0);
    Write(_hRp);
    Write(_hRm);
  }
  
  void MaskCalorNeutronSplitOffs::AssignVectors(EventParticles* ep){
    //Just Copy all EventParticle vectors
    MaskedEventParticles::AssignVectors(ep);

    if(_neutID==2112){
      //use my own gamma
      SetMapVector(_neutID,&_vecNeutrons);
      //which I am going to make from EventBuilder IDed gammas
      _originalNeutrons=ep->GetParticleVector(_neutID);
    }
    else{
      //use my own neutral vector
      SetMapVector(_neutID,&_vec0);
      //which I am going to make from  event particle, all neutrals
      _originalNeutrons=ep->GetParticleVector(_neutID);
      
    }
    
    //so we don't have to use the map in the event loop
    //Must call this at the end of any derived class AssignVectors
    SetPidVectors();
    
  }
  Bool_t MaskCalorNeutronSplitOffs::ReReadEvent(){
    using  Position= ROOT::Math::XYZPointF; //floating point position 

    MaskedEventParticles::ReReadEvent(); //set counters to 0
    
    if(_neutID==2112)_vecNeutrons.clear();
    else _vec0.clear();

    //remove neutrons with PCAL hit
    //remove for now as interferes with gammas when not using PID
    // auto pcalNeuts=ranges::filter(*_originalNeutrons,CheckForNotPCAL);
    
    //Loop over remaining gammas 
    for(auto const& neut:*_originalNeutrons){
      
      auto c12neut=static_cast<CLAS12Particle*>(neut)->CLAS12();

      Position hitPos;
      
      //Now check if hit in ECIN
      if(c12neut->cal(clas12::ECIN)->getEnergy()!=0){
	hitPos.SetXYZ(c12neut->cal(clas12::ECIN)->getX(),
		      c12neut->cal(clas12::ECIN)->getY(),
		      c12neut->cal(clas12::ECIN)->getZ());
      }
      //Now check if hit in ECOUT
      else if(c12neut->cal(clas12::ECOUT)->getEnergy()!=0){
	hitPos.SetXYZ(c12neut->cal(clas12::ECOUT)->getX(),
		      c12neut->cal(clas12::ECOUT)->getY(),
		      c12neut->cal(clas12::ECOUT)->getZ());
      }
      else{//not a calorimeter neutron, should keep it
	if(_neutID==2112)_vecNeutrons.push_back(neut);
	else _vec0.push_back(neut);

	continue;
	//go no further in the loop
      }
      
      Bool_t maskIt=kFALSE;
      particles_ptrs maskedParticles;
      
      ////////////////////////////////////////////////////////
      auto compareToOther =[&maskIt,&maskedParticles,&hitPos,neut,this]
	(const Short_t charge=0,const Float_t rmin,TH1F& hR){

	CLAS12Particle*  other{nullptr};
	UInt_t entry=0;
	while((other=static_cast<CLAS12Particle*>
	       (NextParticle(charge,entry)))!=nullptr){
	  
	  if(other==neut) continue; //don't compare to myself!

	  //only count as split off neutron, if other had a PCAL hit
	  if(other->CLAS12()->cal(clas12::PCAL)->getEnergy()==0) continue;
	  //don't compare to an already rejected cluster
	  if(charge==0 && ranges::contains(maskedParticles,other) ) continue;


	  //only count as split off neutron, if other had a PCAL hit
	  Position otherPos(other->CLAS12()->cal(clas12::PCAL)->getX(),
			    other->CLAS12()->cal(clas12::PCAL)->getY(),
			    other->CLAS12()->cal(clas12::PCAL)->getZ());
	  
	  auto diff= otherPos - hitPos; //now difference relative to gamma
	  if( diff.R()<rmin ){
	    maskIt=kTRUE;
	    maskedParticles.push_back(neut); //so don't remove both
	  }
	  hR.Fill(diff.R()); //histogram distance between clusters


	}
      };/////////////////////////////////////////////////////////

      //note all particles are contained in 0, -ve or _ve vecs
      compareToOther(0,_r0min,_hR0);
      
      compareToOther(PosID(),_rpmin,_hRp);

      compareToOther(NegID(),_rnmin,_hRm);
      
 
      if( maskIt == kFALSE){
	if(_neutID==2112)_vecNeutrons.push_back(neut);
	else _vec0.push_back(neut);
      }
      
      
    }
    
    
    return kTRUE;
  }
  
  
  void MaskCalorNeutronSplitOffs::PrintMask() const{
    Info("MaskCalorNeutronSplitOffs::PrintMask() ",Form("Masking EventParticles with  = %s",Class_Name()),"");
    Info("MaskCalorNeutronSplitOffs::PrintMask() ",Form("   cutting Neutron candidates at r0=%f , r+=%f , r-=%f .\n ",_r0min,_rpmin,_rnmin),"");
    Info("MaskCalorNeutronSplitOffs::PrintMask() ",Form("   using neutron PID value %d",_neutID),"");

  }
  /* Get particle ID info from topology manager
   */
  void MaskCalorNeutronSplitOffs::UseTopoInfo(TopologyManager& topoInfo, TString pidInfo, TString incInfo){
    _neutID=topoInfo.ParticleID(2112);
  }

}
