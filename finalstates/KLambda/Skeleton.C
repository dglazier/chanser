{
  //skeleton code maker for exclusive K+Lambda
       
  FSSkeleton s;
  s.SetFinalState("KLambda");
  s.SetFinalStateParts("Electron:e-,Proton:proton,Pim:pi-,Kp:K+");
  s.SetFinalStateTopo("Electron:Proton:Pim:Kp");
  s.SetFinalStateParents("Lambda:Lambda0;Proton;Pim");
  s.MakeCode();
}
