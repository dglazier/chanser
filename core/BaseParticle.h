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

      
    HSLorentzVector _p4;  //4-vector 
    HSPosition _vertex;     //particle vertex position
    Short_t _pdgCode{0};           //PDG number

    ClassDef(chanser::TruthParticle,1); //class TruthParticle
      
  };
    
  class BaseParticle {
 
  protected:
    HSLorentzVector _p4;  //4-vector 
    HSPosition _vertex;     //particle vertex position
 
    Double32_t _pdgMass{0};
    Double32_t _measMass{0}; //Or other PID info
    Double32_t _time{0};
    Double32_t _path{0};

    Short_t _pdgCode{0};           //PDG number
    Short_t _charge{0};
    Short_t _detector{0};//! some detector ID

    Bool_t _useTruth{0};//
      
    const TruthParticle* _truth{nullptr};
  
  public:
    BaseParticle()=default;  	        //Constructor
    BaseParticle(int code);  	        //Constructor
    BaseParticle(TString pdgname);  	        //Constructor
    virtual ~BaseParticle()=default;	     	//Destructor
    BaseParticle(const BaseParticle& other) = default; //Copy Constructor
    BaseParticle(BaseParticle&& other) = default; //Move Constructor
      
    BaseParticle& operator=(const BaseParticle& other)=default;
    BaseParticle& operator=(BaseParticle&& other)=default;

    //use the truth P4
    void UseTruth() {_useTruth=kTRUE;}
    void NotTruth() {_useTruth=kFALSE;}
    
    //Setting functions
    void SetPDGcode(Int_t code);
    void SetP4(const HSLorentzVector &v){_p4=v;}
    void FixP4(const HSLorentzVector &v){_p4=v;TakePDGMass();}
    void SetVectPDG(const HSLorentzVector &v){_p4.SetXYZT(v.X(),v.Y(),v.Z(),sqrt(v.P2()+_pdgMass*_pdgMass));}
    void SetVectPDG(){_p4.SetXYZT(_p4.X(),+_p4.Y(),_p4.Z(),sqrt(_p4.P2()+_pdgMass*_pdgMass));}
    void SetP4(const HSLorentzVector *v){_p4=*v;}
    void SetXYZT(Double_t X,Double_t Y,Double_t Z,Double_t T){_p4.SetXYZT(X,Y,Z,T);}
    void SetXYZM(Double_t X,Double_t Y,Double_t Z,Double_t M){_p4.SetXYZT(X,Y,Z,sqrt(M*M+X*X+Y*Y+Z*Z));}
    void SetP(Double_t nP){Double_t rp=nP/_p4.P();_p4.SetXYZT(_p4.X()*rp,_p4.Y()*rp,_p4.Z()*rp,sqrt(_p4.M()*_p4.M()+nP*nP));}
    void SetVertex(const HSPosition &v){_vertex=v;}
    void SetVertex(Double_t X,Double_t Y,Double_t Z){_vertex.SetXYZ(X,Y,Z);}
     

    void TakePDGMass(){_measMass=_p4.M();SetVectPDG(_p4);}; //Preserves momentum
    void TakePDGMassFromE(){Double_t rho0=_p4.P();Double_t rho=sqrt(_p4.E()*_p4.E()-_pdgMass*_pdgMass);rho/=rho0;_p4.SetXYZT(_p4.X()*rho,_p4.Y()*rho,_p4.Z()*rho,_p4.E());}; //preserves energy
   
    void SetTruth(const TruthParticle* part){ _truth=part;}
    const TruthParticle* Truth(const TruthParticle* part)const noexcept{ return _truth;}
 
      
    void SetDetector(Short_t det){_detector=det;}
    void SetTime(Double_t ti){_time=ti;};
    void SetMeasMass(Double_t ti){_measMass=ti;};
      
    //Getting functions
    //Imporant this return a copy of p4, if you want to change it use P4p
    HSLorentzVector P4() const {
      return _useTruth ? _truth->_p4 : _p4;
    } 
    
    //    HSLorentzVector P4() const {return _p4;} //return a copy
    HSLorentzVector* P4p()  {return &_p4;}  //return pointer (i.e. you can directly change it)
    const HSPosition& Vertex() const {return _vertex;}
    Double_t PDGMass()const {return _pdgMass;}
    Short_t PDG()const {return _pdgCode;}
    Short_t Charge()const {return _charge;}
    Short_t FindCharge()const;
      
    void SetPath(Double_t path){_path=path;}
    Double_t Path()const {return _path;}

    //Functions required by FinalState
    void ShiftTime(Float_t shift){_time+=shift;}
    Double_t Time()const { return _time;}
    Double_t MeasMass()const { return _measMass;}
    Short_t Detector()const {return _detector;}
    Short_t Status(){return 0;}
    void Clear();
    virtual void MinorClear();
    virtual void CopyParticle(const BaseParticle* part,Bool_t andPDG);
    virtual void CopyTransient(const BaseParticle* part);
    virtual void Print(Option_t *option="") const;

     
    TLorentzVector GetTLorentzVector() const {return (TLorentzVector(_p4.X(),_p4.Y(),_p4.Z(),_p4.T()));}
    TVector3 GetVertex3()const {return (TVector3(_vertex.X(),_vertex.Y(),_vertex.Z()));}

 
    Double_t Beta() const {return _path/_time/2.99792e+08*1E9;}//time ns, path m
    Double_t HypBeta() const {Double_t pp=_p4.P();return pp/sqrt(pp*pp+_pdgMass*_pdgMass);}
    Double_t HypTime() const {return _path/HypBeta()/2.99792e+08*1E9  ;} //in ns
    Double_t DeltaTime() const {return _time-HypTime();};
    Double_t DeltaTimeVer()const {return DeltaTime()+_vertex.Z()/2.99792e+08*1E9;}

    //Add 4-vectors, doca vertices, fix pdg(optional)
    void Add(const BaseParticle *hsp1, const BaseParticle *hsp2,Int_t pdg=0);

 
    //need to permutate class
    //order in vector based on particle momentum
    //nb comparitive operator cannot work on pointers so vectors need
    //to be filled with object not pointers for this to work
    friend bool operator< ( const BaseParticle& lhs, const BaseParticle& rhs ){return lhs._p4.P() < rhs._p4.P(); };

    Double_t p3Distance(HSMomentum vec){return (_p4.Vect()-vec).Mag2();}

    Double_t ResTheta() const {return _truth!=nullptr?_p4.Theta()-_truth->_p4.Theta():0;};
    Double_t ResPhi() const {return _truth!=nullptr?_p4.Phi()-_truth->_p4.Phi():0;};
    Double_t ResP() const {return _truth!=nullptr?_p4.P()-_truth->_p4.P():0;};
    Double_t ResE() const {return _truth!=nullptr?_p4.E()-_truth->_p4.E():0;};
    Double_t ResAngle() const {return _truth!=nullptr?ROOT::Math::VectorUtil::Angle(_p4,_truth->_p4):0;};

    ClassDef(chanser::BaseParticle,1); //class BaseParticle
  };

  inline void  chanser::BaseParticle::SetPDGcode(Int_t code){
    _pdgCode=code;
    if(!_pdgCode) return;
    if(_pdgCode==UndefinedPDG||_pdgCode==-UndefinedPDG) return;
    
    auto partpdg=TDatabasePDG::Instance()->GetParticle(_pdgCode);
    if(partpdg){
      _pdgMass = partpdg->Mass();}
    else _pdgMass=0;
 
  }
  
  inline Short_t chanser::BaseParticle::FindCharge()const{
    
    if(_pdgCode==chanser::UndefinedPDG) return 1;
    else if(_pdgCode==chanser::UndefinedPDG) return -1;

    TParticlePDG *part=TDatabasePDG::Instance()->GetParticle(_pdgCode);
    if(part){
      Int_t charge=part->Charge();
      if(charge!=0)charge=(Int_t) charge/TMath::Abs(charge); //just get sign not mag.
      return charge;
    }
    else return 0;
  }
  inline void chanser::BaseParticle::Clear(){
    MinorClear();
    _pdgCode=0;
    _charge=0;
  }
  inline void chanser::BaseParticle::MinorClear(){
    _p4.SetXYZT(0,0,0,0);
    _measMass=0;
    _time=0;
    _path=0;
    _detector=0;
    _truth=nullptr;
    NotTruth();
  }
  inline void chanser::BaseParticle::CopyParticle(const BaseParticle* part,Bool_t andPDG){
     SetP4(part->P4());
     SetVertex(part->Vertex());
     NotTruth();
    }
  inline void chanser::BaseParticle::CopyTransient(const BaseParticle* part){
    SetP4(part->P4());
    SetVertex(part->Vertex());
  
    _time=part->Time();
    _path=part->Path();
    NotTruth();
  }
  inline void chanser::BaseParticle::Add(const BaseParticle* hsp1, const BaseParticle* hsp2,Int_t pdg){
    SetP4(hsp1->P4()+hsp2->P4());
    SetPDGcode(pdg);
    if(pdg){
      TakePDGMass();
    }
   
  }

}
