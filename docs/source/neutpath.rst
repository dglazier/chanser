Neutron path length correction FDn_pathCorrection
*************************************************

The interaction point for neutrons in the calorimeter layers isn’t necessarily well defined in CLAS12 reconstruction algorithms. This has been studied with pass1 reconstruction, demonstrating the need for path length corrections for neutrons. These can easily be added to your analysis configuration as a correction class with:

::
   
     //n path correction - need start time correction first

     ParticleCorrectionManager pcorrm{"nPathCorrection"};
     pcorrm.AddParticle("neutron",new FDn_pathCorrection(8.13,26.51,29.48,true));
     FS->RegisterPreTopoAction(pcorrm); //PRETOPO

The three first arguments are the corrections applied to the PCAL, ECIN, ECOUT path lengths respectively. The fourth argument should be true if you are using this class with the SecondaryNeutronsMask (or neutrons as CLAS12Neutrons and not CLAS12Particles). You can also use the default path length corrections by removing the first three arguments eg:

::

      pcorrm.AddParticle("neutron",new FDn_pathCorrection(true));

The corrections are then taken from the analysis database (anadb/FDN_PATHCORRECTION.db). These corrections are different for different run periods and with simulated data, the reason being that the calculation of the correction depends on the timing calibration which can vary between run periods, and doesn’t exist in simulation.
 
Note: The correction depends on the start time of an event. You must therefore add a start time action before the path correction.
The class will then recalculate the path and particle four momentum, with the assumption made that this particle is a neutron. When using CLAS12Neutrons this will be applied to all candidates.

