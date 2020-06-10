//This demonstrates automated usage where you just give the path to the Training.root file
//and can then set the tree brances automatically assuming they are all floats
//Run with: chanser_mva RunTrainSignalID.C

#include "ResultInterface.h"
#include "MethConf.h"

void RunTrainSignalID(){

  //Get a tree of particle data that includes Final variables
  auto full = FiledTree::Read("particle","/work/dump/all/dglazier/Pi2_Pi2_NONE_ALL__/particleData/ParticleVariables_0.root");

  //Turn off all final variables apart from MissMass2 which we will cut on to define signal and background
  full->Tree()->SetBranchStatus("Pi2*",0);
  full->Tree()->SetBranchStatus("Pi2MissMass2",1);
  
  auto signal = FiledTree::RecreateCopyFull(full->Tree(),
					   "/work/dump/all/dglazier/Pi2_Pi2_NONE_ALL__/particleData/Signal.root",
					   "TMath::Abs(Pi2MissMass2)<0.1");
  auto background = FiledTree::RecreateCopyFull(full->Tree(),
					       "/work/dump/all/dglazier/Pi2_Pi2_NONE_ALL__/particleData/Background.root",
					       "TMath::Abs(Pi2MissMass2)>0.2");


  auto train = TrainSignalID("mva2"); //The name string will be appended to the ouput directory, this (dir + name) must be specified when using MVASignalIDAction

  //Give an output directory destination. Note the name in the cell above will also be added for this instance of the training.
  train.SetOutDir("");
  //Set any branches in the tree that you which to ignore in the training
  train.IgnoreBranches("Pi2MissMass2:DTCuts2:DTCuts1:DTCuts05:EBCuts:Topo:NPerm:ElectronRegion");
  //Provide the trees we got from the ROOT file above, note as they are FiledTrees I must use the .Tree() function.
  train.AddSignalTree(signal->Tree());
  train.AddBackgroundTree(background->Tree());
  //Set how many events you would like to use from each for testing and training
  train.SetNTrainTest(10000,10000);
  train.PrepareTrees();

  //Standard TMVA Factory Method Booking
  train.BookMethod(TMVA::Types::kBDT, "BDT","!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20");
  //Shortcut for predefined chanser_mva classifiers see MethConf.h
  train.BookMethod(Meths.MLP);

  train.DoTraining();

  train.DrawROCCurve();

  train.DrawResponses();
  
}
