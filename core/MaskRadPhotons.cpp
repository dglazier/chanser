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
    Write(_hRN);
    Write(_hdThetaN);
    Write(_hdPhiN);
    Write(_hdThetaRN);
    Write(_hdThetadPhiN);
    Write(_hdPhiRN);

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
    _originalGams=&ep->_vecGams;
    _originalNeutrons=&ep->_vecNeutrons;

     //so we don't have to use the map in the event loop
     //Must call this at the end of any derived class AssignVectors
    SetPidVectors();
    
    //load the sampling fraction parameters from ccdb
    FillSamplingFractionParams();

  }
  Bool_t MaskRadPhotons::ReReadEvent(){
    using  Position= ROOT::Math::XYZPointF; //floating point position 

    
    _vecGams.clear();
    _vecNeutrons.clear();

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
      auto r2d=TMath::RadToDeg();

      Bool_t maskIt=kFALSE;
      particles_ptrs maskedParticles;
      
      ////////////////////////////////////////////////////////
      auto compareToOther =[&maskIt,&maskedParticles,&r2d,radPart,&neutrons,this]
	(const int pid, const Double_t dTheta,const Double_t rmin,TH1F& hR,TH1F& hdT,TH1F& hdP,TH2F& hdTR,TH2F& hdTdP,TH2F& hdPR,TH1F& hP){

	auto c12RadPart=static_cast<CLAS12Particle*>(radPart)->CLAS12();
	Double_t partTheta= c12RadPart->getTheta();
	Double_t partPhi= c12RadPart->getPhi();
	HSPosition partPos(c12RadPart->cal(clas12::PCAL)->getX(),
			 c12RadPart->cal(clas12::PCAL)->getY(),
			 c12RadPart->cal(clas12::PCAL)->getZ());

	CLAS12Particle*  other{nullptr};
	UInt_t entry=0;
	//get all electrons to check if they radiated a photon
	while((other=static_cast<CLAS12Particle*>
	       (NextParticle(pid,entry)))!=nullptr){
	  
	  if(other==radPart) continue; //don't compare to myself, should never happen here!
	  if(ranges::contains(maskedParticles,other) ) continue;//don't compare to an already rejected cluster
	  
	  Double_t otherTheta = other->CLAS12()->getTheta();
	  Double_t otherPhi = other->CLAS12()->getPhi();
	  HSPosition otherPos(other->CLAS12()->cal(clas12::PCAL)->getX(),
			    other->CLAS12()->cal(clas12::PCAL)->getY(),
			    other->CLAS12()->cal(clas12::PCAL)->getZ());
	  
	  auto diffTheta= (partTheta - otherTheta)*r2d; //difference in Theta
	  auto diffPhi= (partPhi - otherPhi)*r2d; //difference in Phi, used for diagnostics
	  auto diffPos= otherPos - partPos; //difference in PCAL position

	  if(abs(diffTheta)<dTheta && diffPos.R()>rmin){
	    maskIt=kTRUE;
	    maskedParticles.push_back(radPart); //so don't remove both

	    if(_addSplits){
	      if(neutrons){
		Float_t EdepPCAL = c12RadPart->cal(clas12::PCAL)->getEnergy();
		Float_t EdepECIN = c12RadPart->cal(clas12::ECIN)->getEnergy();
		Float_t EdepECOUT = c12RadPart->cal(clas12::ECOUT)->getEnergy();
		Float_t EdepTot = EdepPCAL+EdepECIN+EdepECOUT;

		Float_t reP = EdepTot/GetMeanSF(EdepTot);
		HSLorentzVector newNP4(reP*sin(partTheta)*cos(partPhi),reP*sin(partTheta)*sin(partPhi),reP*cos(partPhi), reP*reP);
		other->SetP4(newNP4+other->P4());
		hP.Fill(reP); //hist momentum
	      } else{
		other->SetP4(radPart->P4()+other->P4());
		hP.Fill(c12RadPart->getP()); //hist momentum
	      }
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

      //will mask photon based on dTheta and ECAL distance
      if(neutrons){
	compareToOther(11,_dTheta,_ecalR,_hRN,_hdThetaN,_hdPhiN,_hdThetaRN,_hdThetadPhiN,_hdPhiRN,_hPN);
	compareToOther(-11,_dTheta,_ecalR,_hRN,_hdThetaN,_hdPhiN,_hdThetaRN,_hdThetadPhiN,_hdPhiRN,_hPN);
      } else {
	compareToOther(11,_dTheta,_ecalR,_hR,_hdTheta,_hdPhi,_hdThetaR,_hdThetadPhi,_hdPhiR,_hP);
	compareToOther(-11,_dTheta,_ecalR,_hR,_hdTheta,_hdPhi,_hdThetaR,_hdThetadPhi,_hdPhiR,_hP);
      }
 
      if( maskIt == kFALSE)_vecGams.push_back(radPart);
    }
  }

  void MaskRadPhotons::PrintMask() const{
    Info("MaskRadPhotons::PrintMask() ",Form("Masking EventParticles with  = %s",Class_Name()),"");
    Info("MaskRadPhotons::PrintMask() ",Form("   cutting Gamma candidates at dTheta<%f degrees r>%f cm.\n Will I combine splitoff neutrals ?  %d",_dTheta,_ecalR,(Int_t)_addSplits),"");
  }

  //Load sampling fraction parameters from ccdb
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
