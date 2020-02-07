//////////////////////////////////////////////////////////////
///
///Class:		Kinematics
///Description:
///

#pragma once

#include <TLorentzVector.h> 
#include <Math/VectorUtil.h> //for boosts etc.
#include <Math/Vector4D.h>


namespace chanzer{
    
  using ROOT::Math::VectorUtil::boost;
  using ROOT::Math::VectorUtil::Angle;
  using ROOT::Math::VectorUtil::RotateZ;
  using  HSLorentzVector=ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<Double32_t> > ;
  using  HSPosition= ROOT::Math::PositionVector3D< ROOT::Math::Cartesian3D< Double32_t >, ROOT::Math::DefaultCoordinateSystemTag >;
    
  using  HSMomentum=ROOT::Math::DisplacementVector3D< ROOT::Math::Cartesian3D< Double_t >, ROOT::Math::DefaultCoordinateSystemTag >;


  class Kinematics{
 
  private :
    const HSLorentzVector* fElin{nullptr};
    const HSLorentzVector* fElsc{nullptr};
    HSLorentzVector fGamma;
    HSLorentzVector fCM;
    const HSLorentzVector* fTar{nullptr};
    const HSLorentzVector* fMes{nullptr};
    const HSLorentzVector* fBar{nullptr};
    const HSLorentzVector* fMes_d1{nullptr};
    const HSLorentzVector* fMes_d2{nullptr};
    const HSLorentzVector* fBar_d1{nullptr};
    const HSLorentzVector* fBar_d2{nullptr};
    
    HSMomentum fCMBoost;
    
    Double_t fCosTh=0;
    Double_t fPhi=0;
    Double_t fCosx=0;
    Double_t fCosy=0;
    Double_t fCosz=0;
    
  public :
    Kinematics(){};
    ~Kinematics(){};
    
    Double_t CosTheta() const noexcept{return fCosTh;}
    Double_t Phi() const noexcept{return fPhi;}
    
    void SetTarget(HSLorentzVector& tar){fTar=&tar;};
    //    void SetTarget(TLorentzVector& tar){fTar=HSLorentzVector(tar.X(),tar.Y(),tar.Z(),tar.T());}
    
    /////////////////////////////////////////////////////////////
    /// Set electron scattering and target 4-vectors \n
    /// Use them to calculate cm 4-vector
    void SetElecsTarget(const HSLorentzVector& in,const HSLorentzVector& sc,const HSLorentzVector& tar){
      fElin=&in;
      fElsc=&sc;
      fGamma=*fElin-*fElsc;
      fTar=&tar;
      fCM=fGamma+*fTar;
      fCMBoost=fCM.BoostToCM();
    }
    /* void SetElecsTarget(TLorentzVector& in,TLorentzVector sc,TLorentzVector tar){ */
    /*   fElin=HSLorentzVector(in.X(),in.Y(),in.Z(),in.T()); */
    /*   fElsc=HSLorentzVector(sc.X(),sc.Y(),sc.Z(),sc.T()); */
    /*   fGamma=fElin-fElsc; */
    /*   fTar=HSLorentzVector(tar.X(),tar.Y(),tar.Z(),tar.T()); */
    /*   fCM=fGamma+fTar; */
    /*   fCMBoost=fCM.BoostToCM(); */
    /* } */
    /////////////////////////////////////////////////////////////
    /// Set incoming photon and target 4-vectors \n
    /// Use them to calculate cm 4-vector
    /* void SetGammaTarget(HSLorentzVector gamma,HSLorentzVector tar){ */
    /*   fGamma=gamma; */
    /*   fTar=tar; */
    /*   fCM=gamma+tar; */
    /*   fCMBoost=fCM.BoostToCM(); */
    /* } */
    /* void SetGammaTarget(TLorentzVector gamma,TLorentzVector tar){ */
    /*   fGamma=HSLorentzVector(gamma.X(),gamma.Y(),gamma.Z(),gamma.T()); */
    /*   fTar=HSLorentzVector(tar.X(),tar.Y(),tar.Z(),tar.T()); */
    /*   fCM=fGamma+fTar; */
    /*   fCMBoost=fCM.BoostToCM(); */
    /* } */
    ///////////////////////////////////////////////////////////
    /// Set primary decay products of the CM system. 
    void SetMesonBaryon(const HSLorentzVector& mes,const HSLorentzVector& bar){
      fMes=&mes;
      fBar=&bar;
    }
    /* void SetMesonBaryon(TLorentzVector mes,TLorentzVector bar){ */
    /*   fMes=HSLorentzVector(mes.X(),mes.Y(),mes.Z(),mes.T()); */
    /*   fBar=HSLorentzVector(bar.X(),bar.Y(),bar.Z(),bar.T()); */
    /* } */
    ///////////////////////////////////////////////////////////
    /// Set decay products of the meson. \n
    /// d1 will be used to calculate angles in GJ and helicity frame.
    void SetMesonDecay(const HSLorentzVector& d1,const HSLorentzVector& d2){
      fMes_d1=&d1;
      fMes_d2=&d2;
    }
    /* void SetMesonDecay(TLorentzVector& d1,TLorentzVector d2){ */
    /*   fMes_d1=HSLorentzVector(d1.X(),d1.Y(),d1.Z(),d1.T()); */
    /*   fMes_d2=HSLorentzVector(d2.X(),d2.Y(),d2.Z(),d2.T()); */
    /* } */
    ///////////////////////////////////////////////////////////
    /// Set decay products of the baryon. \n
    /// d1 will be used to calculate angles in GJ and helicity frame.
    void SetBaryonDecay(const HSLorentzVector& d1,const HSLorentzVector& d2){
      fBar_d1=&d1;
      fBar_d2=&d2;
    }
    /* void SetBaryonDecay(TLorentzVector d1,TLorentzVector d2){ */
    /*   fBar_d1=HSLorentzVector(d1.X(),d1.Y(),d1.Z(),d1.T()); */
    /*   fBar_d2=HSLorentzVector(d2.X(),d2.Y(),d2.Z(),d2.T()); */
    /* } */
    
    HSLorentzVector ParticleXYZM(Double_t X,Double_t Y,Double_t Z,Double_t M){HSLorentzVector p4;p4.SetXYZT(X,Y,Z,sqrt(M*M+X*X+Y*Y+Z*Z)); return p4;}
    
    
    const HSLorentzVector& Gamma() const noexcept{return fGamma;}
    const HSMomentum& BoostToCM() const noexcept{return fCMBoost;}
    Double_t W() const noexcept{ return fCM.M();}
    Double_t Q2() const noexcept {return -fGamma.M2();}
    Double_t GammaE() const noexcept {return fGamma.E();}
    Double_t GammaTh() const noexcept {return fGamma.Theta();}
    Double_t ElScatTh() const noexcept {return fElsc->Theta();}
    Double_t GammaPol() const noexcept{
      //  auto tanel=TMath::Tan(ElScatTh()/2);
      auto thetael2=ElScatTh()/2;
      auto tanel=TMath::Tan(thetael2);
      return 1./(1+2*(1+GammaE()*GammaE()/Q2())*tanel*tanel);
    }
    //    Double_t GammaPol() const noexcept{return 1./(1+2*(1+GammaE()*GammaE()/Q2())*TMath::Tan(ElScatTh()/2)*TMath::Tan(ElScatTh()/2));}
    Double_t GammaFlux() const noexcept{return(1./137/4/TMath::Pi()/TMath::Pi())/fElin->E()*fElsc->E()*((W()*W()-fTar->M2())/fTar->M())/Q2()/(1-GammaPol()); }
    
    
    Double_t t() const noexcept{return t(fGamma,*fMes);}//default gamma-meson
    Double_t t(const HSLorentzVector& p0,const HSLorentzVector& p1) const noexcept{
      // HSLorentzVector transfer=p0-p1;
      //return transfer.M2();
      return (p0-p1).M2();
    }
    /* Double_t t(TLorentzVector p0,TLorentzVector p1){ */
    /*   // HSLorentzVector transfer=p0-p1; */
    /*   //return transfer.M2(); */
    /*   return (p0-p1).M2(); */
    /* } */
    Double_t t0() const noexcept{return t0(fGamma,*fMes);}//default gamma-meson
    Double_t t0(const HSLorentzVector& p0,const HSLorentzVector& p1) const noexcept;
    //Double_t t0(TLorentzVector p0,TLorentzVector p1){return t0(HSLorentzVector(p0.X(),p0.Y(),p0.Z(),p0.T()),HSLorentzVector(p1.X(),p1.Y(),p1.Z(),p1.T()));};
    
    Double_t Cosx() const noexcept{return fCosx;}  
    Double_t Cosy() const noexcept{return fCosy;}  
    Double_t Cosz() const noexcept{return fCosz;}
    
    Double_t CosThetaVector(HSMomentum *mm) const noexcept{return TMath::Cos(mm->Theta());}
    Double_t CosThetaVector(HSLorentzVector *mm) const noexcept{return TMath::Cos(mm->Theta());}
    //Decay angles
    void MesonDecayHelicity();
    void MesonDecayGJ();
    void BaryonDecayHelicity();
    void BaryonDecayGJ();
    void omegaMesonDecayHelicity();
    void omegaMesonDecayGJ();
    void ElectroCMDecay();
    void PhotoCMDecay();
    void LambdaDecay();
    void PolPhotoCMDecay();
  };
  
  inline Double_t Kinematics::t0(const HSLorentzVector& p0,const HSLorentzVector& p1) const noexcept{
    //THIS FUNCTION IS PROBABLY NOT CORRECT AND NEEDS CHECKED
    HSLorentzVector cm0=boost(p0,fCMBoost);
    HSLorentzVector cm1=boost(p1,fCMBoost);
    
    Double_t P0CM=cm0.P();
    Double_t P1CM=cm1.P();
    
    Double_t t0=(p1.M2()/2/fCM.M())*(p1.M2()/2/fCM.M())-(P0CM-P1CM)*(P0CM-P1CM);
    return t(p0,p1) - t0;
  }
  
  inline void Kinematics::ElectroCMDecay(){
    //CM frame defined by e-scattering
    HSLorentzVector CMBeam=boost(*fElin,fCMBoost);
    // HSLorentzVector CMScat=boost(fElsc,fCMBoost);
    HSLorentzVector CMMes=boost(*fMes,fCMBoost);
    HSLorentzVector CMGamma=boost(fGamma,fCMBoost);
    
    HSMomentum zV=CMGamma.Vect().Unit();
    HSMomentum yV=CMGamma.Vect().Cross(CMBeam.Vect()).Unit();
    HSMomentum xV=yV.Cross(zV).Unit();
    
    HSMomentum angles(CMMes.Vect().Dot(xV),CMMes.Vect().Dot(yV),CMMes.Vect().Dot(zV));
    fCosTh=CosThetaVector(&angles);
    fPhi=angles.Phi();

     
  }
  /* //////////////////////////////////////////////////////// */
  /* ///CM frame defined by CM boost. No rotation to lab phi. */
  /* inline void Kinematics::PhotoCMDecay(){ */
  /*   HSLorentzVector CMMes=boost(fMes,fCMBoost); */
  /*   fCosTh=CosThetaVector(&CMMes); */
  /*   fPhi=CMMes.Phi(); */
  /* } */
  ////////////////////////////////////////////////////////
  /// From Daria Sokhan thesis \n
  /// Assuming polarisation vector is (0,1,0) (the E vector) and B vector (1,0,0), these are components of the boosted E-vector.  
  /* inline void Kinematics::PolPhotoCMDecay(){ */
  /*   Double_t gamma=fCM.Gamma(); */
  /*   Double_t beta=fCM.Beta(); */
    
  /*   Double_t E1 = (gamma * (gamma-1.) * fCMBoost.X() * fCMBoost.Y() / (beta*beta)) - (fCMBoost.X() * fCMBoost.Y() * gamma*gamma); */
    
  /*   Double_t E2 = (fCMBoost.Z() * gamma) - (fCMBoost.Y()*(fCMBoost.Y()) * gamma*gamma) + gamma + (gamma * (gamma-1.) * pow(fCMBoost.Y(),2) / (beta*beta)); */
    
  /*   Double_t E3 = (gamma * (gamma-1.) * fCMBoost.Y() * fCMBoost.Z() / (beta*beta)) - (fCMBoost.Y() * gamma) - (fCMBoost.Y() * fCMBoost.Z() * gamma*gamma); */
    
  /*   HSMomentum Polarisation(E1,E2,E3); //photon pol in CM  */
  /*   Polarisation=RotateZ(Polarisation,TMath::Pi()/2); */
  /*   HSLorentzVector CMGamma=boost(fGamma,fCMBoost); */
    
  /*   HSMomentum zV=CMGamma.Vect().Unit(); */
  /*   HSMomentum yV=Polarisation.Unit().Cross(CMGamma.Vect()).Unit(); */
  /*   HSMomentum xV=yV.Cross(zV).Unit(); */
    
  /*   HSLorentzVector CMMes=boost(fMes,fCMBoost); */
  /*   HSMomentum angles(CMMes.Vect().Dot(xV),CMMes.Vect().Dot(yV),CMMes.Vect().Dot(zV)); */
  /*   fCosTh=CosThetaVector(&angles); */
  /*   fPhi=angles.Phi(); */
    
  /* } */
  
  /////////////////////////////////////////////////////
  ///z-axis along -baryon in meson rest frame
  inline void Kinematics::MesonDecayHelicity(){
    HSMomentum decBoost=fMes->BoostToCM();
    HSLorentzVector decBar=boost(*fBar,decBoost);
    HSLorentzVector decGamma=boost(fGamma,decBoost);
    HSMomentum zV=-decBar.Vect().Unit();
    HSMomentum yV=decBar.Vect().Cross(decGamma.Vect()).Unit();
    HSMomentum xV=yV.Cross(zV).Unit();
    
    HSLorentzVector decD1=boost(*fMes_d1,decBoost);
    
    HSMomentum angles(decD1.Vect().Dot(xV),decD1.Vect().Dot(yV),decD1.Vect().Dot(zV));
    fCosTh=CosThetaVector(&angles);
    fPhi=angles.Phi();
  }
  
  ////////////////////////////////////////////////////////
  ///z-axis along gamma direction in meson rest frame
  inline void Kinematics::MesonDecayGJ(){
    HSMomentum decBoost=fMes->BoostToCM();
    HSLorentzVector decBar=boost(*fBar,decBoost);
    HSLorentzVector decGamma=boost(fGamma,decBoost);
    HSMomentum zV=decGamma.Vect().Unit();
    HSMomentum yV=decBar.Vect().Cross(decGamma.Vect()).Unit();
    HSMomentum xV=yV.Cross(zV).Unit();
    
    HSLorentzVector decD1=boost(*fMes_d1,decBoost);
    
    HSMomentum angles(decD1.Vect().Dot(xV),decD1.Vect().Dot(yV),decD1.Vect().Dot(zV));
    fCosTh=CosThetaVector(&angles);
    fPhi=angles.Phi();
  }
  
  //////////////////////////////////////////////////////
  ///z-axis along -meson in baryon rest frame
  inline void Kinematics::BaryonDecayHelicity(){
    HSMomentum decBoost=fBar->BoostToCM();
    HSLorentzVector decMes=boost(*fMes,decBoost);
    HSLorentzVector decGamma=boost(fGamma,decBoost);
    HSMomentum zV=-decMes.Vect().Unit();
    HSMomentum yV=decGamma.Vect().Cross(decMes.Vect()).Unit();
    HSMomentum xV=yV.Cross(zV).Unit();
    
    HSLorentzVector decD1=boost(*fBar_d1,decBoost);
    
    HSMomentum angles(decD1.Vect().Dot(xV),decD1.Vect().Dot(yV),decD1.Vect().Dot(zV));
    fCosTh=CosThetaVector(&angles);
    fPhi=angles.Phi();
  }
  
  //////////////////////////////////////////////////////
  ///z-axis along -target direction in baryon rest frame
  inline void Kinematics::BaryonDecayGJ(){
    HSMomentum decBoost=fBar->BoostToCM();
    HSLorentzVector decMes=boost(*fMes,decBoost);
    HSLorentzVector decGamma=boost(fGamma,decBoost);
    HSLorentzVector decTar=boost(*fTar,decBoost);
    HSMomentum zV=-decTar.Vect().Unit();
    HSMomentum yV=decGamma.Vect().Cross(decMes.Vect()).Unit();
    HSMomentum xV=yV.Cross(zV).Unit();
    
    HSLorentzVector decD1=boost(*fBar_d1,decBoost);
    
    HSMomentum angles(decD1.Vect().Dot(xV),decD1.Vect().Dot(yV),decD1.Vect().Dot(zV));
    fCosTh=CosThetaVector(&angles);
    fPhi=angles.Phi();
  }
  
  /////////////////////////////////////////////////////
  ///z-axis along -baryon in meson rest frame
  inline void Kinematics::omegaMesonDecayHelicity(){
    HSMomentum decBoost=fMes->BoostToCM();
    HSLorentzVector decBar=boost(*fBar,decBoost);
    HSLorentzVector decGamma=boost(fGamma,decBoost);
    HSMomentum zV=-decBar.Vect().Unit();
    HSMomentum yV=decBar.Vect().Cross(decGamma.Vect()).Unit();
    HSMomentum xV=yV.Cross(zV).Unit();
    
    HSLorentzVector decD1=boost(*fMes_d1,decBoost);
    HSLorentzVector decD2=boost(*fMes_d2,decBoost);
    
    HSMomentum normal=(decD1.Vect().Cross(decD2.Vect())).Unit();
    
    HSMomentum angles(normal.Dot(xV),normal.Dot(yV),normal.Dot(zV));
    fCosTh=CosThetaVector(&angles);
    fPhi=angles.Phi();
    
  }
  ////////////////////////////////////////////////////////
  ///z-axis along gamma direction in meson rest frame
  inline void Kinematics::omegaMesonDecayGJ(){
    HSMomentum decBoost=fMes->BoostToCM();
    HSLorentzVector decBar=boost(*fBar,decBoost);
    HSLorentzVector decGamma=boost(fGamma,decBoost);
    HSMomentum zV=decGamma.Vect().Unit();
    HSMomentum yV=decBar.Vect().Cross(decGamma.Vect()).Unit();
    HSMomentum xV=yV.Cross(zV).Unit();
    
    HSLorentzVector decD1=boost(*fMes_d1,decBoost);
    HSLorentzVector decD2=boost(*fMes_d2,decBoost);
    
    HSMomentum normal=(decD1.Vect().Cross(decD2.Vect())).Unit();
    
    HSMomentum angles(normal.Dot(xV),normal.Dot(yV),normal.Dot(zV));
    fCosTh=CosThetaVector(&angles);
    fPhi=angles.Phi();
  }
  //////////////////////////////////////////////////////
  ///z-axis along gamma direction in meson decay frame
  inline void Kinematics::LambdaDecay(){
    //z-axis along gamma direction in meson decay frame
    HSMomentum decBoost=fBar->BoostToCM();
    HSLorentzVector decBar=boost(*fBar,decBoost);
    HSLorentzVector decGamma=boost(fGamma,decBoost);
    HSMomentum zV=decGamma.Vect().Unit();
    HSMomentum yV=decBar.Vect().Cross(decGamma.Vect()).Unit();
    HSMomentum xV=yV.Cross(zV).Unit();
    
    HSLorentzVector decD1=boost(*fBar_d1,decBoost);
    fCosx=TMath::Cos(Angle(decD1.Vect(),xV));
    fCosy=TMath::Cos(Angle(decD1.Vect(),yV));
    fCosz=TMath::Cos(Angle(decD1.Vect(),zV));
    
  }
}
