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
   
    //use my own gamma
    SetMapVector(22,&_vecGams);
    //which I am going to make from EventBuilder IDed gammas
    _originalGams=ep->GetParticleVector(22);
    //use my own neutrons
    SetMapVector(2112,&_vecNeutrons);
    //which I am going to make from EventBuilder IDed gammas
    _originalNeuts=ep->GetParticleVector(2112);
     //use my own all neutral vector for gammas
    SetMapVector(0,&_vec0);
    //which I am going to make from event particle, all neutrals
    _original0s=ep->GetParticleVector(0);
   
    //so we don't have to use the map in the event loop
    //Must call this at the end of any derived class AssignVectors
    SetPidVectors();
    
  }
  Bool_t MaskCalorSplitOffs::ReReadEvent(){
    using  Position= ROOT::Math::XYZPointF; //floating point position 

    MaskedEventParticles::ReReadEvent(); //set counters to 0
    
    _vecGams.clear();
    _vecNeutrons.clear();
    _vec0.clear();

    //Look for FD neutrals	
    auto fdGams=ranges::filter(*_originalGams,CheckForFD);
    auto fdNeuts=ranges::filter(*_originalNeuts,CheckForFD);
    auto fd0s=ranges::filter(*_original0s,CheckForFD);
    //Keep all non FD 
    ranges::append(ranges::filter(*_originalGams,CheckForNotFD),_vecGams);
    ranges::append(ranges::filter(*_originalNeuts,CheckForNotFD),_vecNeutrons);
    ranges::append(ranges::filter(*_original0s,CheckForNotFD),_vec0);


    //keep the hits with PCAL as possible real neutral candidates
    auto notpcalGams=ranges::filter(fdGams,CheckForPCAL);
    ranges::append(notpcalGams,_vecGams);
    auto notpcalNeuts=ranges::filter(fdNeuts,CheckForPCAL);
    ranges::append(notpcalNeuts,_vecNeutrons);
    auto notpcal0s=ranges::filter(fd0s,CheckForPCAL); 
    ranges::append(notpcal0s,_vec0);

    //remove neutrals with PCAL hit from split off candidates
    auto pcal0s=ranges::filter(fd0s,CheckForNotPCAL);
   
    //Loop over remaining 0s 
    for(auto const& gam:pcal0s){
      auto c12gam=static_cast<CLAS12Particle*>(gam)->CLAS12();
   
      Position	 hitPos;     
      //IF PCAL hit then cannot be split off!!
      //Now check if hit in ECIN
      if(c12gam->cal(clas12::ECIN)->getEnergy()!=0){
        hitPos.SetXYZ(c12gam->cal(clas12::ECIN)->getX(),
        c12gam->cal(clas12::ECIN)->getY(),
        c12gam->cal(clas12::ECIN)->getZ());
      }
      //Now check if hit in ECOUT
      else if(c12gam->cal(clas12::ECOUT)->getEnergy()!=0){
        hitPos.SetXYZ(c12gam->cal(clas12::ECOUT)->getX(),
        c12gam->cal(clas12::ECOUT)->getY(),
        c12gam->cal(clas12::ECOUT)->getZ());
      }

      Bool_t maskIt=kFALSE;
      particles_ptrs maskedParticles;
      
      ////////////////////////////////////////////////////////
      auto compareToOther =[&maskIt,&maskedParticles,&hitPos,gam,this]
      (const Short_t charge=0,const Float_t rmin,TH1F& hR){

        CLAS12Particle*  other{nullptr};
        UInt_t entry=0;
        while((other=static_cast<CLAS12Particle*>(NextParticle(charge,entry)))!=nullptr){
        
          if(other==gam) continue; //don't compare to myself!
          if(charge==0 && ranges::contains(maskedParticles,other) ) continue;//don't compare to an already rejected cluster

          //FD neutral split off parent must have PCAL hit 
          Position otherPos;
          if(charge==0){
            otherPos.SetXYZ(other->CLAS12()->cal(clas12::PCAL)->getX(),
                other->CLAS12()->cal(clas12::PCAL)->getY(),
                other->CLAS12()->cal(clas12::PCAL)->getZ());
          }//charged parent tracks can use trajectory
          else{
            otherPos.SetXYZ(other->CLAS12()->traj(clas12::ECAL,clas12::PCAL)->getX(),
                other->CLAS12()->traj(clas12::ECAL,clas12::PCAL)->getY(),
                other->CLAS12()->traj(clas12::ECAL,clas12::PCAL)->getZ());

          }
          if(otherPos.R()==0){
            continue;
          }	

          auto diffTh= ROOT::Math::VectorUtil::Angle(otherPos,hitPos)*TMath::RadToDeg(); //now difference relative to gamma
          if( diffTh<rmin ){
            maskIt=kTRUE;
            maskedParticles.push_back(gam); //so don't remove both

            if(charge==0&&_addSplits){

              //modifying vector, so create new one so other finalstates not effected
              if(ranges::contains(maskedParticles,other) == false){

                  if (std::find(_vecGams.begin(), _vecGams.end(), other) != _vecGams.end()){
                    auto oldother = other;
                    other =  ReplaceParticlePtr(22,other,NextFromPool()); 
                    ReplaceOnlyParticlePtr(0,oldother,other); //also update _vec0
                  }
                  else{
                    other =  ReplaceParticlePtr(0,other,NextFromPool()); 
                  }

                  maskedParticles.push_back(other); 
                }
                      
              //give my energy to the other
              other->SetP4(gam->P4()+other->P4());
              other->SetDetector(1);
            }
          }
          if(TMath::IsNaN(diffTh)==kFALSE)hR.Fill(diffTh); //histogram distance between neutral clusters
          
          if(maskIt==kTRUE) return;

        }
      };/////////////////////////////////////////////////////////

      //note all particles are contained in 0, -ve or _ve vecs
      compareToOther(0,_r0min,_hR0);
      
      if(maskIt==kFALSE) compareToOther(PosID(),_rpmin,_hRp);

      if(maskIt==kFALSE) compareToOther(NegID(),_rnmin,_hRm);
      
      //Don't mask it, put it back in vectors
      if( maskIt == kFALSE){
        if(gam->PDG()==22)_vecGams.push_back(gam);
        else  if(gam->PDG()==2112)_vecNeutrons.push_back(gam);	
        _vec0.push_back(gam);
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
