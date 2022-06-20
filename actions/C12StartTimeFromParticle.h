//////////////////////////////////////////////////////////////
///
///Class:		C12StartTimeFromParticle
///Description:
///              Calculate the event start time from the given
///              Particle DeltaTime

#pragma once
#include "StartTimeAction.h"
#include "CLAS12Base.h"
#include <cmath>

namespace chanser{
  
  ////////////////////////////////////////////////////////////
  class C12StartTimeFromParticle : public C12StartTime, public CLAS12Base{

  public: 
    C12StartTimeFromParticle()  = default;

  C12StartTimeFromParticle(TString timeParticle):_timeName(timeParticle){};

    void ChangeRun() final;
    
    void SetC12(CLAS12FinalState* c12) override{
      CLAS12Base::SetC12(c12);
      _fromTime=static_cast<CLAS12Particle*>(c12->GetParticle(_timeName));
    }
    void Reset() final{_timeZero=0;}
    Double_t withParticle(CLAS12Particle* p) const override{
      //returns start time at (0,0,0) relative to EB particle Vertex
      //So to get correct time for each particle must use its
      //vertex position to correct for offset from (0,0,0)
      if(_timeZero==0){
	if(_fromTime->CLAS12()->getRegion()==1000)
	  _timeZero=TimeZero(_fromTime->DeltaTime(),_targetPosition);
	else 	_timeZero=TimeZero(_fromTime->DeltaTime(),_fromTime->Vertex().Z());


      }
      return _timeZero;
    }
    
    const TString GetName() const override {return TString("C12StartTimeFromParticle = ")+_timeName;}

    const TString& TimeParticeName()const {return _timeName;}

  protected:
    CLAS12Particle* _fromTime{nullptr};//!
    mutable Double_t _timeZero{0};
    Double_t TimeZero(Float_t ptime,Float_t vz=0) const;
 
  private:
    
    TString _timeName;
    Float_t _bunchTime{0};
    Float_t _sTimePeak{0};
    
    Double_t _targetPosition={0};
    Double_t _rfBucketLength={0};
    Double_t _rfOffset={0};
    
    const int _RF_LARGE_INTEGER={800};//https://github.com/JeffersonLab/clas12-offline-software/blob/096dc4d3cd39b6c7e540c7b1760f11a2a417bfaa/reconstruction/eb/src/main/java/org/jlab/rec/eb/EBConstants.java#L15

    
    ClassDefOverride(chanser::C12StartTimeFromParticle,1);
  };
#pragma link C++ class chanser::C12StartTimeFromParticle;

  //Note EB equivalent to TimeZero
  /*https://github.com/JeffersonLab/clas12-offline-software/blob/653f835459980826b948db43fde34fea4e4b392e/reconstruction/eb/src/main/java/org/jlab/rec/eb/EBRadioFrequency.java#L37
  public double  getStartTime(DetectorParticle p,final DetectorType type,final int layer,final double vz) {
        final double tgpos = this.ccdb.getDouble(EBCCDBEnum.TARGET_POSITION);
        final double rfBucketLength = this.ccdb.getDouble(EBCCDBEnum.RF_BUCKET_LENGTH);
        final double vertexTime = p.getVertexTime(type,layer,p.getPid());
        final double vzCorr = (tgpos - vz) / PhysicsConstants.speedOfLight();
        final double deltatr = - vertexTime - vzCorr
                + this.rfTime + this.ccdb.getDouble(EBCCDBEnum.RF_OFFSET)
                + (EBConstants.RF_LARGE_INTEGER+0.5)*rfBucketLength;
        final double rfCorr = deltatr % rfBucketLength - rfBucketLength/2;
        return vertexTime + rfCorr;
    }
  */
  inline Double_t C12StartTimeFromParticle::TimeZero(Float_t ptime,Float_t vz) const{
    //supply chosen (e-) particle vertex time
    Float_t rftime=GetFinalState()->GetEventInfo()->_RFTime;
    //double  vzCorr = (vz)/TMath::C()*1E9; //ns/cm
    //double  vzCorr =0;

    /*
     double  vzCorr = (_targetPosition-vz)/TMath::C()*1E9;//DSTS already shifted!
     double deltatr = - ptime - vzCorr + rftime + _rfOffset + (_RF_LARGE_INTEGER+0.5)*_rfBucketLength; //DSTs rftime already shifted by rfOffset
     double rfCorr = std::fmod(deltatr , _rfBucketLength) - _rfBucketLength/2;
     auto stime=ptime + rfCorr;
    */
    
    double deltatr = - ptime + rftime + (_RF_LARGE_INTEGER+0.5)*_rfBucketLength;
    double rfCorr = std::fmod(deltatr , _rfBucketLength) - _rfBucketLength/2;
    auto stime=ptime + rfCorr - _targetPosition/TMath::C()*1E9;
    
     
    return stime;
  }

  ////////////////////////////////////////////////////////////
  class C12StartTimeFromHighMomentum : public C12StartTimeFromParticle{

  public: 
    C12StartTimeFromHighMomentum()  = default;

  C12StartTimeFromHighMomentum(TString timeParticle):C12StartTimeFromParticle(timeParticle){};
    
    void SetC12(CLAS12FinalState* c12) override;
    
    Double_t withParticle(CLAS12Particle* p) const final{

      if(_timeZero==0) {
	//Find the highest momentum particle
	_fastest.SetXYZT(0,0,0,0);
	CLAS12Particle* fastptr={nullptr};
	for(auto* phigh : _possibleParticle){
	    if( phigh->P4().P() > _fastest.P()){
	    _fastest=phigh->P4();
	    fastptr=phigh;
	  }
	}
	//find the start time using the highest momentum particle and p
	_timeZero=TimeZero(fastptr->DeltaTime());
      }
      return _timeZero;
    }
    
    const TString GetName() const override {return TString("C12StartTimeFromHighMomentum = ") + TimeParticeName();}

  private :
    
    std::vector<CLAS12Particle*> _possibleParticle;//!
    mutable HSLorentzVector _fastest;//!

    ClassDefOverride(chanser::C12StartTimeFromHighMomentum,1);
  };
#pragma link C++ class chanser::C12StartTimeFromHighMomentum;
  
  inline void C12StartTimeFromHighMomentum::SetC12(CLAS12FinalState* c12) {

    C12StartTimeFromParticle::SetC12(c12);
    
    auto choices = TimeParticeName().Tokenize(":");
    for(auto* particleName : *choices){
      auto particle=static_cast<CLAS12Particle*>(c12->GetParticle(particleName->GetName()));
      if(particle==nullptr){
	std::cerr<<"WARNING C12StartTimeFromHighMomentum no such particle "<<particleName->GetName()<<std::endl;
      }
      
      _possibleParticle.push_back(particle);
    }
    if(_possibleParticle.empty()){
      std::cerr<<"ERROR C12StartTimeFromHighMomentum no correctly named particles given"<<std::endl;
      exit(0);
    }
    return;
  }
  
}
