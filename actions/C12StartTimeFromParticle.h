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
    
    void SetC12(CLAS12FinalState* c12) final{
      _c12fs=c12;
      _fromTime=static_cast<CLAS12Particle*>(_c12fs->GetParticle(_timeName));
      _bunchTime=_c12fs->BunchTime();
      _sTimePeak=_c12fs->STimePeak();
    }
    void Reset() final{_timeZero=0;}
    Double_t withParticle(CLAS12Particle* p) const final{
      if(_timeZero==0) _timeZero=TimeZero(_fromTime->DeltaTime());
      return _timeZero-p->Vertex().Z()/2.99792e+08*1E9;
    }

    const TString GetName() const final {return TString("C12StartTimeFromParticle = ")+_timeName;}

  private:
    
    TString _timeName;
    CLAS12Particle* _fromTime{nullptr};//!
    Float_t _bunchTime{0};
    Float_t _sTimePeak{0};
    mutable Double_t _timeZero{0};
    
    Double_t TimeZero(Float_t ptime) const;
    
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


}
