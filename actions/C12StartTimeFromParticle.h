//////////////////////////////////////////////////////////////
///
///Class:		C12StartTimeFromParticle
///Description:
///              Calculate the event start time from the given
///              Particle DeltaTime

#pragma once
#include "StartTimeAction.h"

namespace chanser{
  
  ////////////////////////////////////////////////////////////
  class C12StartTimeFromParticle : public C12StartTime{

  public: 
    C12StartTimeFromParticle()  = default;

  C12StartTimeFromParticle(TString timeParticle):_timeName(timeParticle){};
    
    void SetC12(CLAS12FinalState* c12) override{
      _c12fs=c12;
      _fromTime=static_cast<CLAS12Particle*>(_c12fs->GetParticle(_timeName));
      _bunchTime=_c12fs->BunchTime();
      _sTimePeak=_c12fs->STimePeak();
    }
    void Reset() final{_timeZero=0;}
    Double_t withParticle(CLAS12Particle* p) const override{
      if(_timeZero==0) _timeZero=TimeZero(_fromTime->DeltaTime());
      return _timeZero+p->Vertex().Z()/2.99792e+08*1E9;
    }

    const TString GetName() const override {return TString("C12StartTimeFromParticle = ")+_timeName;}

    const TString& TimeParticeName()const {return _timeName;}

  protected:
    CLAS12Particle* _fromTime{nullptr};//!
    mutable Double_t _timeZero{0};
    Double_t TimeZero(Float_t ptime) const;
 
  private:
    
    TString _timeName;
    Float_t _bunchTime{0};
    Float_t _sTimePeak{0};
    
     
    ClassDefOverride(chanser::C12StartTimeFromParticle,1);
  };
#pragma link C++ class chanser::C12StartTimeFromParticle;
  
  inline Double_t C12StartTimeFromParticle::TimeZero(Float_t ptime) const{
     
    //supply chosen (e-) particle vertex time
    Float_t rftime=_c12fs->GetEventInfo()->_RFTime;
    //Find the nearest rf beam bucket
    auto stime=_sTimePeak-_bunchTime*((Int_t)(std::round(((_sTimePeak-(ptime-rftime))/_bunchTime))))+rftime;

    
    return stime;
  }

  ////////////////////////////////////////////////////////////
  class C12StartTimeFromHighMomentum : public C12StartTimeFromParticle{

  public: 
    C12StartTimeFromHighMomentum()  = default;

  C12StartTimeFromHighMomentum(TString timeParticle):C12StartTimeFromParticle(timeParticle){};
    
    void SetC12(CLAS12FinalState* c12) override;
    
    Double_t withParticle(CLAS12Particle* p) const final{
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
      if(_timeZero==0) _timeZero=TimeZero(fastptr->DeltaTime());
      return _timeZero - p->Vertex().Z()/2.99792e+08*1E9;
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
      auto particle=static_cast<CLAS12Particle*>(_c12fs->GetParticle(particleName->GetName()));
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
