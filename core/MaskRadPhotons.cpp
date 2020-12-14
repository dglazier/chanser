#include "MaskRadPhotons.h"
#include "VectorUtility.h"
#include "CLAS12FinalState.h"
  


namespace chanser{

  MaskRadPhotons::~MaskRadPhotons(){
    Write(_hR);
    Write(_hdTheta);
    Write(_hdPhi);
    Write(_hdThetaR);
    Write(_hdThetadPhi);
    Write(_hdPhiR);
    Write(_hP);
    Write(_hPx);
    Write(_hPy);
    Write(_hPz);
    Write(_hRN);
    Write(_hdThetaN);
    Write(_hdPhiN);
    Write(_hdThetaRN);
    Write(_hdThetadPhiN);
    Write(_hdPhiRN);
    Write(_hPN);
    Write(_hPxN);
    Write(_hPyN);
    Write(_hPzN);

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
    cPR.Print("_hdPhiR.pdf","pdf");

    TCanvas cPm;
    _hP.Draw();
    cPm.Draw();
    cPm.Print("_hP.pdf","pdf");

    TCanvas cRN;
    _hRN.Draw();
    cRN.Draw();
    cRN.Print("_hRN.pdf","pdf");

    TCanvas cTN;
    _hdThetaN.Draw();
    cTN.Draw();
    cTN.Print("_hdThetaN.pdf","pdf");

    TCanvas cPN;
    _hdPhiN.Draw();
    cPN.Draw();
    cPN.Print("_hdPhiN.pdf","pdf");

    TCanvas cTRN;
    _hdThetaRN.Draw("colz");
    cTRN.Draw();
    cTRN.Print("_hdThetaRN.pdf","pdf");

    TCanvas cTPN;
    _hdThetadPhiN.Draw("colz");
    cTPN.Draw();
    cTPN.Print("_hdThetadPhiN.pdf","pdf");

    TCanvas cPRN;
    _hdPhiRN.Draw("colz");
    cPRN.Draw();
    cPRN.Print("_hdPhiRN.pdf","pdf");

     TCanvas cPmN;
    _hPN.Draw();
    cPmN.Draw();
    cPmN.Print("_hPN.pdf","pdf");*/
  }
  
  void MaskRadPhotons::AssignVectors(EventParticles* ep){
    //Just Copy all EventParticle vectors
    MaskedEventParticles::AssignVectors(ep);
    // return;
    //use my own gamma and neutrons
    SetMapVector(22,&_vecGams);
    SetMapVector(2112,&_vecNeutrons);

    //which I am going to make from
    _originalGams=ep->GetParticleVector(22);
    _originalNeutrons=ep->GetParticleVector(2112);
    
    //particle 4-vectors that might change
    SetMapVector(11,&_vecEls);
    _originalEls=ep->GetParticleVector(11);
    SetMapVector(-11,&_vecPos);
    _originalPos=ep->GetParticleVector(-11);
 
    //so we don't have to use the map in the event loop
    //Must call this at the end of any derived class AssignVectors
    SetPidVectors();
    
  }
  Bool_t MaskRadPhotons::ReReadEvent(){
    using  Position= ROOT::Math::XYZPointF; //floating point position 

    MaskedEventParticles::ReReadEvent(); //set counters to 0
    
    _vecGams.clear();
    _vecNeutrons.clear();

    //copy els and pos as we may modify those
    ranges::copy(*_originalEls,_vecEls);
    ranges::copy(*_originalPos,_vecPos);
 
    //remove photons with no PCAL hit
    auto pcalGams=ranges::filter(*_originalGams,CheckForPCAL);
    auto pcalNeutrons=ranges::filter(*_originalNeutrons,CheckForPCAL);
 
    doCorrection(pcalGams,false);
    doCorrection(pcalNeutrons,true);

    return kTRUE;
  }

  /* Function responsible for the corrections, separated from ReReadEvent
   * to run over gammas and mis-IDed neutrons separately.
   * The neutrons also require recalculation of their momentum based on the 
   * sampling fraction parametrisation of photons.
   */
  void MaskRadPhotons::doCorrection(std::vector<chanser::BaseParticle*> radParts, bool neutrons){

    //Loop over remaining gammas 
    for(auto const& radPart:radParts){

      Bool_t maskIt=kFALSE; //assume we are going to include this gamma in data
       
      ////////////////////////////////////////////////////////
      auto compareToOther =[&maskIt,radPart,&neutrons,this]
	(const int pid, const Double_t dTheta,const Double_t rmin,TH1F& hR,TH1F& hdT,TH1F& hdP,TH2F& hdTR,TH2F& hdTdP,TH2F& hdPR,TH1F& hP,TH1F& hPx,TH1F& hPy,TH1F& hPz){
  	auto c12RadPart=static_cast<CLAS12Particle*>(radPart)->CLAS12();
	Double_t partTheta= c12RadPart->getTheta();
	Double_t partPhi= c12RadPart->getPhi();
	auto partPos= HSPosition(c12RadPart->cal(clas12::PCAL)->getX(),
		       c12RadPart->cal(clas12::PCAL)->getY(),
		       c12RadPart->cal(clas12::PCAL)->getZ());

	CLAS12Particle*  lepton{nullptr};
	UInt_t entry=0;
	particles_ptrs maskedParticles;

	//get all electrons to check if they radiated a photon
	while((lepton=static_cast<CLAS12Particle*>
	       (NextParticle(pid,entry)))!=nullptr){//lepton loop

	  //shouldn't need the following lines as different ParticleVectors radPat!= a lepton, maskedPArticle can only be gamma or neutron
	  //if(lepton==radPart) continue; //don't compare to myself, should never happen here!
	  //if(ranges::contains(maskedParticles,lepton) ) continue;//don't compare to an already rejected cluster
	  
	  Double_t leptonTheta = lepton->CLAS12()->getTheta();
	  Double_t leptonPhi = lepton->CLAS12()->getPhi();
	  auto leptonPos=HSPosition(lepton->CLAS12()->cal(clas12::PCAL)->getX(),
			   lepton->CLAS12()->cal(clas12::PCAL)->getY(),
			   lepton->CLAS12()->cal(clas12::PCAL)->getZ());
	  
	  auto diffTheta= (partTheta - leptonTheta)*TMath::RadToDeg(); //difference in Theta
	  auto diffPhi= (partPhi - leptonPhi)*TMath::RadToDeg(); //difference in Phi, used for diagnostics
	  auto diffPos= leptonPos - partPos; //difference in PCAL position

	  if(abs(diffTheta)<dTheta && diffPos.R()>rmin){
	    maskIt=kTRUE;

	    
	    if(_addSplits){
	      
	      //going to modify lepton, so replace with local copy pointer
	      if(ranges::contains(maskedParticles,lepton) == false){
		lepton =  ReplaceParticlePtr(pid,lepton,NextFromPool()); 		
		maskedParticles.push_back(lepton); 
	      }
 
	      if(neutrons){
		Float_t EdepPCAL = c12RadPart->cal(clas12::PCAL)->getEnergy();
		Float_t EdepECIN = c12RadPart->cal(clas12::ECIN)->getEnergy();
		Float_t EdepECOUT = c12RadPart->cal(clas12::ECOUT)->getEnergy();
		Float_t EdepTot = EdepPCAL+EdepECIN+EdepECOUT;

		Float_t reP = EdepTot/GetMeanSF(EdepTot);
		HSLorentzVector newNP4(reP*sin(partTheta)*cos(partPhi),reP*sin(partTheta)*sin(partPhi),reP*cos(partPhi), reP*reP);

		//reassign e lorentzvector
		lepton->SetP4(newNP4+lepton->P4());
		//histogram
		hP.Fill(reP); //hist momentum
		hPx.Fill(newNP4.Px()); //hist momentum x
		hPy.Fill(newNP4.Py()); //hist momentum y
		hPz.Fill(newNP4.Pz()); //hist momentum z
	      } else{
		//reassign e lorentzvector
		lepton->SetP4(radPart->P4()+lepton->P4());

		//histogram
		hP.Fill(c12RadPart->getP()); //hist momentum
		hPx.Fill(c12RadPart->par()->getPx()); //hist momentum x
		hPy.Fill(c12RadPart->par()->getPy()); //hist momentum y
		hPz.Fill(c12RadPart->par()->getPz()); //hist momentum z
	      }
	      
	    }
	  }
	  hR.Fill(diffPos.R()); //histogram distance in PCAL
	  hdT.Fill(diffTheta); //hist theta difference
	  hdP.Fill(diffPhi); //hist phi difference
	  hdTR.Fill(diffPos.R(),diffTheta); //hist dTheta v distance
	  hdTdP.Fill(diffPhi,diffTheta); //hist dTheta v dPhi
	  hdPR.Fill(diffPos.R(),diffPhi); //hist dPhi v distance

	  if(maskIt==kTRUE) break; //already assigned to lepton


	}//lepton loop
      };/////////////////////////////////////////////////////////

      //will mask photon based on dTheta and ECAL distance
      if(neutrons){
	compareToOther(_elID,_dTheta,_ecalR,_hRN,_hdThetaN,_hdPhiN,_hdThetaRN,_hdThetadPhiN,_hdPhiRN,_hPN,_hPxN,_hPyN,_hPzN);
	compareToOther(_posID,_dTheta,_ecalR,_hRN,_hdThetaN,_hdPhiN,_hdThetaRN,_hdThetadPhiN,_hdPhiRN,_hPN,_hPxN,_hPyN,_hPzN);
      } else {
	compareToOther(_elID,_dTheta,_ecalR,_hR,_hdTheta,_hdPhi,_hdThetaR,_hdThetadPhi,_hdPhiR,_hP,_hPx,_hPy,_hPz);
	compareToOther(_posID,_dTheta,_ecalR,_hR,_hdTheta,_hdPhi,_hdThetaR,_hdThetadPhi,_hdPhiR,_hP,_hPx,_hPy,_hPz);
      }
      
      //This gamma is fine, include it in data
      if( maskIt == kFALSE)_vecGams.push_back(radPart);
    }
  }

  void MaskRadPhotons::PrintMask() const{
    Info("MaskRadPhotons::PrintMask() ",Form("Masking EventParticles with  = %s",Class_Name()),"");
    Info("MaskRadPhotons::PrintMask() ",Form("   cutting neutral candidates at dTheta<%f degrees r>%f cm.\n Will I combine radiated neutrals?  %d",_dTheta,_ecalR,(Int_t)_addSplits),"");
  }

  //Load sampling fraction parameters from ccdb
  void  MaskRadPhotons::ChangeRun(FinalState* fs) {
    auto c12fs=dynamic_cast<CLAS12FinalState*>(fs);
    if(c12fs!=nullptr){
      auto c12reader = c12fs->GetEventInfo()->CLAS12();
      auto& ccdbPhSF = c12reader->ccdb()->requestTableDoubles("/calibration/eb/photon_sf");

      _sfPa=ccdbPhSF[0][3]; //0.250 
      _sfPb=ccdbPhSF[0][4]; //1.029
      _sfPc=ccdbPhSF[0][5]; //-0.015
      _sfPd=ccdbPhSF[0][6]; // 0.00012
    std:cout<<"MaskRadPhotons::ChangeRun "<<_sfPa<<" "<<_sfPb<<" "<<_sfPc<<" "<<_sfPd<<std::endl;

    }
    else{
      std::cerr<< " MaskRadPhotons::ChangeRun final state is not a CLAS12FinalState  "<<std::endl;
      exit(0);
    }
  }

  void MaskRadPhotons::FillSamplingFractionParams(){

    if(_sfPa==0){_sfPa=0.250;} 
    if(_sfPb==0){_sfPb=1.029;}
    if(_sfPc==0){_sfPc=-0.015;}
    if(_sfPd==0){_sfPd=0.00012;}

    /*
      Wait to have database code!!

    auto& ccdbPhSF = GetCLAS12().ccdb()->requestTableDoubles("/calibration/eb/photon_sf");

    _sfPa=ccdbPhSF[0][3]; //0.250 
    _sfPb=ccdbPhSF[0][4]; //1.029
    _sfPc=ccdbPhSF[0][5]; //-0.015
    _sfPd=ccdbPhSF[0][6]; // 0.00012
    */
  }

  /*
   * Returns the mean value of the sampling fraction at a given
   * energy deposition in all three FD calorimeters.
   * See RG-A analysis note for more details on the parametrisation.
   */
  Float_t MaskRadPhotons::GetMeanSF(Float_t  Edep){
    return _sfPa*(_sfPb+(_sfPc/Edep)+(_sfPd/(Edep*Edep)));
  }

}
