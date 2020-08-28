/**
	\class ParticleIter
	
	Class to loop over THSParticle vectors
	Permuation, Rotation or Combination (Selection) done
	via THSCombinitorial class
	This class contains a pointer to another ParticleIter
	to allow for recursive combinations.
	This class is used to control the permutating of particles
	in THSFinalState
*/

#include "ParticleIter.h"

namespace chanser{
  
  ParticleIter::ParticleIter(size_t X,const vector<BaseParticle*> parts):
    _evParts(parts)
  {
    SetNSel(X);
    //SetNIdentical(X);
 }
  ParticleIter::ParticleIter(size_t X)
  {
    SetNSel(X);
    //SetNIdentical(X);
  }
  //////////////////////////////////////////////////////////////////////////
  ///Get the next combination of the iterator
  ///Returns kFALSE when all possible combinations have been given
  ///Returns kTRUE when still more combinations to go
  ///The new ordering can be accessed via NextParticle()
  ///Or if event particles have been set they can be updated
  ///by a call to SortEvent();

    
  Bool_t ParticleIter::UpdateCombitorial(){
    _selected.clear();
 
    DoCombitorial();
    
    if(_innerIter) //recursively go through all linked iterators
      _innerIter->UpdateCombitorial();

    return kTRUE;
  }
  Bool_t ParticleIter::NextCombitorial(){
       
    if(_innerIter){ //recursively go through all linked iterators
       if(_innerIter->NextCombitorial())
	return kTRUE;
       }
    //do my own iterator
    if( DoCombitorial()){
   
      if(_innerIter)_innerIter->UpdateCombitorial();
	
      return kTRUE;
    }
	
    return kFALSE;
  }
    
  Bool_t ParticleIter::FirstCombitorial(){
 
    //reset everything
    GotoStart();
      
    //look for case of only 1 possible combination
    //while getting the first selection
    Bool_t notDone=DoCombitorial(); //true if  more combis to come
    if(_innerIter){ //recursively go through all linked iterators
      //any iter not done makes this true
      notDone+=_innerIter->FirstCombitorial();
	
    }
    // cout<<" ParticleIter::FirstCombitorial() "<<GetName()<<" "<<endl;
    return notDone;
  }

    
  Bool_t ParticleIter::DoCombitorial(){
  
    // cout<<"ParticleIter::DoCombitorial() nsel "<<_nSel<<" "<<_allParticles<<" "<<this<<endl;
    //cout<<"ParticleIter::DoCombitorial() allparticles size "<<_allParticles->size()<<endl;
    if(_allParticles->size()<_nSel){
      return kFALSE;
    }
    Bool_t moreToCome=kTRUE;
    BaseParticle *psingle{nullptr};

    //return if have been given particles and already have selected
    if(_nSel==_allParticles->size()&&!_selected.empty())
      return kFALSE;
     switch (_type) {
      //case select 1 and no remainder
    case IterType::Single :
      {
	size_t index=0;
	if(_combi.NextSingle(index,_allParticles)){

	  _selected.clear();
	  _selected.push_back(_allParticles->at(index));
	}
	else {
	  _selected.clear();
	  _selected.push_back(_allParticles->at(index));

	  moreToCome= kFALSE; //no more combinations
	}
      }
      break;
	
      //case select 1 and remainder
    case IterType::SingleRem :
      {
	if(_combi.NextRotate(_allParticles,_nIdentical)){
	  _selected.clear();
	  _remainder.clear();
	  _selected.push_back((*_allParticles)[0]);
	  _remainder.insert(_remainder.begin(),_allParticles->begin()+1, _allParticles->end());
	}
	else moreToCome= kFALSE; //no more combinations
      }  
      break;
	
      //case select p and no remainder
    case IterType::Multi :
      {	
	if(_combi.NextMulti(_allParticles,_nSel,_nIdentical)){
	  _selected.clear();
	  _remainder.clear();
	  _selected.insert(_selected.begin(),_allParticles->begin(), _allParticles->begin()+_nSel);
	  if(_remIter)_remainder.insert(_remainder.begin(),_allParticles->begin()+_nSel, _allParticles->end());
	}
	else moreToCome= kFALSE; //no more combinations
      }
    }//end swith(_type)

    if(_selIter){
      _selIter->SetParticles(&_selected);
    }
    if(_remIter){
      _remIter->SetParticles(&_remainder);
    }
       
 
    if(!moreToCome) {_isFinished = kTRUE;return kFALSE;}
  
    return kTRUE;
  }

  //////////////////////////////////////////////////////////////////////////
  ///Set the order that the selected and remaining iterators will be called
  ///First Add selected then Remaining recursively
  void ParticleIter::ConfigureIters(){
    if(_isConfigured) return;
    _isConfigured=kTRUE;
      
    //cout<<"Configure "<<_selIter<<" "<<_remIter<<" "<<_innerIter<<endl;
    // if(_allParticles)cout<<"          "<<_nSel<<" "<<_evParts.size()<<" "<<_allParticles->size()<<endl;
    if(_selIter){
      SetNextInnerIter(_selIter);
      if(_remIter){
	SetNextInnerIter(_remIter);
      }
    }
    else if(_remIter){
      //cout<<" ParticleIter::ConfigureIters() no _selIter......investigate"<<endl;
      SetNextInnerIter(_remIter);
    }

    //define type to optimise NextCombitorial
    if(_nSel==1&&!_remIter)_type=IterType::Single;
    else if(_nSel==1&&_remIter)_type=IterType::SingleRem;
    else _type=IterType::Multi;

      
    //configure inner 
    if(_innerIter) _innerIter->ConfigureIters();
  }
  //////////////////////////////////////////////////////////////////////////
  ///Add _iter to the end of the current iterator sequence
  void ParticleIter::SetNextInnerIter(ParticleIter *_iter){
    if(_innerIter) //If inner already exists move on inside it to look for next space
      _innerIter->SetNextInnerIter(_iter);
    else
      _innerIter=_iter; //Take shared  ownership of the pointer
  }

  //////////////////////////////////////////////////////////////////////////
  ///Automates the recursive selection of Y identical particle types  X times
  ///e.g. select 3pi0 from 6 photons
  ///== select 2 from 6; select 2 from remaining 4; use remaining 2
  ///example usage : piterator.SelectXofY(3,2);
  ///An additional level of combinations sel_iter can be given and will be
  ///performed on all Y selections
  /// i.e.can add a permutation to the X selections
  ///example usage : piterator.SelectXofY(3,2, new THSPermutation());

 
  //////////////////////////////////////////////////////////////////////////
  ///If event particles have been given assign copy a HSParticle
  ///to them in the current combination order
  ///The particles have to have been assigned to the correct iterator
  void ParticleIter::SortEvent(){

    _skipThis=0;
    if(_innerIter){
      _innerIter->SortEvent();
    }
    if(_selIter) return ; //these particles will be set in _selIter

    //loop over final state particles controlled by this iter
    //Copy the event data for this combo of this particle
    for(UInt_t isel=0;isel<_evParts.size();isel++){
      if(isel<_selected.size()){
	_evParts[isel]->CopyTransient(_selected[isel]);
	_evParts[isel]->SetVectPDG();
      }
      else{
	_evParts[isel]->P4p()->SetXYZT(0,0,0,0);
	_skipThis=1; //iterator not properly reset, try another event and it should be OK
      }
    }
    
    
  }
  

  void ParticleIter::Print(Int_t verbose)const {
    if (verbose==1&&_evParts.size()==0) {if(_innerIter) _innerIter->Print(verbose); return;}

    cout<<" ParticleIter::Print() "<<endl;
    if(verbose>1) cout<<GetName()<<endl;
    cout<<"     Type : "<<" number chosen "<<_nSel*_nIdentical<<" of id  "<<_PDG<<" and number used here = "<<_evParts.size()<<endl;
    if(verbose==0)cout<<"     NSel "<<_nSel<<" Nidentical "<<_nIdentical<<" SelIter "<<(_selIter!=nullptr)<<" RemIter "<<(_remIter!=nullptr)<<endl; 
    if(_innerIter) _innerIter->Print(verbose);
    else cout<<endl;
      
  }
    
    
}
