//////////////////////////////////////////////////////////////
///
///Class:		ParticleIter
///Description:
///
#pragma once

#include "BaseParticle.h"
#include "Selection.h"
#include <vector>

namespace chanser{

  using std::vector;
  using std::cout;
  using std::endl;

  using particleUPtr = std::unique_ptr<BaseParticle>;
    
  enum class IterType {Single,SingleRem,Multi,MultiRem};

  class ParticleIter{
    
  public :
    ParticleIter()=default;
    ParticleIter(size_t X, const vector<BaseParticle*> parts);
    ParticleIter(size_t X);
    virtual ~ ParticleIter()=default;
 
    ParticleIter(const ParticleIter& other) = default; //Copy Constructor
    ParticleIter(ParticleIter&& other) = default; //Move Constructor
      
    ParticleIter& operator=(const ParticleIter& other)=default;
    ParticleIter& operator=(ParticleIter&& other)=default;
 
    const Selection* GetCombi() const{return &_combi;}
    
    void SetNSel(const Int_t n){_nSel=n;}
    void SetNIdentical(const Int_t n){_nIdentical=n;}
    Int_t GetNSel() const {return _nSel;}
    Int_t GetNIdentical()const {return _nIdentical;}
    
    const vector<BaseParticle*>* GetAllParticles() const{return _allParticles;}
    void SetParticles(vector<BaseParticle*> *parts) {_allParticles=parts;}
    void SetSelParticles(const vector<BaseParticle*> parts){_selected=parts;}
    
    
    void SetSelIter(ParticleIter* com){_selIter=com;_selIter->SetPDG(_PDG);}
    void SetRemIter(ParticleIter* com){_remIter=com;_remIter->SetPDG(_PDG);_remIter->SetName(Form("Select from %s remainder",GetName().Data()));}
    
    //  void AddEventParticle(BaseParticle* part){_evParts.push_back(part);};
    //void AddEventParticles(BaseParticle* part0=nullptr,BaseParticle* part1=nullptr,BaseParticle* part2=nullptr,BaseParticle* part3=nullptr,BaseParticle* part4=nullptr,BaseParticle* part5=nullptr,BaseParticle* part6=nullptr,BaseParticle* part7=nullptr,BaseParticle* part8=nullptr,BaseParticle* part9=nullptr);
    void SetEventParticles(const vector<BaseParticle*> parts){_evParts=parts;};
    
    void SortEvent();
 
    void GotoStart(){ _selected.clear();_remainder.clear();_nSteps=0;_isFinished=kFALSE;_combi.Reset();if(_innerIter) _innerIter->GotoStart();}
    
  
    //Base next combitorial functions which
    //iterate through inner iterators
    Bool_t NextCombitorial0(){
      while(NextCombitorial()){
	SortEvent();
	if(SkipThis()==0)
	  return kTRUE;
      }
      return kFALSE;
    }
    Bool_t FirstCombitorial0(){
      if(FirstCombitorial()){
	SortEvent();
	if(SkipThis()==0)
	  return kTRUE;
      }
      SortEvent();
      return kFALSE;
    }

    void SetNextInnerIter(ParticleIter *_iter);
    void ConfigureIters();
 
  
    Int_t PDG()const {return _PDG;}
    void SetPDG(const Int_t pdg){_PDG=pdg;}; 
    void Print(Int_t verbose)const;
    void SetName(const TString name){fName=name;}
    TString GetName()const {return fName;}
      
    Bool_t IsFinished()const {return _isFinished;}
   
    Int_t SkipThis(){
      if(_innerIter) _skipThis+=_innerIter->SkipThis();
      return _skipThis;
    }
  protected :

    Bool_t DoCombitorial();
    Bool_t NextCombitorial();
    Bool_t FirstCombitorial();
    Bool_t UpdateCombitorial();

  private:

    TString fName;
    Int_t _PDG=0; //PDG code for particles in this iterator
    Selection _combi; //handler for the combination/permuations

    //std::shared_ptr<ParticleIter> _innerIter;  //!recursive iterator
    ParticleIter* _innerIter{nullptr};  //!recursive iterator
    ParticleIter *_selIter{nullptr};  //!selected iterator
    ParticleIter *_remIter{nullptr};  //!remaining iterator


    vector<BaseParticle*> *_allParticles=nullptr; //!vector of particles for this event
    vector<BaseParticle*>  _selected; //!combitorial for selected particles
    vector<BaseParticle*>  _remainder; //!combitorial for unselected particles
    
    vector<BaseParticle*>  _evParts; //!particles to be used in event

    IterType _type=IterType::Single;
      
    UInt_t _parti=0;// index of next particle
    Bool_t _useCombi=kFALSE;
    // Bool_t   _doneRemSel=kTRUE;
    Bool_t _isConfigured=kFALSE;
    Bool_t _isFinished = kFALSE;
    Int_t _skipThis=0;
 
    Int_t _nSel=0;
    Int_t _nIdentical=1;

    Int_t _nSteps=0;
    BaseParticle _extraParticle{-1};
  
  }; //class ParticleIter
    
}//namespace HS

