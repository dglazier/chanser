//////////////////////////////////////////////////////////////
///
///Class:       mva::TrainingInterface		
///Description:
///	Abstract Base class for interfacing to TMVA training.
///	You will need to use a derived class for regression or classification
///	Links to TMVA::DataLoader for connecting with input tree
///	And to TMVA::Factory for defining models (BDT, MLP, ...)
///	By default all branches in given tree are used in training
///	To ignore some branches use IgnoreBranches("b1:b5:...")
///	Constructor require and name which will be used as a directory name
///	and can be used to connect to the result. If the same name is 
///	used again results will be overwritten. You may also want to define
///	an additional output directory SetOutDir("dirname") where the name dir
///	will be copied to after training is completed

#pragma once

#include <TNamed.h>
#include <TCut.h>
#include <TString.h>
#include <TCanvas.h>
#include "TMVA/Tools.h"
#include "TMVA/Types.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Factory.h"
#include "TMVA/MethodBase.h"
#include "TMVA/mvas.h"
#include <vector>
#include <iostream>

#include "MethConf.h"

namespace chanser{
  namespace mva{
    
    using std::vector;
    using std::cout;
    using std::endl;
    using std::unique_ptr;

    class TrainingInterface : public TNamed{

    public :
    
      TrainingInterface()=default;
      TrainingInterface(TString name,TString opt);
      virtual ~TrainingInterface()=default;
  
      void LoadTreeVars(TTree* tree,Double_t weight=1);
      void SetNTrainTest(Long64_t ntr,Long64_t nte){_nTrain=ntr;_nTest=nte;}
      void SetCut(TCut cut){_cut=cut;}
      void IgnoreBranches(TString name);
      void OnlyTheseBranches(TTree* tree,TString v0);
    
      virtual void PrepareTrees()=0;
      virtual void Gui() = 0;
    
      void DoTraining();
    
      TMVA::Factory *Factory()const noexcept{return _factory.get();}
      TMVA::DataLoader *DataLoader()const noexcept{return _dataLoader.get();}

      void BookMethod(TMVA::Types::EMVA type,TString name,TString params){
	Factory()->BookMethod(DataLoader(),type,name,params);}
      void BookMethod(MethConf meth){
	Factory()->BookMethod(DataLoader(),meth.fType,meth.fName,meth.fParams);};
      
      TString GetOutFileName(){return _outFileName;}
      void SetOutDir(TString dir){_outDirName =dir;}
      TString GetOutDir(){
	if(_outDirName!=TString("")) return _outDirName+"/";
	return "";
      }


 
    protected:
      
      Bool_t AreVarsLoaded(){if(_areVarsLoaded) return kTRUE;return kFALSE;}
      TCut GetCut()const noexcept{return _cut;}
      
      Int_t NTrain()const noexcept{return _nTrain;}
      Int_t NTest()const noexcept{return _nTest;}
      
    private :
      unique_ptr<TMVA::Factory> _factory;
      unique_ptr<TMVA::DataLoader> _dataLoader;
      
      
      unique_ptr<TFile> _outputFile{nullptr};
      
      Long64_t _nTrain=1000;
      Long64_t _nTest=1000;
      
      vector<TString> _ignoreBranches;
      
      Int_t _areVarsLoaded{0}; //Not 0 when LoadTreeVars has been called
      TCut _cut="";    //Currently only 1 cut on tree allowed vis SetCut!
      TString _outFileName="Training.root";
      TString _outDirName="";//move output directory to here
      
      ClassDef(chanser::mva::TrainingInterface,1);  // Virtual base class for all HSMVA training
      
    }; //TrainingInterface
    
  } //namespace mva
} //namespace chanser
