Actions
=======

Once you have setup your standard final state operations : filter event particles; calculate missing 4-momentum; calculate rection kinematics; write to tree. You can supplement your analysis with Action plugins. For example you may add standard procedures such as momentum corrections, fiducial cuts. You may tune the particle data you wish to write to your output file, you may want to try a TMVA classification.

For code developers, rather than users, to allow Action classes to be automated over a list of files where Action parameter values may change, they must inherit from CLAS12Base and implement the ChangeRun function. Here they have access to rcdb, ccdb and anadb from which to extract parameters from.

.. toctree::

   starttime.rst
   particledata.rst
   particlecut.rst
   particlecorr.rst


Quick Usage
-------------

Start Time Actions
******************

To use a start time action in your Create script (Note you may only use 1 of these )

::
   
     StartTimeAction st("StartTime",new C12StartTimeFromParticle("Electron"));
     StartTimeAction st("StartTime",new C12StartTimeFromHighMomentum("Electron:Pip:Pim"));
     StartTimeAction st("StartTime",new C12StartTime()); 
     StartTimeAction st("StartTime",new C12StartTimeFromEBFTB());
     StartTimeAction st("StartTime",new C12StartTimeFromVtFTB());
     FS->RegisterPreTopoAction(st);  //PRETOPO

For full details of what each does see :ref:`StartTime Actions`.

Particle Cuts Action
********************

Apply Delta time cut of 2ns to all particles

::

     ParticleCutsManager pcm{"DeltaTimeCuts",1}; //1 means will actually apply this cut to output data
     pcm.SetDefaultCut(new DeltaTimeCut(2));//Apply a Delta Time cut of 2ns
     // pcm.SetDefaultCut(new DeltaTimeVerCut(2));//USe this if using C12StartTimeFromParticle
     FS->RegisterPostTopoAction(pcm);

Particle Data Action

Add default particle detector data to a new tree and file. Users may derive
there own to replace CLAS12ParticleOutEvent0. The resulting trees can be found
in the output directory in sub directory particleData/ParticleVariables_X.root, where the X refers to the topology number.

::

   ParticleDataManager pdm{"particle",1};
   pdm.SetParticleOut(new CLAS12ParticleOutEvent0);
   FS->RegisterPostKinAction(pdm);

   
