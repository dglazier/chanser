//////////////////////////////////////////////////////////////
///
///Class:		Selection
///Description:
///
#pragma once

#include "BaseParticle.h"
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <TMath.h>

namespace chanzer{

  using std::vector;
  using std::cout;
  using std::endl;
    
  namespace SELECT{
      
    template <typename Iterator> bool next_combination(const Iterator first, Iterator k, const Iterator last)
      {
	/* Credits: Mark Nelson http://marknelson.us */
	if ((first == last) || (first == k) || (last == k))
	  return false;
	Iterator i1 = first;
	Iterator i2 = last;
	++i1;
	if (last == i1)
	  return false;
	i1 = last;
	--i1;
	i1 = k;
	--i2;
	while (first != i1)
	  {
	    if (*--i1 < *i2)
	      {
		Iterator j = k;
		while (!(*i1 < *j)) ++j;
		std::iter_swap(i1,j);
		++i1;
		++j;
		i2 = k;
		std::rotate(i1,j,last);
		while (last != j)
		  {
		    ++j;
		    ++i2;
		  }
		std::rotate(k,i2,last);
		return true;
	      }
	  }
	std::rotate(first,k,last);
	return false;
      }
  }//namespace SELECT
    
  ///////////////////////////////////////////////////////////////////////
  class Selection{
    
  public :
    Selection()=default;
    virtual ~Selection()=default;
    Selection(const Selection& other) = default; //Copy Constructor
    Selection(Selection&& other) = default; //Move Constructor
      
    Selection& operator=(const Selection& other)=default;
    Selection& operator=(Selection&& other)=default;

     
    template<class T> bool NextMulti(std::vector<T*> *vec,const size_t p=0,const size_t identical=1);
    template<class T> bool NextRotate(std::vector<T*> *vec,const size_t identical=1);
    //template<class T> T* NextSingle(std::vector<T*> *vec);
    // template<typename T> bool NextSingle(T* sitem, std::vector<T*> *vec);
    bool NextSingle(size_t &index, std::vector<BaseParticle*> *vec){
      //cout<<"selection "<<_Niter<<" "<<vec->size()<<endl;
      // cout<<"selection "<<index <<" ok "<<_Niter<<" "<<vec->size()<<endl;
      if(_Niter==vec->size()){
	_Niter=0;
	index=0;
	return false;
	//	return vec->at(0);
      }
      //     auto result=vec->at(_Niter);
      //++_Niter;
      index=_Niter++;
      //cout<<"selection "<<index <<" ok "<<_Niter<<" "<<vec->size()<<endl;
      return true;
      //    return vec->at(_Niter++);
    }

    void Reset(){_Niter=0;}
      
  private:
  
    unsigned long _Niter=0;
    unsigned long  _Ncombi=0;
     
  };
  //////////////////////////////////////////////////////////////////////////
    
  //template<class T> T* Selection::NextSingle( std::vector<T*> *vec){
  /* template<typename T> bool Selection::NextSingle(T* sitem, std::vector<T*> *vec){ */
  /*       //cout<<"selection "<<_Niter<<" "<<vec->size()<<endl; */
  /* cout<<"selection "<<sitem <<" ok "<<_Niter<<" "<<vec->size()<<endl; */
  /*      if(_Niter==vec->size()){ */
  /* 	_Niter=0; */
  /* 	sitem=vec->at(0); */
  /* 	return false; */
  /* 	//	return vec->at(0); */
  /*       } */
  /*      //     auto result=vec->at(_Niter); */
  /*      //++_Niter; */
  /*      sitem=vec->at(_Niter++); */
  /* cout<<"selection "<<sitem <<" ok "<<_Niter<<" "<<vec->size()<<endl; */
  /*  return true; */
  /*      //    return vec->at(_Niter++); */
  /*     } */

  /////////////////////////////////////////////////////////////////////////////////////////
  //  bool Selection::NextSingle(BaseParticle* sitem, std::vector<BaseParticle*> *vec)
  ///////////////////////////////////////////////////////////////////////
  template<class T> bool Selection::NextMulti(std::vector<T*> *vec,const size_t p,const size_t identical){
      
    size_t n=vec->size();
    uint start =0;
 
    if(_Niter==0){//New iteration
      //calculate number of combinations
      _Ncombi=TMath::Factorial(n)/TMath::Factorial(p)/TMath::Factorial(n-p)/(identical);//should perhaps cache this calculation

      _Niter++;
      return true;
    }
      

    SELECT::next_combination(vec->begin(), vec->begin() + p, vec->end()); 

 
    if(_Niter==_Ncombi){
      _Niter=0;
      _Ncombi=0;
      return false;
    }
    ++_Niter;

    return true;
  }
  ////////////////////////////////////////////////////////////////////////
  template<class T> bool Selection::NextRotate(std::vector<T*> *vec,const size_t identical){
    size_t n=vec->size();
    uint start =0;
 
    if(_Niter==0){//New iteration
      //calculate number of combinations
      //_Ncombi=TMath::Factorial(n)/TMath::Factorial(n-1)/(identical);//should perhaps cache this calculation
      _Ncombi=n/(identical);
      ++_Niter;
      return true;
	
    }
    //rotate all elements by 1 place
    std::rotate(vec->begin(),vec->begin()+1,vec->end());
 
    if(_Niter==_Ncombi){

      _Niter=0;
      _Ncombi=0;
      return false;

    }

    ++_Niter;


    return true;
  }

    
}//namespace HS
