Handling Simulated Data
~~~~~~~~~~~~~~~~~~~~~~~

In the HiPO DST file reconstructed particles are stored in the REC::Particle
banks. If the particled genertated for the simulation is not succesfully
reconstructed then it will not have an entry in this bank. In particular
for events where no particles are reconstructed this bank will be empty.
The genereated particle information is however still accessible from the
MC::Lund bank. In clas12root this is accessed as p->mc().

For chanser we will need to use this information for 2 different tasks.

1) Produce the generated distributions, required for normalising
   acceptances etc.
2) Truth match the reconstructed event so the reconstrcuted particle's
   truth 4-momentum is available, to caclulate higher level truth variables
   such as decay angles etc. This information can then be used to check
   for bias' and so in induced by the reconstruction.


Both of these require that your final state code know which LUND particle
corresponds to each final state particle. This is done by adding the order of
the particle in the LUND file to the AddParticle function call. For example
for the e-,p, pi+, pi-, pi0 final state, my AddParticle calls would look like

::
   
   AddParticle("Electron",&_electron,kTRUE,4);
   AddParticle("Proton",&_proton,kTRUE,3);
   AddParticle("Pip",&_pip,kTRUE,0);
   AddParticle("Pim",&_pim,kTRUE,2);
   AddParticle("Gamma1",&_gamma1,kTRUE,-1);
   AddParticle("Gamma2",&_gamma2,kTRUE,-1);
 
   //Set final state parents
   AddParticle("Pi0",&_pi0,kTRUE,1);

                   #in your final state .cpp file

The last index in each function call corresponds to the position in the LUND
file. -1 implies the particle is not included in the LUND event. So in this
case the event generator did not decay the pi0, but this was left to gemc
so the LUND event just has the information for the pi0 which is included as
particle 1.

For truth events only 4 momentum are valid. Other detector related variables
should not be used. To this end only the Kinematics function is called for
truth/generated events, so any calculations or variables requiring detector
information should be put in the Topology function instead. 

Note the ordering in the file can be checked using 'hipo-utils -dump' on the
simulated DST file. For the events shown above this gives

::
   Choose (n=next,p=previous, q=quit, h=help), Type Bank Name or id : MC::Lund position for [MC::Lund] == 5089
   * NODE * group =     40, item =   3, type = 11, size =      220
   index :          1         2         3         4         5
   
   lifetime :     0.0000    0.0000    0.0000    0.0000    0.0000

   type :          1         1         1         1         1

   pid :        211       111      -211      2112        11

   parent :          0         0         0         0         0

   daughter :          0         0         0         0         0

   px :     0.2810   -0.6545    0.4739    0.0502   -0.1506

   py :     0.5613    0.1094   -0.5558   -0.0458   -0.0691

   pz :     1.8363    3.0784    3.0298    0.3431    2.3125

   energy :     1.9456    3.1520    3.1197    1.0025    2.3184

   mass :     0.1396    0.1350    0.1396    0.9396    0.0005

   vx :     0.0000    0.0000    0.0000    0.0000    0.0000

   vy :     0.0000    0.0000    0.0000    0.0000    0.0000

   vz :    -0.0788   -0.0788   -0.0788   -0.0788   -0.0788

And the pid row give the PDG code for the particle in the LUND file,
and the order can be taken from left to right.

Constructing generated truth distributions
==========================================

This can be done by simply replacing the data reader HipoData with HipoTruth

::

   HipoTruth hdata;
   hdata.AddFile("/my/simulated/hipo/ouput/file.hipo");
                  #in your RunXXX.C file

instead of
::
   
   HipoData hdata;
   hdata.AddFile("/my/realData/hipo/ouput/file.hipo");


The output tree will then contain variable distributions for the complete
generated events, with the same names as for the normal running.


Constructing reconstructed distributions
========================================

No further action is required. The output tree will automatically be appended
with branches corresponding to the truth variables. These will be prefixed by
"tru".


Constructing accepted truth distributions and truth matching
============================================================

When doing combitorial events the truth variables will always be the same
for each combination. However only 1 combination will correspond to the
true event. It is useful to know which combination is correct as so chanser
allows truth matching. This can be added as an action at Create time. Note
it is algorithm dependent, so in principle you may try different truth matching
algorithms. It is difficult to be 100% correct with this. Hopefully truth
information passed through from gemc will help with this when avaiable.

To add the truth matching action in your Create script
::
   
   //Truth Matching, before ParticleData so can add to that tree
   EventTruthAction etra("EventTruth");
   FS->RegisterPostKinAction(etra); //PostKin
               #in your CreateXXX.cpp file

You will then get an additional branch "Truth" in your ouput tree which will
be equal to 1 when the combination is truth matched and 0 when not. You should
use this to filter your events to get truth matched reconstructed distributions.



Correcting the FT particle z-vertex
===================================

Currently particles detected in the FT are given 0 vertex position. As the
target was offset from nominal CLAS12 0 this leads to offsets in Delta time
for FT particles. To correct this we need to apply the target offset. This
can be done by adding the correction action,

::

   ParticleCorrectionManager pVz{"FTelVz"};
   pVz.AddParticle("e-",new FTel_VzCorrection(-0.03));//3cm shift
   FS->RegisterPreTopoAction(pVz); //PRETOPO
               #in your CreateXXX.cpp file

Warning, this behaviour may change with future reconstructed data.
