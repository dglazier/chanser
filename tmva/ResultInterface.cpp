#include "ResultInterface.h"
#include <TSystem.h>
#include  <stdexcept>

namespace chanser{
  namespace mva{

    ResultInterface::ResultInterface(TString trainpath,TString methodname) {
      //Assumes used TrainingInterface
      TFile* trainFile=TFile::Open(trainpath+"/Training.root");
      if(!trainFile) throw std::runtime_error{"ResultInterface::ResultInterface could not open %s"};
  
      _dataSetInfo.reset(dynamic_cast<TMVA::DataSetInfo* >(trainFile->Get("DataSetInfo")));
  

      delete trainFile;
    }
    void ResultInterface::InitMethod(TString trainpath,TString methodname){
      //Now the training jobname=name of the top level directory
      //This allows us to make the name of the weights file
      //make sure no / at end so BaseName works
      if(trainpath.EndsWith("/")) trainpath.Remove(TString::kTrailing,'/');
      TString jobname=gSystem->BaseName(trainpath);
      SetName(jobname+"Result");
      TString weightfile=trainpath+"/weights/"+jobname+"_"+methodname+".weights.xml";
      _method=dynamic_cast<TMVA::MethodBase *>(Reader()->BookMVA( methodname, weightfile ));
    }
    void ResultInterface::AddToTree(){
      cout<<" ResultInterface::AddToTree() "<<Method()<<" "<<Tree()<<endl;
      if(!Tree()){
	cout<<"ResultByTree::AddToTree() I do not own a tree! Need to add kTRUE as extra argument in constructor"<<endl;
	exit(1);
      }
      if(Method()->DoRegression())
	AddRegressionToTree();
      else
	AddClassificationToTree();

      Tree()->ResetBranchAddresses();
    }
    void ResultInterface::AddRegressionToTree(){
      auto vecTargets=DataSetInfo()->GetTargetInfos();
      vector<Float_t> varTargets(vecTargets.size());
      vector<TBranch*> branches;
      UInt_t iv=0;
      for(auto const& target: vecTargets){
	TString name=target.GetInternalName();
	cout<<name<<" "<<varTargets[iv]<<endl;
	branches.push_back(Tree()->Branch(name+"_reg",&varTargets[iv++],name+"_reg/F"));
      }
      while(NextEntry()){
	auto vecRes=EvalRegN();
	varTargets=vecRes;
	for(auto const& branch: branches)
	  branch->Fill();
      }
    
    }
    void ResultInterface::AddClassificationToTree(){
      cout<<" ResultInterface::AddClassificationToTree()"<<endl;
      Float_t response=0;
      auto branch=Tree()->Branch(Method()->GetName(),&response,TString(Method()->GetName())+"/F");
  
      //classification
      while(NextEntry()){
	response=Eval();

	branch->Fill();
      }
  
    }
    /////////////////////////////////////////ResultByFloatTree
    ResultByFloatTree::ResultByFloatTree(TString trainpath,TString methodname,TTree *tree):
      ResultInterface(trainpath,methodname){

      //Create _vars for linking tree to Reader
      const UInt_t NVars=DataSetInfo()->GetNVariables();
      auto& vars=Vars();
      vars.resize(NVars);
      //Note internal name in principle could have been a formula in TMVA
      //in HSMVA only actual branch variables are supported
      for(UInt_t i=0;i<NVars;i++)
	Reader()->AddVariable(DataSetInfo()->GetVariableInfo(i).GetInternalName(),&(vars.at(i)));

      SetBranchAddresses(tree);

      InitMethod(trainpath,methodname);
    };
  
    //////////////////////////////////////////////////////////////////
    ///In the case we know all branches are floats we can just
    //directly link the branch addresses to _vars
    void ResultByFloatTree::SetBranchAddresses(TTree* tree){
      auto& vars=Vars();
      const UInt_t NVars=DataSetInfo()->GetNVariables();
   
      for(UInt_t i=0;i<NVars;i++){
	cout<<DataSetInfo()->GetVariableInfo(i).GetInternalName()<<endl;
	auto res=tree->SetBranchAddress(DataSetInfo()->GetVariableInfo(i).GetInternalName(),&(vars.at(i)));
	if(res!=0){
	  cout<<"Error ResultInterface::SetBranchAllFloats, Branch "<<Method()->GetInputVar(i) <<" not set, perhaps it is not a float "<<tree->GetBranch(Method()->GetInputVar(i))->GetTitle()<<endl;
	  exit(1);
	}
      }
    }
    /////////////////////////////////////////////////ResultByTree
    //Path to training dir, methodname for this result,
    //tree you want to manipulate, copyTree(default false) make a copy of tree
    //copy needed for AddToTree()
    ResultByTree::ResultByTree(TString trainpath,TString methodname,TTree *tree,Bool_t copyTree):
      ResultInterface(trainpath,methodname){

      //Create _vars for linking tree to Reader
      const UInt_t NVars=DataSetInfo()->GetNVariables();
      auto& vars=Vars();
      vars.resize(NVars);
      //Note internal name in principle could have been a formula in TMVA
      //in HSMVA only actual branch variables are supported
      for(UInt_t i=0;i<NVars;i++)
	Reader()->AddVariable(DataSetInfo()->GetVariableInfo(i).GetInternalName(),&(vars.at(i)));
      if(copyTree){
	//Make a copy of the tree
	InitTreePtr(FiledTree::RecreateCopyFull(tree,methodname+".root"));
	fNEntries=Tree()->GetEntries();
	SetBranchAddresses(Tree());
      }
      else{ //just link to tree
	SetTreePtr(tree);
	fNEntries=Tree()->GetEntries();
	SetBranchAddresses(tree);
      }
  
      InitMethod(trainpath,methodname);
    };

    //////////////////////////////////////////////////////////////////
    ///Connect the variables in the method to a tree containing branches
    ///with the same names as the MVA was trained with
    //To allow for non float branches we connect to other types
    //first then transfer to _vars in each event
    void ResultByTree::SetBranchAddresses(TTree* tree){
      auto& vars=Vars();
      const UInt_t NVars=DataSetInfo()->GetNVariables();
 
      _treeVarsD.resize(NVars);
      _treeVarsF.resize(NVars);
      _treeVarsL.resize(NVars);
      _treeVarsI.resize(NVars);
 
      for(UInt_t i=0;i<NVars;i++){
	//First get the branch type
	TString bname=DataSetInfo()->GetVariableInfo(i).GetInternalName();
	auto branch=tree->GetBranch(bname);
	Int_t res=-1;
	if(TString(branch->GetTitle()).Contains("/F")){
	  UInt_t vsize=_isVarF.size();
	  _treeVarsF[vsize]=0;
	  res=tree->SetBranchAddress(bname,&_treeVarsF[vsize]);
	  _isVarF.push_back(i);
	}
	if(TString(branch->GetTitle()).Contains("/D")){
	  UInt_t vsize=_isVarD.size();
	  _treeVarsD[vsize]=0;
	  res=tree->SetBranchAddress(bname,&_treeVarsD[_treeVarsD.size()-1]);
	  _isVarD.push_back(i);
	}
	if(TString(branch->GetTitle()).Contains("/I")){
	  UInt_t vsize=_isVarI.size();
	  _treeVarsI[vsize]=0;
	  res=tree->SetBranchAddress(bname,&_treeVarsI[_treeVarsI.size()-1]);
	  _isVarI.push_back(i);
	}
     
	if(TString(branch->GetTitle()).Contains("/L")){
	  UInt_t vsize=_isVarL.size();
	  _treeVarsL[vsize]=0;
	  res=tree->SetBranchAddress(bname,&_treeVarsL[_treeVarsL.size()-1]);
	  _isVarL.push_back(i);
	}
      
	if(res!=0)
	  cout<<"ResultInterface::SetBranchAddresses, Branch "<< Method()->GetInputVar(i) <<"not set "<<endl;
      
      }
  
      _treeVarsD.resize(_isVarD.size());
      _treeVarsF.resize(_isVarF.size());
      _treeVarsL.resize(_isVarL.size());
      _treeVarsI.resize(_isVarI.size());

      _treeVarsD.shrink_to_fit();
      _treeVarsF.shrink_to_fit();
      _treeVarsL.shrink_to_fit();
      _treeVarsI.shrink_to_fit();
  
      cout<<" ResultInterface::SetBranchAddresses found branches : "<<endl;
      cout<< "   double "<<_isVarD.size()<<endl;
      cout<< "   float "<<_isVarF.size()<<endl;
      cout<< "   long "<<_isVarL.size()<<endl;
      cout<< "   int "<<_isVarI.size()<<endl;
      cout<<"   total vars in reader are "<<vars.size()<<endl;
    }

    void ResultByTree::ReadVars(){
      auto& vars=Vars();

      // for(auto const& val : *vars)
	//   cout<<val<<" ";
	// cout<<endl;
      for(UInt_t i=0;i<_isVarD.size();i++)
	(vars)[_isVarD[i]]=_treeVarsD[i];
      
      for(UInt_t i=0;i<_isVarF.size();i++)
	(vars)[_isVarF[i]]=_treeVarsF[i];
   
      for(UInt_t i=0;i<_isVarL.size();i++)
	(vars)[_isVarL[i]]=_treeVarsL[i];
   
      for(UInt_t i=0;i<_isVarI.size();i++)
	(vars)[_isVarI[i]]=_treeVarsI[i];
    }
    ///////////////////////////////////////////////////ResultByRefLink
    ////////////////////////////////////////////////////////////////
    ///Class to link reader variables to float references via
    /// map<TString, Float_t* > (aka mapNameFloat!)
    ///e.g. map<TString, Float_t& > mylinks;
    ///     Float_t v1=0;
    ///     mylinks["v1"]=&v1;
    ///     ...
    ///     ResultByRefLink result("/path/to/Training.root","defBDT",mylinks);
    ResultByRefLink::ResultByRefLink(TString trainpath,TString methodname,mapNameFloat links):
      ResultInterface(trainpath,methodname){

      const UInt_t NVars=DataSetInfo()->GetNVariables();
      //_vars.resize(NVars);//Not used!
      for(UInt_t i=0;i<NVars;i++){
	//***********************************************************
	//Here is different use external links rather than _vars
	TString name=DataSetInfo()->GetVariableInfo(i).GetInternalName();
	if(!links[name]){
	  cout<<"Error  ResultByRefLink::ResultByRefLink variable "<<name<<endl;
	  throw std::runtime_error{"ResultByRefLink variable does not exist in link map"};
	}
	else
	  Reader()->AddVariable(name,links[name]);
      }
      InitMethod(trainpath,methodname);

    }
    ///////////////////////////////////////////////////////////////
  }
}
