Example of creating clas12 KLambda analysis

Create Skeleton code maker Skeleton.C (see main README for details)

       {
	   //skeleton code maker for exclusive K+Lambda
       
           FSSkeleton s;
 	   s.SetFinalState("KLambda");
  	   s.SetFinalStateParts("Electron:e-,Proton:proton,Pim:pi-,Kp:K+");
  	   s.SetFinalStateTopo("Electron:Proton:Pim:Kp");
  	   s.SetFinalStateParents("Lambda:Lambda0;Proton;Pim");
  	   s.MakeCode();
	}

Create code

        chanser_skeleton Skeleton.C
	ls
	Create_KLambda.C  KLambda.cpp  KLambda.h  Run_KLambda.C  Skeleton.C  TreeDataKLambda.h



