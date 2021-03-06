//Based on $ROOTSYS/tutorials/tmva/TMVAClassification.C
//Run with: chanser_mva RunTrainSignalID.C

#include "GetTutorialFile.h"
#include "TrainSignalID.h"

void RunTrainSignalID(){
 
  auto input = GetClassificationFile();

  auto *signalTree     = dynamic_cast<TTree*>(input->Get("TreeS"));
  auto *background     = dynamic_cast<TTree*>(input->Get("TreeB"));

  TrainSignalID train("TMVAClassificationTut");

  train.SetOutDir("/work/dump/tmva/");
  train.IgnoreBranches("");//Any branches in tree not used must be flagged!
  train.AddSignalTree(signalTree);
  train.AddBackgroundTree(background);
  train.SetNTrainTest(1500,1500);
  train.PrepareTrees();

  //Can Book methods either via standard TMVA::Factory interface...
  train.BookMethod(TMVA::Types::kBDT, "BDT","!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20");

  //..or predefined methods (See HSMVA::MethodConfigure.h)
  train.BookMethod(chanser::mva::Meths.MLP);

  train.DoTraining();
  train.DrawResponses();
  train.DrawROCCurve();//Got to come after responses

  }
