#include <TROOT.h>
#include <TSystem.h>
#include <TLeaf.h>

#include "TrainingInterface.h"

namespace chanser{
  namespace mva{	

    ////////////////////////////////////////////////////////////
    ///Specific implemntation requires:
    TrainingInterface::TrainingInterface(TString name,TString opt):
      TNamed(name,name),
      _dataLoader{new  TMVA::DataLoader{name}}
    {
      gSystem->Exec(Form("mkdir -p %s",GetName()));
  
      _outFileName=name+"/"+_outFileName;
      
      _outputFile.reset(TFile::Open( _outFileName, "RECREATE" ));

      _factory.reset(new TMVA::Factory(name,_outputFile.get(),opt));   
    }
    ///////////////////////////////////////////////////////////////////////
    ///Load a tree to DataLoader using all of its branches
    ///Unless branches turned off by IgnoreBranches or OnlyTheseBranches
    void TrainingInterface::LoadTreeVars(TTree*  tree,Double_t weight){
      auto *loader=DataLoader();

      //Use first tree as tempate for variables
      //others assumed the same
      if(!_areVarsLoaded){
	TIter nextbr(tree->GetIteratorOnAllLeaves());
	std::map<TString, char> typeMap = {{"Long64_t",'I'},{"Int_t",'I'},{"Float_t",'F'},{"Double_t",'F'}};
 
	while(auto br=dynamic_cast<TLeaf*>(nextbr())){

	  if(std::count(_ignoreBranches.begin(),_ignoreBranches.end(),TString(br->GetName()))){cout<< "mva::TrainingInterface::LoadTreeVars ingoring branch "<<br->GetName()<<endl;continue;}
      
	  if(tree->GetBranchStatus(br->GetName())){

	    char type = typeMap[br->GetTypeName()];

	    if(br->GetNdata()==1){
	      loader->AddVariable(br->GetName(),type);
	    }
	    else{	  
	      for(Int_t i=0; i<br->GetNdata()-1; i++ ){
		TString varName;
		varName.Form("%s_%i := %s[%i]",br->GetName(),i,br->GetName(),i);
		loader->AddVariable(varName,type);
	      }
	    }
	  }
	}
      }
      _areVarsLoaded++;
    }
    ///////////////////////////////////////////////////////////////////////
    ///Load a tree to DataLoader using all of its branches
    ///Unless branches turned off by IgnoreBranches or OnlyTheseBranches
    // void TrainingInterface::LoadTreeVars(TTree*  tree,Double_t weight){

    //   auto *loader=DataLoader();

    //   //Use first tree as tempate for variables
    //   //others assumed the same
    //   if(!_areVarsLoaded){
    //     TIter nextbr(tree->GetListOfBranches());
    //     while(auto br=nextbr()){

    //       if(std::count(_ignoreBranches.begin(),_ignoreBranches.end(),TString(br->GetName()))){cout<< "HSMVA::TrainingInterface::LoadTreeVars ingoring branch "<<br->GetName()<<endl;continue;}
      
    //       if(tree->GetBranchStatus(br->GetName())){
    // 	if(TString(br->GetTitle()).Contains("/F"))
    // 	  loader->AddVariable(br->GetName(),'F');    
    // 	else if(TString(br->GetTitle()).Contains("/D"))
    // 	  loader->AddVariable(br->GetName(),'F');
    // 	else if(TString(br->GetTitle()).Contains("/I"))
    // 	  loader->AddVariable(br->GetName(),'I');
    // 	else Warning("HSMVA::TrainingInterface::AddTree","variable %s type not supported",br->GetName());	 
    // 	//    tree->SetBranchAddress(br->GetName(),&fVars[i]);
    //       }
    //     }
    //   }
    //   _areVarsLoaded++;
    // }
    void TrainingInterface::DoTraining(){
      auto factory=Factory();

      //MAke sure in ouput file
      _outputFile->cd();
      DataLoader()->GetDataSetInfo().Write("DataSetInfo");

      // Train MVAs using the set of training events
      factory->TrainAllMethods();
  
      // Evaluate all MVAs using the set of test events
      factory->TestAllMethods();
  
      // Evaluate and compare performance of all configured MVAs
      factory->EvaluateAllMethods();

      _outputFile.reset();
      
      if(_outDirName!=TString("")){
	gSystem->Exec(Form("mkdir -p %s",_outDirName.Data()));
	TString trstring="Training.root";
	const char* exists= gSystem->FindFile(GetOutDir()+GetName(),trstring);
	if(exists){
	  cout<<"Warning mva::TrainingInterface::DoTraining() Directory already exists so I will remove it !"<<endl<<"                                               Sorry any previous data will be lost"<<endl;   
	  gSystem->Exec(Form("rm  -r %s",(GetOutDir()+GetName()).Data()));
	}
	gSystem->Exec(Form("mv %s %s",GetName(),GetOutDir().Data()));
	// gSystem->Exec(Form("ln -s  %s %s",(GetOutDir()+GetName()).Data(),GetName()));
      }

      if (!gROOT->IsBatch()){
	gSystem->Load("libTMVAGui");
	Gui();  
      }

    }
    ///////////////////////////////////////////////////////////////
    ///Do not load branches given in this list
    void TrainingInterface::IgnoreBranches(TString branches){
  
      //Split the string with the variables list (v1:v2:v3:...) 
      auto bvars=branches.Tokenize(":");
  
      //Add variables from constructor list to the reader
      //And link to this fVars
      for(Int_t ib=0;ib<bvars->GetEntries();ib++)
	_ignoreBranches.push_back(bvars->At(ib)->GetName());
    }
    ////////////////////////////////////////////////////////////////////
    ///Set Branch Status to 0 if not listed in v0 ("v1:v2:v3:...")
    void TrainingInterface::OnlyTheseBranches(TTree* tree,TString v0){
      //Get arrays of the variables to be used for each distribution
      auto bvars=v0.Tokenize(":");
      //Turn off all branches in tree
      tree->SetBranchStatus("*",0);
      //Turn on branches for distribution1
      for(Int_t i=0;i<bvars->GetEntries();i++)
	tree->SetBranchStatus(bvars->At(i)->GetName(),1);

    }
    //////////////////////////////////////////////////////////////
  }
}
