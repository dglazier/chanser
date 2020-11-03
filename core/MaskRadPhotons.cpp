#include "MaskRadPhotons.h"
#include "VectorUtility.h"


namespace chanser{

  MaskRadPhotons::~MaskRadPhotons(){
    Write(_hR);
    Write(_hdTheta);
    Write(_hdPhi);
    Write(_hdThetaR);
    Write(_hdThetadPhi);
    Write(_hdPhiR);

    /*TCanvas cR;
    _hR.Draw();
    cR.Draw();
    cR.Print("_hR.pdf","pdf");

    TCanvas cT;
    _hdTheta.Draw();
    cT.Draw();
    cT.Print("_hdTheta.pdf","pdf");

    TCanvas cP;
    _hdPhi.Draw();
    cP.Draw();
    cP.Print("_hdPhi.pdf","pdf");

    TCanvas cTR;
    _hdThetaR.Draw("colz");
    cTR.Draw();
    cTR.Print("_hdThetaR.pdf","pdf");

    TCanvas cTP;
    _hdThetadPhi.Draw("colz");
    cTP.Draw();
    cTP.Print("_hdThetadPhi.pdf","pdf");

    TCanvas cPR;
    _hdPhiR.Draw("colz");
    cPR.Draw();
    cPR.Print("_hdPhiR.pdf","pdf");*/
  }
  
  void MaskRadPhotons::AssignVectors(EventParticles* ep){
    //Just Copy all EventParticle vectors
    MaskedEventParticles::AssignVectors(ep);
    // return;
    //use my own gamma
    SetMapVector(22,&_vecGams);

    //which I am going to make from
    _originalGams=&ep->_vecGams;

     //so we don't have to use the map in the event loop
     //Must call this at the end of any derived class AssignVectors
    SetPidVectors();
 
  }
  Bool_t MaskRadPhotons::ReReadEvent(){
    using  Position= ROOT::Math::XYZPointF; //floating point position 

    
    _vecGams.clear();

    //remove photons with no PCAL hit
    auto pcalGams=ranges::filter(*_originalGams,CheckForPCAL);
    // std::cout<<"MaskCalorSplitOffs::ReReadEvent() do loop "<< pcalGams.size()<<std::endl;
    
    //Loop over remaining gammas 
    for(auto const& gam:pcalGams){
      auto r2d=TMath::RadToDeg();

      auto c12gam=static_cast<CLAS12Particle*>(gam)->CLAS12();
      Double_t gamTheta= c12gam->getTheta()*r2d;
      Double_t gamPhi= c12gam->getPhi()*r2d;
      Position gamPos(c12gam->cal(clas12::PCAL)->getX(),
		      c12gam->cal(clas12::PCAL)->getY(),
		      c12gam->cal(clas12::PCAL)->getZ());

      Bool_t maskIt=kFALSE;
      particles_ptrs maskedParticles;
      
      ////////////////////////////////////////////////////////
      auto compareToOther =[&maskIt,&maskedParticles,&gamTheta,&gamPhi,&gamPos,&r2d,gam,this]
	(const Double_t dTheta,const Double_t rmin,TH1F& hR,TH1F& hdT,TH1F& hdP,TH2F& hdTR,TH2F& hdTdP,TH2F& hdPR){

	CLAS12Particle*  other{nullptr};
	UInt_t entry=0;
	//get all electrons to check if they radiated a photon
	while((other=static_cast<CLAS12Particle*>
	       (NextParticle(11,entry)))!=nullptr){
	  
	  if(other==gam) continue; //don't compare to myself, should never happen here!
	  if(ranges::contains(maskedParticles,other) ) continue;//don't compare to an already rejected cluster
	  
	  Double_t otherTheta = other->CLAS12()->getTheta()*r2d;
	  Double_t otherPhi = other->CLAS12()->getPhi()*r2d;
	  Position otherPos(other->CLAS12()->cal(clas12::PCAL)->getX(),
			    other->CLAS12()->cal(clas12::PCAL)->getY(),
			    other->CLAS12()->cal(clas12::PCAL)->getZ());
	  
	  auto diffTheta= gamTheta - otherTheta; //difference in Theta
	  auto diffPhi= gamPhi - otherPhi; //difference in Phi, used for diagnostics
	  auto diffPos= otherPos - gamPos; //difference in PCAL position

	  if(abs(diffTheta)<dTheta && diffPos.R()>rmin){
	    maskIt=kTRUE;
	    maskedParticles.push_back(gam); //so don't remove both

	    if(_addSplits){
	      //give my energy to the electron
	      other->SetP4(gam->P4()+other->P4());
	    }
	  }
	  hR.Fill(diffPos.R()); //histogram distance in PCAL
	  hdT.Fill(diffTheta); //hist theta difference
	  hdP.Fill(diffPhi); //hist phi difference
	  hdTR.Fill(diffPos.R(),diffTheta); //hist dTheta v distance
	  hdTdP.Fill(diffPhi,diffTheta); //hist dTheta v dPhi
	  hdPR.Fill(diffPos.R(),diffPhi); //hist dPhi v distance


	}
      };/////////////////////////////////////////////////////////

      //Here other will always be an electron, will mask photon based on dTheta and ECAL distance
      compareToOther(_dTheta,_ecalR,_hR,_hdTheta,_hdPhi,_hdThetaR,_hdThetadPhi,_hdPhiR);
 
      if( maskIt == kFALSE)_vecGams.push_back(gam);
    }

    return kTRUE;
  }

  void MaskRadPhotons::PrintMask() const{
    Info("MaskRadPhotons::PrintMask() ",Form("Masking EventParticles with  = %s",Class_Name()),"");
    Info("MaskRadPhotons::PrintMask() ",Form("   cutting Gamma candidates at dTheta<%f r>%f .\n Will I combine splitoff neutrals ?  %d",_dTheta,_ecalR,(Int_t)_addSplits),"");
  }

}
