//////////////////////////////////////////////////////////////
///
///Class:		
///Description:
///     my own derived particle cut

#pragma once

#include "BaseCut.h"

namespace chanser{


  class HadronChi2PID : public chanser::BaseCut{
      
  public:
    HadronChi2PID()=default; // must have default constructor
    
    HadronChi2PID(Int_t cut){ _cutVal=cut;}
      
      
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data

      Double_t chi2Pid = c12->par()->getChi2Pid();

      Double_t C;
      Int_t pid = c12->getPid();
      if(pid == 211) C = 0.88;
      else if (pid == -211) C = 0.93;
      else C = 100;

      Double_t calc_min_chi2Pid = C*-3;

      Double_t p = c12->getP();
      Double_t calc_max_chi2Pid = 0;

      if(_cutVal == 1)
	{//this is the standard cut
	  if(p < 2.44)
	    {calc_max_chi2Pid = C*3;}
	  else if(p > 2.44 )
	    {calc_max_chi2Pid = C*(0.00869 + 14.98587*TMath::Exp(-p/1.18236)+1.81751*TMath::Exp(-p/4.86394));}
	}

      if(_cutVal == 2)
	{//stricter cut
	  if(p < 2.44)
	    {calc_max_chi2Pid = C*3;}
	  else if(p > 2.44 && p < 4.6)
	    {calc_max_chi2Pid = C*(0.00869 + 14.98587*TMath::Exp(-p/1.18236)+1.81751*TMath::Exp(-p/4.86394));}
	  else if(p > 4.6)
	    {calc_max_chi2Pid = C*(-1.14099 + 24.14992*TMath::Exp(-p/1.36554) + 2.66876 * TMath::Exp(-p/6.80552));}
	}

      return (chi2Pid>calc_min_chi2Pid && chi2Pid<calc_max_chi2Pid) ?  kTRUE :  kFALSE;
    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with cutVal "<<_cutVal<<std::endl;
    }

  private:
    Int_t _cutVal=chanser::THIS_INT_MAX2;
      
    ClassDefOverride(chanser::HadronChi2PID,1);

  };//class HadronChi2PID

}
