//////////////////////////////////////////////////////////////
///
///Class:	Cut_PionChi2Pid	
///Description: A 3-sigma cut for negative values for the pion chi2Pid value from event builder
/// Details of the cut at positive sigma can be found in the RG-A analysis note

#pragma once

#include "BaseCut.h"

namespace chanser{


  class Cut_PionChi2Pid : public BaseCut{
      
  public:
    Cut_PionChi2Pid()=default; // must have default constructor
    
  Cut_PionChi2Pid(Int_t level,Float_t cval):_level{level},_C{cval}{}
      
    //void ChangeRun() final;//not currently needed
    
    Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
      auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
      auto c12=c12p->CLAS12(); //if you require other DST data

      auto chi2Pid = c12->par()->getChi2Pid();
      auto p = part->P4().P();

      Double_t calc_min_chi2Pid = -3 * _C;
      Double_t calc_max_chi2Pid = 0;

      //See analysis Note Section 8.3.2.2
    
      if(p < 2.44)
	calc_max_chi2Pid = _C * _a0_0;

      else if(p < 4.6 || _level == 1 )
	calc_max_chi2Pid = _C*(_a1_0
			      +  _a1_1 * TMath::Exp(-p/_a1_2)
			      +  _a1_3 * TMath::Exp(-p/_a1_4)
			      );
      else//"strict" cut
	calc_max_chi2Pid = _C*(_a2_0
			      + _a2_1 * TMath::Exp(-p/_a2_2)
			      + _a2_3 * TMath::Exp(-p/_a2_4)
			      );
    


      return (chi2Pid>calc_min_chi2Pid && chi2Pid<calc_max_chi2Pid)
	?  kTRUE :  kFALSE;

    }
      
    
    void Print(Option_t* option = "")const final{
      std::cout<<"\t\t"<<ClassName()<<" with C =  "<<_C<< " and cut level "<<_level<<std::endl;
    }

  private:
    Float_t _C=chanser::THIS_FLT_MAX;
    Int_t _level={1};

    Float_t _a0_0=3;
    Float_t _a1_0=0.00869;
    Float_t _a1_1=14.98587;
    Float_t _a1_2=1.18236;
    Float_t _a1_3=1.81751;
    Float_t _a1_4=4.86394;
    Float_t _a2_0=-1.14099;
    Float_t _a2_1=24.14992;
    Float_t _a2_2=1.36554;
    Float_t _a2_3=2.66876;
    Float_t _a2_4=6.80552;
  
    ClassDefOverride(chanser::Cut_PionChi2Pid,1);

  };//class Cut_PionChi2Pid

}
