//Based on $ROOTSYS/tutorials/tmva/TMVAClassificationApplication.C
//You can replace the default classifier name with any other you trained pervious (here defMLP)
////////////////////////////////////////////////////////////////////////////////
//This demonstrates automated usage where you just give the path to the Training.root file
//and can then set the tree brances automatically assuming they are all floats
//Run with: chanser_mva RunSignalIDResult.C

#include "ResultInterface.h"

void RunSignalIDResult(){

  
  auto input =TFile::Open("/work/dump/all/dglazier/Pi2_Pi2_NONE_ALL__/particleData/ParticleVariables_0.root","update");
 
  auto *signalTree     = dynamic_cast<TTree*>(input->Get("particle"));

   //if the tree contains other than floats I will convert
   //and add the response to the internal tree.
   //To make a copy of the tree and save t file last arg =>kTRUE
  ResultByTree classif2("TMVAClassification1","BDT",signalTree,kTRUE);
  classif2.AddToTree(); //this works for ResultsByFLoatTree too

   //now can look in MLP.root for tree + response
  signalTree->Draw("Pi2MissMass2:BDT>>(100,-0.5,0.5,100,-5,5)","","col1");
 
}
