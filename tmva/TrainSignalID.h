//////////////////////////////////////////////////////////////
///
///Class:       mva::TrainSignalID		
///Description:
///	\class TrainSignalID
///	Derived from TrainingInterface
///	Used for classifying signal and background events
///	For example
///
///	     TrainSignalID train("MySigBGSelection");
///	     // train.SetOutDir("/a/directory");
///	     train.IgnoreBranches("");//Any branches in tree not used must be flagged!
///	     train.AddSignalTree(signalTree);
///	     train.AddBackgroundTree(background);
///	     train.PrepareTrees();
///	     train.BookMethod(HSMVA::Meths.MLP); //Predefined MLP mode see ModelConfigs.h
///	     train.DoTraining();
///
///	@example RunTrainReg1.C 


#pragma once

#include <TMVA/TMVAGui.h>
#include <TCanvas.h>

#include "FiledTree.h"
#include "TrainingInterface.h"

namespace chanser{
  namespace mva{
  
    class TrainSignalID : public TrainingInterface{

    public :
      
      TrainSignalID()=default;
      TrainSignalID(TString name,TString opt="!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
      virtual ~TrainSignalID()=default;
      
      void AddSignalTree(TTree*  tree,TString wvar="",Double_t weight=1);
      void AddBackgroundTree(TTree*  tree,TString wvar="",Double_t weight=1);

      void PrepareTrees() override;
      void Gui() override{ TMVA::TMVAGui( GetOutDir()+GetOutFileName(),GetName() );};
      
      void DrawROCCurve();
      void DrawResponses();
      
      std::vector<TCanvas*> GetCanvases(){return _canvases;}
    protected:
      
    private :

      std::vector<TCanvas*> _canvases;
      
      ClassDefOverride(chanser::mva::TrainSignalID,1);  // HSMVA Signal Identification training
      
    }; //
    
  } //namespace mva
} //namespace chanser
