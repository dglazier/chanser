Getting Started
================

CLAS12 HIPO Analyser

Installation
------------

Prerequisites, ROOT and clas12root
https://github.com/JeffersonLab/clas12root

git clone https://github.com/dglazier/chanser

cd chanser

setenv CHANSER $PWD

mkdir build

cd build

cmake ../

make install


Creating your own final state class
-----------------------------------

Note you should set the variable CHANSER to the install directory above

And add the bin to your path

setenv PATH "$PATH":"$CHANSER/bin"

mkdir /somewhere/to/create/my/finalstates

cd /somewhere/to/create/my/finalstates

Example 4 charged pion production
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

mkdir Pi4

cd Pi4

Create a script to produce skeleton code for this reaction, e.g. create
a file called Skeleton.C with the following code

::

       {
       //skeleton code maker
       
       FSSkeleton s;

       //Give your class a name

       s.SetFinalState("Pi4");

       //Define the possible detected particles in the final state
       //Name1:species1,Name2:species2,...
       //where species is given in $ROOTSYS/etc/pdg_table.txt
       //e.g Electron:e-,Proton:proton,...
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

The species names used for particle should be those given in
$ROOTSYS/etc/pdg\_table.txt.

Now run this script in chanser\_skeleton

::

         chanser_skeleton Skeleton.C

You should now have the following files

::

    ls
    Create_Pi4.C  Pi4.cpp  Pi4.h  Run_Pi4.C  Skeleton.C  TreeDataPi4.h

You can check it compiles and loads OK

::

    chanser_root Pi4:Pi4.cpp

Where Pi4 is the name of your final state class.

Please note that all classes are given an additional C++ namespace equal
to the username of the person who created the skeleton class. This is to
prevent conflcicts when multiusers load finalstates at the same time
which may have the dame classname. The namespace means that the full
class name will be like dglazier::Pi4, this is required when creating
objects of your class in ROOT scripts.

Using your class
~~~~~~~~~~~~~~~~

There are 3 main ways to develop this class

1) Define topology dependent behaviour

2) Define overall reaction kinematics and other general quantities

3) Define which of these to write to the output file

1) Defining topology dependent behaviour
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To do this we must define the \_doForTopo["topology"] function body for
each topology we defined. Here "topology" means specific detected
particles so will be repalaced with something like
"Electron:Proton:Pip1:Pip2:Pim1:Pim2" in the Pi4 example. Each of the
particles you defined in your final state will have a data memeber in
the final state class associated with it. e.g in Pi4 there are the
following declartions in the Pi4 class in P4.h :

::

    //Final Particles Detected
    Particle   _electron = Particle{"e-"};
    Particle   _proton = Particle{"proton"};
    Particle   _pip1 = Particle{"pi+"};
    Particle   _pip2 = Particle{"pi+"};
    Particle   _pim1 = Particle{"pi-"};
    Particle   _pim2 = Particle{"pi-"};

    #in Pi4.h

In the \_doForTopo functions the particles will be updated automatically
and you may just use them directly. These particles contain things like
4-vector information as well as a direct link to the clas12root particle
which then gives access to any of the DST information associated with
that particle. For example I can calculate the missing mass for the
event :

::

       _doToTopo["Electron:Proton:Pip1:Pip2:Pim1:Pim2"]=[&](){
       //TOPOLOGY Define your topology dedendent code in here
       ///////+++++++++++++++++++++++++++++++++++///////
          auto miss= _beam + _target - _electron.P4() - _proton.P4()
                 -_pip1.P4()-_pip2.P4() -_pim1.P4() -_pim2.P4();

          TD->MissMass=miss.M();
      TD->MissMass2=miss.M2();
      
      ///////------------------------------------///////
      };

      #in Pi4.cpp

Here \*\electron, \*\ proton etc are CLAS12Particles and so we have to call
the P4() function to get their lorentz vectors. \*\beam, \*\ target and
miss are lorentz vectors (although not TLorentzVectors, they are
'ROOT::MATH genvector' LorentzVectors).

Anything prefixed by TD-> has to be included in the TreeData and will be
written to the output.

If you had any missing or parent particle you may choose to assign their
4-vectors here or in the Kinematics function. You can use the SetP4 or
FixP4, the latter fixes the particle mass to the PDG value and
recalculates the energy. In general this will be different for differnt
topologies

::

      _pim2.FixP4(miss);

and/or

::

      _k1.FixP4(_pip1.P4()+_pim1.P4());
      _k2.FixP4(_pip2.P4()+_pim2.P4());
      
      #in Pi4.cpp

If it anytime you decide that you do not want to keep this event
combination you can call RejectEvent(); and return;

2) Define overall reaction kinematics and other general quantities
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This is done in the Kinematics function. This will only be called if
previous cuts have been passed (for example from PostTopoAction
ParticleCuts).

Some Electron scattering variables are given by default,

::

     //Use Kinematics to calculate electron variables
     _kinCalc.SetElecsTarget(_beam,_electron.P4(),_target);
     TD->W=_kinCalc.W(); //photon bem energy
     TD->Q2=_kinCalc.Q2();
     TD->Pol=_kinCalc.GammaPol();
     TD->Egamma=_kinCalc.GammaE();

     # in Pi4.cpp

Again anything to be written to the output tree is prefixed with TD->

Here the kinematic calculator is used, it can also be used for resonance
decay kinematics and other appropriate functions can be added.

::

     //calculate meson Lorentz Vector
     auto meson = _k1.P4() + _k2.P4();
     TD->MesonMass = meson.M(); 

     //Caclulate X->2K0 decay angles
     _kinCalc.SetMesonBaryon(meson,_proton.P4());
     _kinCalc.SetMesonDecay(_k1.P4() , _k2.P4());
     _kinCalc.MesonDecayGJ();
     TD->MesonCosThGJ=_kinCalc.CosTheta();
     TD->MesonPhiGJ=_kinCalc.CosTheta();

     # in Pi4.cpp
     

Again If it anytime you decide that you do not want to keep this event
combination you can call RejectEvent(); and exit the function

::

      if(TD->MesonMass>5) {RejectEvent(); return;} //will not save
      
     # in Pi4.cpp

3) Define which of these to write to the output file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This is just done by adding data members to the TreeData class. Here it
is called TreeDataPi4. Inclduing the extra variables I am using,

::

     //data member for tree branches below here
     Double_t MissMass=0;
     Double_t MissMass2=0;
     Double_t K1Mass=0;
     Double_t K2Mass=0;

     //example of e- kinematics
     //you can remove these if not using
     Double_t W=0;
     Double_t Q2=0;
     Double_t Pol=0;
     Double_t Egamma=0;

     //Meson stuff
     Double_t MesonMass=0;
     Double_t MesonCosThGJ=0;
     Double_t MesonPhiGJ=0;


     # in TreeDataPi4.h

Configuring an analysis
-----------------------

You can now configure an object of this class adding combitorial
algorithms, cuts, outputs. There is a template for starting this in the
Create\_Pi4.C script.

::

    chanser_root Pi4:Pi4.cpp Create_Pi4.C

First you must choose the combitorial algorithm you would like for this
conficuration and create an instance of your class using the Make
function (this creates a unique\_ptr which ROOT is respnsible for
deleting at the end of the session,

The first "ALL" => use EventBuilder PID for all particles, other options
can be "NONE"

The second "ALL" => particles to be inclusive for (i.e. any number of
these particles allowed in the event). alternative "" for NONE,
"e-:proton" for inclusive electrons and protons only

::

      auto FS = dglazier::Pi4::Make("ALL","ALL");

      #in Create_Pi4.C

Now choose however many of the topologies you have defined that you want
to include in this configuration. These strings should be the same as those
in the _doToTopo functions in your .cpp file.

::

      FS->AddTopology("Electron:Proton:Pip1:Pip2:Pim1:Pim2");

      #in Create_Pi4.C

Choose format for output tree, if none given no output tree written

::

      FS->UseOutputRootTree();
      ///FS->UseOutputHipoNtuple();

      #in Create_Pi4.C

Now add different Actions.

Note the output tree file is given the name
/basedir/username/config\_file/FinalState.root , where config\_file is
the string given to FS->WriteToFile below, username is $USER, and
basedir is given in the Run\_XXX.C script later as the basedirectory to
put all analysis outputs.

Particle cuts
~~~~~~~~~~~~~

For example you can add different particle ID cuts for different
particles :

::

    ParticleCutsManager pcm{"EBCuts",1}; //the 1 => cut will be applied

    pcm.AddParticleCut("e-",new EventBuilderCut());
    pcm.AddParticleCut("proton",new EventBuilderCut());
    pcm.AddParticleCut("pi+",new EventBuilderCut());
    pcm.AddParticleCut("pi-",new DeltaTimeCut(2));

    //register it with this final state instance
    FS->RegisterPostTopoAction(pcm);

      #in Create_Pi4.C

Note you can include as many different ParticleCutsManagers in you
analysis as you want. For example you could hae one with all particles
having DeltaTime cuts of 1ns and another with 2ns.

::

      ParticleCutsManager pcm_dt{"DeltaTimeCuts2",0};
      pcm_dt.AddParticleCut("e-",new DeltaTimeCut(2));
      pcm_dt.AddParticleCut("proton",new DeltaTimeCut(2));
      pcm_dt.AddParticleCut("pi+",new DeltaTimeCut(0.5));
      pcm_dt.AddParticleCut("pi-",new DeltaTimeCut(1));
      FS->RegisterPostTopoAction(pcm_dt);

      #in Create_Pi4.C

Note the argument 1 provided in pcm{"EBCuts",1}, means that this cut
will actually be applied to the data, if this is not included or a 0 is
used instead then the cut is just included as a flag in the ouput tree.
So in this case "EBCuts" will be applied to the output data, while
DeltaTimeCuts2 will just appear as a flag in the ouptput tree with a
value equal to the number of particles that past the cut.

To set a default cut for particles not called in AddParticleCut,

::

      ParticleCutsManager pcm{"DeltaTimeCuts",1};
       pcm.SetDefaultCut(new DeltaTimeCut(2));

SetDefaultCut and AddParticleCut can be used in the same
ParticleCutsManager

Particle data
~~~~~~~~~~~~~

Or output data related to each particle in the event to a root tree :

::

       ParticleDataManager pdm{"particle",1};
       pdm.SetParticleOut(new CLAS12ParticleOutEvent0);
       FS->RegisterPostKinAction(pdm);


     #in Create_Pi4.C

This will output a set of standard detector variables, you may create
you own ParticleOutEvent class for this purpose.

Start time
~~~~~~~~~~

You also have to decide where to get the event start time from. See the
FAQ for details, to take the particle vertex time from the event builder

::

    ///StartTime
    StartTimeAction st("EBStartTime",new C12StartTimeFromVt());
    FS->RegisterPreTopoAction(st);

     #in Create_Pi4.C

Or you can reccalculate the starttime for each combitorial
from the e- candidate (or other nominated particle),

::

    ///StartTime
    StartTimeAction st("StartTime",new C12StartTimeFromParticle("Electron"));
    FS->RegisterPreTopoAction(st);  //PRETOPO

     #in Create_Pi4.C
     
     
For full StartTime options see section :ref:`StartTime Actions`.

Particle Corrections
~~~~~~~~~~~~~~~~~~~~

Ths particle corrections action is used much like the particle cuts. So
far only a FT e- energy correction has been included. If you want to use
it,

::

     ParticleCorrectionManager pcorrm{"FTelEnergyCorrection"};
     pcorrm.AddParticle("e-",new FTel_pol4_ECorrection());
     FS->RegisterPreTopoAction(pcorrm); //PRETOPO

     #in Create_Pi4.C

See the FAQs for information on creating your own particle correction

At the end you should write to a root file so it can be processed. The
clas12\_proof processor then just needs this root file to run as it
extracts and compiles the source code from the file before running.

::

      FS->WriteToFile("ALLALL_configuration.root");
      FS->Print(); //summarise configuration

      #in Create_Pi4.C

Running the analyse
-------------------

To run the FinalState analysis you must add your object and data to the
FinalStateManager. There is a template for this copied as part of your
skeleton code Run\_\*.C

**Any time you want to change an analysis configuration or your final
state class you must run chanser\_root Create\_MyFS.C, before running
Run\_MyFS.C.**

To set the data file

::

      ////Set hipo file to be analysed
      HipoData hdata;
      hdata.AddFile("/input/dir/file.hipo");
      //hdata.Reader()->useFTBased();//use this if want FTBased ID

      #in Run_Pi4.C

To create FinalStateManager and give an output directory

::

      FinalStateManager fsm;
      fsm.SetBaseOutDir("/output/directory");

      #in Run_Pi4.C

Note all outputs will be written to sub-directories within
/output/directory.

To Load your final state analysis objects

::

      fsm.LoadFinalState("Pi4", "ALLALL_configuration1.root");
      fsm.LoadFinalState("Pi4", "NONEALL_configuration1.root");

      #in Run_Pi4.C

Limit the number of particle of each charge for each event

::

      fsm.GetEventParticles().SetMaxParticles(6);

And run

::

      fsm.ProcessAll();

      #in Run_Pi4.C

To execute use chanser\_root,

::

      chanser_root Pi4:Pi4.cpp Run_Pi4.C

Running the analyse with PROOF
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To take advantage of parallel processing and chains of many files you
can use chanser\_proof.

In place of the previous method o running tha analysis just create a
tect file with your class name and object file, i.e. finalstate.txt

::

      Pi2 /full/path/ALLALL_configuration1.root
      Pi4 /full/path/NONEALL_configuration1.root

      #in finalstate.txt

Note you can add as many analyses as you like, and they may be of
different classes and from different users.

Create a script to allocate the data files, e.g. Processor.C

::

       clas12root::HipoChain chain;
       chain.Add("/full/path/files_*.hipo");
       chain.SetReaderTags({0});
       //chain.GetC12Reader()->useFTBased(); //if you want ot use FT PiD

      #in Processor.C

Create processor with list of final state analysis and output directory,
remembering your ourtput files may be large.

::

       chanser::HipoProcessor processor(&chain,"finalstates.txt","/out/dir");

Supply some options for the PROOF processing see FAQ for others,

::

       processor.AddOption("HIPOPROCESSOR_MAXPARTICLES","5");

Then process all the files

::

      gProof->Process(&processor,chain.GetNRecords());
       
      #in Processor.C

Note if you like you can replace chain.GetNRecords() with any number of
records you wish to analyse as long as <chain.GetNRecords(). Typically
for clas12 DSTs a record may consist of around 100 actual events

Now you can run this with chaser\_proof,

::

      chanser_proof 4 Processor.C

Good Luck.
