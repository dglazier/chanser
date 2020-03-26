{
   //skeleton code maker
   
   FSSkeleton s;

   //Give your class a name

   s.SetFinalState("Pi4");

   //Define the possible detected particles in the final state

   s.SetFinalStateParts("Electron:e-,Proton:proton,Pip1:pi+,Pip2:pi+,Pim1:pi-,Pim2:pi-");

   //Define possible topologies for this final state
   //Note ',' seperates different topologies
   // ':' seperates different particle within a topology
   // Here we define 3 topologies, exclusive, 1 missin pi-, 2  missing pi-

   s.SetFinalStateTopo("Electron:Proton:Pip1:Pip2:Pim1:Pim2,Electron:Proton:Pip1:Pip2:Pim1,Electron:Proton:Pip1:Pim1");

   //Define short lived parent particles which decay to
   //detected particles
   //this should not include broad resonances
   //But things like omega, phi, Lambda, K0
   //':' seperates parent name from type
   //';' seperates child particles
   //',' seperates different parents

   s.SetFinalStateParents("K1:K0;Pip1;Pim1,K2:K0;Pip2;Pim2");

   //produce the code	

   s.MakeCode();
   }
