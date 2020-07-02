//////////////////////////////////////////////////////////////
///
///Class:		MaskedEventParticles
///Description:
///    Class responsible for sorting particles into type vectors
///

#pragma once

#include "EventParticles.h"

#include <TFile.h>

namespace chanser{

 
  class MaskedEventParticles : public EventParticles{
      
  public :
    MaskedEventParticles()=default;
    virtual ~MaskedEventParticles()=default;
    MaskedEventParticles(const MaskedEventParticles& other) = default; //Copy Constructor
    MaskedEventParticles(MaskedEventParticles&& other) = default; //Move Constructor
      
    MaskedEventParticles& operator=(const MaskedEventParticles& other)=default;
    MaskedEventParticles& operator=(MaskedEventParticles&& other)=default;
 
    virtual void AssignVectors(EventParticles* ep);
    virtual void PrintMask() const;
    virtual Bool_t ReReadEvent(){return kTRUE;};
    virtual void SetPidVectors();
    void  AddPids(const Short_t pid,const particles_ptrs* vec);
    void  PidCounter();
    
    const std::vector<short>& Pids()const noexcept{return _pidCounts;}

    void ReadyFile(TString outDir);

  protected:
    
    particle_ptr NextParticle(Short_t pid,UInt_t& entry);

    void Write(TObject& obj );
    
  private:

    std::vector<particles_ptrs*> _pidParticles;//!
    std::vector<Short_t> _pidCounts;//!


    std::unique_ptr<TFile> _outFile;//!
    
    ClassDef(chanser::MaskedEventParticles,1); //class MaskedEventParticles
    
  };



}
