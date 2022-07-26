//////////////////////////////////////////////////////////////
///
///Class:		StartTimeAction
///Description:
///

#pragma once

#include "ActionManager.h"
#include "VectorUtility.h"
#include "BaseParticle.h"
#include "CLAS12FinalState.h"
#include "CLAS12Base.h"

namespace chanser{

  using vecparticles = std::vector< CLAS12Particle* >;
  
  class C12StartTime 
  { 

  protected:
    CLAS12FinalState* _c12fs{nullptr};
    
  public: 
    
    C12StartTime( )  = default;
     
    virtual void SetC12fs(CLAS12FinalState* c12){_c12fs=c12;}
    virtual void Reset(){};
    virtual Double_t withParticle(CLAS12Particle* p) const { 
      return _c12fs->GetEventInfo()->_StartTime;
    }
    virtual const TString GetName() const {return "C12StartTime";}
    
    ClassDef(chanser::C12StartTime,1);
  };
#pragma link C++ class chanser::C12StartTime;
  ////////////////////////////////////////////////////////////
  class C12StartTimeFromEBFTB : public C12StartTime{

  public: 
    
    C12StartTimeFromEBFTB( )  = default;
  
    Double_t withParticle(CLAS12Particle* p) const final{
      return _c12fs->GetEventInfo()->_FTBStartTime;
    }

    const TString GetName() const final {return "C12StartTimeFromEBFTB";}
    ClassDefOverride(chanser::C12StartTimeFromEBFTB,1);
  };
#pragma link C++ class chanser::C12StartTimeFromEBFTB;
    
  ////////////////////////////////////////////////////////////
  class C12StartTimeFromVt : public C12StartTime{

  public: 
    
    C12StartTimeFromVt( )  = default;
  
    Double_t withParticle(CLAS12Particle* p) const final{
      return p->CLAS12()->par()->getVt();
    }

    const TString GetName() const final {return "C12StartTimeFromVt";}
    ClassDefOverride(chanser::C12StartTimeFromVt,1);
  };
#pragma link C++ class chanser::C12StartTimeFromVt;
    
  ////////////////////////////////////////////////////////////
  class C12StartTimeFromVtFTB : public C12StartTime{

  public: 
    
    C12StartTimeFromVtFTB( )  = default;
  
    Double_t withParticle(CLAS12Particle* p) const final{
      return p->CLAS12()->ftbpar()->getVt();
    }

    const TString GetName() const final {return "C12StartTimeFromVtFTB";}
    ClassDefOverride(chanser::C12StartTimeFromVtFTB,1);
  };
#pragma link C++ class chanser::C12StartTimeFromVtFTB;
    
     
  ////////////////////////////////////////////////////////////
  class StartTimeAction : public ActionManager {

  public:
    StartTimeAction()=default;
    ///Must give an output directory for saving trees to
  StartTimeAction(TString name,C12StartTime* st):ActionManager(name),_stu{st}{};
    
    virtual ~StartTimeAction()=default;
    StartTimeAction(const StartTimeAction& other) = delete; //Copy Constructor
    StartTimeAction(StartTimeAction&& other) = default; //Move Constructor
      
    StartTimeAction& operator=(const StartTimeAction& other)=delete;
    StartTimeAction& operator=(StartTimeAction&& other)=default;

    void PrintAction() override;
    
    void ChangeRun() override{
      auto c12base=dynamic_cast<CLAS12Base*>(_stu.get());
     
      if(c12base!=nullptr)
	c12base->ChangeRun();
    }
 
    Bool_t Execute(UInt_t ti)  override{
      //subtract start time from each particle
      auto c12st=_stu.get();
      c12st->Reset();
      ranges::for_each(_particles[ti], [c12st](CLAS12Particle* p){ p->ShiftTime( - c12st->withParticle(p)); });
      return kTRUE;
    }
      
  
    void Configure(FinalState* fs) override;

  private:
    std::unique_ptr<C12StartTime> _stu;
    
    std::vector<vecparticles>_particles; //element for each topology

    
    ClassDefOverride(chanser::StartTimeAction,1);
  };


}
