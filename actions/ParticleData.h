//////////////////////////////////////////////////////////////
///
///Class:		ParticleData
///Description:
///             Keeps tree and branches for particle data

#pragma once

#include "BaseParticle.h"
#include "ParticleOutEvent.h"
#include "FiledTree.h"
#include <vector>

namespace chanser{

   
  class ParticleData {
      
  public:
    ParticleData()=default;
  ParticleData(const TString& treename,const TString& filename,size_t np):_tree{FiledTree::Recreate(treename,filename)}{};
      
    virtual ~ParticleData()=default;
    ParticleData(const ParticleData& other) = default; //Copy Constructor
    ParticleData(ParticleData&& other) = default; //Move Constructor
      
    ParticleData& operator=(const ParticleData& other)=default;
    ParticleData& operator=(ParticleData&& other)=default;

    void AddParticle( ParticleOutEvent* out,BaseParticle* particle,TString name){
      if(out==nullptr) return;
      auto addOut=out->clone(); //clone return unique_ptr
      addOut->SetName(name);
      addOut->SetParticle(particle);
      addOut->Branches(_tree->Tree());
      _outputs.push_back(std::move(addOut));
      _nParticles++;
    }

    void Fill();
    void Save(){_tree.reset();}

    TTree* GetTree() noexcept{return _tree->Tree();}
      
  private:
      
    std::vector<partoutev_uptr >_outputs;//!

    std::unique_ptr<FiledTree> _tree;//!

    UInt_t _nParticles{0};
      
    ClassDef(chanser::ParticleData,1);
      
  };//class ParticleData
    
  using partdata_ptr = ParticleData*;
  using partdatas =std::vector<partdata_ptr>;
}
