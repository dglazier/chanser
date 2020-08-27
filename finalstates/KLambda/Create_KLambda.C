{
  auto FS = dglazier::KLambda::Make("ALL","ALL");
  FS->AddTopology("Electron:Proton:Pim:Kp");
  
  ////Save TreeDataKLambda
  FS->UseOutputRootTree();
  
  ////Write to file for later processing
  FS->WriteToFile("KLambda_ALL_ALL.root");
 //Delete the final state rather than let ROOT try
  FS.reset();
}
