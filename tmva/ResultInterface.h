//////////////////////////////////////////////////////////////
///
///Class:       mva::ResultInterface		
///Description:
///
///	Absract Base class for getting values of the TMVA result
///	You will need to use derived classes for regression or classification tasks
///	These can be constructed in manual mode where you specify the variables 
///	and weightfile directly in the constructor
///	OR you run automated by just providing the directory of the Training.root file
///	It will only run 1 method at a time which must also be specified in the constructor
///	The result value can be retrieved for an event using the Eval() function
///	If you want the value for a non-TTree event you must give the variables
///	via a vector<Float_t> : e.g. vector<Float_t > vars={v1,v2,v3...}; result.Eval(vars);


#pragma once

#include "FiledTree.h"

#include <TNamed.h>
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodBase.h"
#include <vector>
#include <iostream>

namespace chanser{
  namespace mva{

    using floats=std::vector<Float_t>;
    using std::vector;
    using std::cout;
    using std::endl;
    using std::unique_ptr;
    
    class ResultInterface : public TNamed{
      
      public :
      
      ResultInterface()=default;
      ResultInterface(TString trainpath,TString methodname);
      
      virtual ~ResultInterface()=default;
      
      void AddToTree();
      
      virtual Float_t Eval();
      virtual vector<Float_t> EvalRegN();
      
      Float_t EvalClass();
      Float_t EvalReg1();
      Float_t Eval(const floats vars){_vars=vars;return Eval();}
      void SetVars(const floats vars){_vars=vars;};
      
      Bool_t NextEntry(){
	if(_entry<fNEntries){ Tree()->GetEntry(_entry++);return kTRUE;}
	_entry=0;
	return kFALSE;
      };
      
      ttree_ptr Tree()const noexcept{return _treeptr;}

      TMVA::Reader* Reader()const noexcept{return _reader.get();}
      TMVA::MethodBase* Method()const noexcept{return _method;}
      TMVA::DataSetInfo* DataSetInfo()const noexcept{return _dataSetInfo.get();}
  
    protected:
  
      void InitTreePtr(filed_uptr tp){_tree=std::move(tp);_treeptr=_tree->Tree();} 
      void SetTreePtr(TTree* tree){_treeptr=tree;}
      floats& Vars()noexcept{return _vars;}
      
      void InitMethod(TString trainpath,TString methodname);
      
      void AddRegressionToTree();
      void AddClassificationToTree();
      
      Long64_t fNEntries=0;
      
    private :
      
      unique_ptr<TMVA::DataSetInfo> _dataSetInfo{nullptr};
      unique_ptr<TMVA::Reader> _reader=unique_ptr<TMVA::Reader>{new TMVA::Reader( "!Color:!Silent" )}; 
      unique_ptr<FiledTree> _tree{nullptr};
      TTree* _treeptr{nullptr};//!
      
      TMVA::MethodBase *_method{nullptr};//will be owned by _reader
 
      floats _vars;
      Long64_t _entry{0};
      
      ClassDef(chanser::mva::ResultInterface,1);  // Virtual base class for all HSMVA results
      
    }; //


    class ResultByFloatTree :  public ResultInterface{
    public:
      ResultByFloatTree()=default;
      ResultByFloatTree(TString trainpath,TString methodname,TTree *tree);
      virtual ~ResultByFloatTree()=default;
      
    protected:
      void SetBranchAddresses(TTree* tree);
      
    };
    /////////////////////////////////////////////////
    ///Allows branches of differnt types
    class ResultByTree :  public ResultInterface{
    public:
      ResultByTree()=default;
      ResultByTree(TString trainpath,TString methodname,TTree *tree,Bool_t copyTree = kFALSE);
      virtual ~ResultByTree()=default;
      
      Float_t Eval() override;
      vector<Float_t> EvalRegN() override;
      void ShowVars();
      
    protected:
      void ReadVars();
      void SetBranchAddresses(TTree* tree);
      
    private:
      vector<Double_t> _treeVarsD;
      vector<Float_t> _treeVarsF;
      vector<Long64_t> _treeVarsL;
      vector<Int_t> _treeVarsI;
      
      vector<UInt_t> _isVarD;
      vector<UInt_t> _isVarF;
      vector<UInt_t> _isVarL;
      vector<UInt_t> _isVarI;
      
    };//ResultByTree
    
    //////////////////////////////////////////////
    using mapNameFloat = std::map<TString, Float_t* >;
 
    class ResultByRefLink :  public ResultInterface{
    public:
      ResultByRefLink()=default;
      ResultByRefLink(TString trainpath,TString methodname,mapNameFloat links);
      virtual ~ResultByRefLink()=default;
      
      //void AddToTree() override {}; 
      
    }; //ResultByRefLink

  }//namespace mva

} //namespace chanser

/////////////////////////////////////////////////
///For ResultInterface
inline Float_t chanser::mva::ResultInterface::Eval(){
  if(Method()->DoRegression()) return EvalReg1();
  else return EvalClass();
}
inline Float_t chanser::mva::ResultInterface::EvalReg1(){
  Float_t result=Method()->GetRegressionValues()[0];
  return result;
}
inline std::vector<Float_t> chanser::mva::ResultInterface::EvalRegN(){
  return Method()->GetRegressionValues();
}
inline Float_t chanser::mva::ResultInterface::EvalClass(){
  Float_t result=Method()->GetMvaValue();
  return result;
}
////////////////////////////////////////////////////////
///For ResultByTree
inline Float_t chanser::mva::ResultByTree::Eval(){
  ReadVars();
  return ResultInterface::Eval();
}
inline std::vector<Float_t> chanser::mva::ResultByTree::EvalRegN(){
  ReadVars();
  return Method()->GetRegressionValues();
}
