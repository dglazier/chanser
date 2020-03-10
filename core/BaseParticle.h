//////////////////////////////////////////////////////////////
///
///Class:		BaseParticle
///Description:
///
#pragma once

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TDatabasePDG.h"
#include "TMatrixD.h"
#include <vector>
#include <map>
#include <iostream>
#include <Math/Vector4D.h>
#include <Math/Point3D.h>
#include <Math/DisplacementVector3D.h>
#include <Math/VectorUtil.h> //for boosts etc.

//Allow THSParticle to be written to tree
#pragma link C++ class std::vector<chanser::BaseParticle >+;
#pragma link C++ class std::vector<chanser::BaseParticle* >+;


namespace chanser{
  using  HSLorentzVector=ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<Double32_t> > ;
  using  HSPosition= ROOT::Math::PositionVector3D< ROOT::Math::Cartesian3D< Double32_t >, ROOT::Math::DefaultCoordinateSystemTag >;
    
  using  HSMomentum=ROOT::Math::DisplacementVector3D< ROOT::Math::Cartesian3D< Double_t >, ROOT::Math::DefaultCoordinateSystemTag >;
    
  const Short_t UndefinedPDG=1E4;

  class TruthParticle {


  public:
      
    TruthParticle()=default;  	        //Constructor
    TruthParticle(int code);  	        //Constructor
    TruthParticle(TString pdgname);  	        //Constructor
    virtual ~TruthParticle()=default;	     	//Destructor
    TruthParticle(const TruthParticle& other) = default; //Copy Constructor
    TruthParticle(TruthParticle&& other) = default; //Move Constructor
      
    TruthParticle& operator=(const TruthParticle& other)=default;
    TruthParticle& operator=(TruthParticle&& other)=default;

      
    HSLorentzVector _P4;  //4-vector 
    HSPosition _Vertex;     //particle vertex position
    Short_t _PDGCode=0;           //PDG number

    ClassDef(chanser::TruthParticle,1); //class TruthParticle
      
  };
    
  class BaseParticle {
 
  protected:
    HSLorentzVector _P4;  //4-vector 
    HSPosition _Vertex;     //particle vertex position
 
    Double32_t _PDGMass=0;
    Double32_t _MeasMass=0; //Or other PID info
    Double32_t _Time=0;
    Double32_t _Path=0;

    Short_t _PDGCode=0;           //PDG number
    Short_t _Charge=0;
    Short_t _Detector=0;//! some detector ID

      
      
     const TruthParticle* _Truth{nullptr};
  
  public:
    BaseParticle()=default;  	        //Constructor
    BaseParticle(int code);  	        //Constructor
    BaseParticle(TString pdgname);  	        //Constructor
    virtual ~BaseParticle()=default;	     	//Destructor
    BaseParticle(const BaseParticle& other) = default; //Copy Constructor
    BaseParticle(BaseParticle&& other) = default; //Move Constructor
      
    BaseParticle& operator=(const BaseParticle& other)=default;
    BaseParticle& operator=(BaseParticle&& other)=default;
      
    //Setting functions
    void SetPDGcode(Int_t code);
    void SetP4(const HSLorentzVector &v){_P4=v;}
    void FixP4(const HSLorentzVector &v){_P4=v;TakePDGMass();}
    void SetVectPDG(const HSLorentzVector &v){_P4.SetXYZT(v.X(),v.Y(),v.Z(),sqrt(v.P2()+_PDGMass*_PDGMass));}
    void SetVectPDG(){_P4.SetXYZT(_P4.X(),+_P4.Y(),_P4.Z(),sqrt(_P4.P2()+_PDGMass*_PDGMass));}
    void SetP4(const HSLorentzVector *v){_P4=*v;}
    void SetXYZT(Double_t X,Double_t Y,Double_t Z,Double_t T){_P4.SetXYZT(X,Y,Z,T);}
    void SetXYZM(Double_t X,Double_t Y,Double_t Z,Double_t M){_P4.SetXYZT(X,Y,Z,sqrt(M*M+X*X+Y*Y+Z*Z));}
    void SetP(Double_t nP){Double_t rp=nP/_P4.P();_P4.SetXYZT(_P4.X()*rp,_P4.Y()*rp,_P4.Z()*rp,sqrt(_P4.M()*_P4.M()+nP*nP));}
    void SetVertex(const HSPosition &v){_Vertex=v;}
    void SetVertex(Double_t X,Double_t Y,Double_t Z){_Vertex.SetXYZ(X,Y,Z);}
     

    void TakePDGMass(){_MeasMass=_P4.M();SetVectPDG(_P4);}; //Preserves momentum
    void TakePDGMassFromE(){Double_t rho0=_P4.P();Double_t rho=sqrt(_P4.E()*_P4.E()-_PDGMass*_PDGMass);rho/=rho0;_P4.SetXYZT(_P4.X()*rho,_P4.Y()*rho,_P4.Z()*rho,_P4.E());}; //preserves energy
   
    void SetTruth(const TruthParticle* part){ _Truth=part;}
 
      
    void SetDetector(Short_t det){_Detector=det;}
    void SetTime(Double_t ti){_Time=ti;};
    void SetMeasMass(Double_t ti){_MeasMass=ti;};
      
    //Getting functions
    HSLorentzVector P4() const {return _P4;} //return a copy
    HSLorentzVector* P4p()  {return &_P4;}  //return pointer (i.e. you can directly change it)
    const HSPosition& Vertex() const {return _Vertex;}
    Double_t PDGMass()const {return _PDGMass;}
    Short_t PDG()const {return _PDGCode;}
    Short_t Charge()const {return _Charge;}
    Short_t FindCharge()const;
      
    void SetPath(Double_t path){_Path=path;}
    Double_t Path()const {return _Path;}

    //Functions required by FinalState
    void ShiftTime(Float_t shift){_Time+=shift;}
    Double_t Time()const { return _Time;}
    Double_t MeasMass()const { return _MeasMass;}
    Short_t Detector()const {return _Detector;}
    Short_t Status(){return 0;}
    void Clear();
    virtual void MinorClear();
    virtual void CopyParticle(const BaseParticle* part,Bool_t andPDG);
    virtual void CopyTransient(const BaseParticle* part);
    virtual void Print(Option_t *option="") const;

     
    TLorentzVector GetTLorentzVector() const {return (TLorentzVector(_P4.X(),_P4.Y(),_P4.Z(),_P4.T()));}
    TVector3 GetVertex3()const {return (TVector3(_Vertex.X(),_Vertex.Y(),_Vertex.Z()));}

 
    Double_t Beta() const {return _Path/_Time/2.99792e+08*1E9;}//time ns, path m
    Double_t HypBeta() const {Double_t pp=_P4.P();return pp/sqrt(pp*pp+_PDGMass*_PDGMass);}
    Double_t HypTime() const {return _Path/HypBeta()/2.99792e+08*1E9  ;} //in ns
    Double_t DeltaTime() const {return _Time-HypTime();};
    Double_t DeltaTimeVer()const {return DeltaTime()+_Vertex.Z()/2.99792e+08*1E9;}

    //Add 4-vectors, doca vertices, fix pdg(optional)
    void Add(const BaseParticle *hsp1, const BaseParticle *hsp2,Int_t pdg=0);

 
    //need to permutate class
    //order in vector based on particle momentum
    //nb comparitive operator cannot work on pointers so vectors need
    //to be filled with object not pointers for this to work
    friend bool operator< ( const BaseParticle& lhs, const BaseParticle& rhs ){return lhs._P4.P() < rhs._P4.P(); };

    Double_t p3Distance(HSMomentum vec){return (_P4.Vect()-vec).Mag2();}

    Double_t ResTheta() const {return _Truth!=nullptr?_P4.Theta()-_Truth->_P4.Theta():0;};
    Double_t ResPhi() const {return _Truth!=nullptr?_P4.Phi()-_Truth->_P4.Phi():0;};
    Double_t ResP() const {return _Truth!=nullptr?_P4.P()-_Truth->_P4.P():0;};
    Double_t ResE() const {return _Truth!=nullptr?_P4.E()-_Truth->_P4.E():0;};
    Double_t ResAngle() const {return _Truth!=nullptr?ROOT::Math::VectorUtil::Angle(_P4,_Truth->_P4):0;};

    ClassDef(chanser::BaseParticle,1); //class BaseParticle
  };

  inline void  chanser::BaseParticle::SetPDGcode(Int_t code){
    _PDGCode=code;
    if(!_PDGCode) return;
    if(_PDGCode==UndefinedPDG||_PDGCode==-UndefinedPDG) return;
    
    auto partpdg=TDatabasePDG::Instance()->GetParticle(_PDGCode);
    if(partpdg){
      _PDGMass = partpdg->Mass();}
    else _PDGMass=0;
 
  }
  
  inline Short_t chanser::BaseParticle::FindCharge()const{
    
    if(_PDGCode==chanser::UndefinedPDG) return 1;
    else if(_PDGCode==chanser::UndefinedPDG) return -1;

    TParticlePDG *part=TDatabasePDG::Instance()->GetParticle(_PDGCode);
    if(part){
      Int_t charge=part->Charge();
      if(charge!=0)charge=(Int_t) charge/TMath::Abs(charge); //just get sign not mag.
      return charge;
    }
    else return 0;
  }
  inline void chanser::BaseParticle::Clear(){
    MinorClear();
    _PDGCode=0;
    _Charge=0;
  }
  inline void chanser::BaseParticle::MinorClear(){
    _P4.SetXYZT(0,0,0,0);
    _MeasMass=0;
    _Time=0;
    _Path=0;
    _Detector=0;
    _Truth=nullptr;
  }
  inline void chanser::BaseParticle::CopyParticle(const BaseParticle* part,Bool_t andPDG){
     SetP4(part->P4());
    SetVertex(part->Vertex());
    }
  inline void chanser::BaseParticle::CopyTransient(const BaseParticle* part){
    SetP4(part->P4());
    SetVertex(part->Vertex());
  
    _Time=part->Time();
    _Path=part->Path();
  }
  inline void chanser::BaseParticle::Add(const BaseParticle* hsp1, const BaseParticle* hsp2,Int_t pdg){
    SetP4(hsp1->P4()+hsp2->P4());
    SetPDGcode(pdg);
    if(pdg){
      TakePDGMass();
    }
   
  }

}
