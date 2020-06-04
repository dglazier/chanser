//Based on $ROOTSYS/tutorials/tmva/TMVAClassificationApplication.C
//You can replace the default classifier name with any other you trained pervious (here defMLP)
////////////////////////////////////////////////////////////////////////////////
//This demonstrates automated usage where you just give the path to the Training.root file
//and can then set the tree brances automatically assuming they are all floats
//Run with: chanser_mva RunSignalIDResult.C

#include "ResultInterface.h"
#include "GetTutorialFile.h"

void RunSignalIDResult(){

  
  auto input = GetClassificationFile();
 
  auto *signalTree     = dynamic_cast<TTree*>(input->Get("TreeS"));


  //Example 1
  //If I know the tree only contains floats
  //Full path to training directory
  ResultByFloatTree classif("/work/dump/tmva/TMVAClassificationTut","MLP",signalTree);
   
   Long64_t Nent=signalTree->GetEntries();
   for(Int_t i=0;i<Nent;i++){
     signalTree->GetEntry(i);
     classif.Eval();//get the MVA uput value for this event
   }
   /////////////////////////////////////////////////////////
   //Example 2
   //if the tree contains other than floats I will convert
   //and add the response to the internal tree.
   //To make a copy of the tree and save t file last arg =>kTRUE
   ResultByTree classif2("/work/dump/tmva/TMVAClassificationTut","MLP",signalTree,kFALSE);
   classif2.AddToTree(); //this works for ResultsByFLoatTree too

   //now can look in MLP.root for tree + response
   signalTree->Draw("var1:MLP");
 
}
