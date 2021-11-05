Masking particles
=================

The event builder provides a list of particles that have been reconstructed per
event. However it may be that some of these particle candidates are not good
and analysing them can result in many background events when using a combitorial
algorithm. To address this chanser provides the ability to mask certain
particles from the event, so they are not used in deciding topologies or
assigned to final state particles.

This technique is particularly relevent for neutral particle candidates :
photons and neutrons, due to the likelihood of split off clusters in
calorimeters for example.

To make you own mask class you must inherit from the MaskedEventParticles class.

You may decide to try one of the already existing masks for your analysis.
It would be a good idea to run parallel chanser final state configurations,
one with the mask and one without to see the impact.

Masking Calorimeter split-off clusters
**************************************

Often when one particle (charged or neutral) hits the calorimeter the clustering
algorithm can generate more than 1 distinct cluster, these additional clusters
are refered to as split-offs here.

They can be identified by their close proximity to another cluster or charged track. Here we use the distance between hits to identify split-offs.

The algorithm follows:

1. Look for all FD neutral hits (photons+neutrons)
2. Remove photon candidates with no PCAL hit
3. Iterate through neutrals and for each 
   a. Loop through all other particles
   b. Calculate angle difference with neutral candidate EC XYZ position and PCAL XYZ for other neutrals and trajectory position on PCAL for charged particles.   
   c. If difference is less than cut value, assume split-off cluster and *remove or from event*, if other particle was a neutral add the 2 neutral clusters together if this option, MergeNeutrals, is requested.

      Diagnostic histograms are saved in the ouput directory with filename MaskCalorSplitOffs.root to let you check cut values.

To use this algorithm you must assign it to your final state class when you are configuring it, i.e. in Create script,

::
        Usage : MaskCalorSplitOffs(AngleDiffNeutral,AngleDiff+ve,AngleDiff-ve,MergeNeutrals)
   
        auto useEBPidFor = "ALL"; //or "NONE"
        auto useInclusiveFilterFor = "ALL"; //or "NONE" or "gamma:neutron" (i.e. as may gammas and neutrons but exact other particles)
	auto FS = dglazier::Pi2::Make(useEBPidFor,useInclusiveFilterFor);
 	...
	...
        FS->MaskParticles(new MaskCalorSplitOffs(30,30,30,1) );

Note this only works when the inclusive option (useInclusiveFilterFor) is set to ALL in your final state constructor.

Angle differences are specified in degrees.


Masking Radiated photons
************************

When an electron or positron are created it an interaction in the target they
may radiate a bremsstrahlung photon and loose some energy. These photons
will be directed along the momentum of the charged particle that emmitted it.
As these photons are often created before the electron enters the toroidal field
we can expect to find these photons in the calorimeters at a postion given
by the reconstructed charge particle momentum direction. These photons can
then be removed from the event and their energy given to the charged particle
parent.

The algorithm proceeds as,

1. Collect all electrons and positrons
2. Collect all neutral calorimeter hits with a PCAL hit as photon candidates
3. Take the photon hit postion from its PCAL position
4. Loop over electrons, take their position from PCAL
5. Calculate momentum theta,phi and position difference between electron and photon
6.  If theta difference is below set value, and position difference is above a set value, then consider this a radiated photon
7. In case particle was identified as neutron, its energy will not be calorimeter energy deposit, so calculate total calorimeter energy deposit and correct from sampling fraction read in from ccdb.
8.  If radiated add photon enegy to electron momentum 4-vector.
9.  Assign the electron a new detector number :....
10. Repeat for positrons.

    
To use this algorithm you must assign it to your final state class when you are configuring it, i.e. in Create script,

::
   
   Usage : 
   MaskRadPhotons(Float_t positionDiffCut,Float_t thetaDiffCut, Short_t addToElectron)
      
   auto useEBPidFor = "ALL"; //or "NONE"
   auto useInclusiveFilterFor = "ALL"; //or "NONE" or "gamma:neutron" (i.e. as may gammas and neutrons but exact other particles)
   auto FS = dglazier::Pi2::Make(useEBPidFor,useInclusiveFilterFor);
   ...
   ...
   FS->MaskParticles(new MaskRadPhotons(10,0.5,1));
   
Note this only works when the inclusive option (useInclusiveFilterFor) is set to ALL in your final state constructor.

Angle differences are specified in degrees.

Diagnostic histograms are saved in the ouput directory with filename .root to let you check cut values.
