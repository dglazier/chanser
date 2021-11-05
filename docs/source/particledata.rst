Particle Data Actions
~~~~~~~~~~~~~~~~~~~~~

*Warning, you should not generate particle data trees for whole data sets, several files should be enough as the output files can get very large and slow down processing*

When using your own classes they should be put in a directory which is
linked to via the enviroment variable $CHANSER_CLASSES . Make sure this
variable is set before using the skeleton class maker.

Use chanser_skeleton to make template class code, this code will inherit
from CLAS12ParticleOutEvent,

::

     chanser_skeleton CreateClassParticleData.C name=MyParticleOutEvent

(Please choose something more meaningful than MyParticleOutEvent, this
is just for example.)

The class .h and .cpp files should now be in $CHANSER_CLASSES

::

     ls $CHANSER_CLASSES
     LoadMyClasses.C  MyParticleOutEvent.cpp  MyParticleOutEvent.h

The file LoadMyClasses.C is generated automatically the first time you
make a class. And is used to compile and load your class at run time
with the line,

::

     Loader::Compile("$CHANSER_CLASSES/MyParticleOutEvent.cpp");

You can now edit MyParticleOutEvent.h to include data members you want
to write to ntuple and to define the function FillData(). For example a
very simple particle data would be just to write the particle momentum.
For this I add the data member P,

::

       /////////////////////////////////////////////////

       /////////////////////////////////////////////////
       /// data member for tree branches below here
       /// Experiment developers add your own data members
       /// if you want them in the tree

       Double_t P=0;

And define the FillData function,

::

       inline void MyParticleOutEvent::FillData(){

         //move the c12p object to this particle for getting detector data
         auto c12p=_part->CLAS12();

         P=c12p->getP(); //Get the momentum from clas12root region_particle
       }

This class will now be compiled and loaded when you run chanser_root.
The source code will be saved in the configured root file and
automatically loaded for running with chanser_proof.

To use it with a ParticleDataManager,

::

   /////Make particle trees first in case want to add cut flags
   ParticleDataManager pdm{"particle",1}; //1 means append FinalState output data to this ntuple, replace with 0 if you do not need this
   pdm.SetParticleOut(new MyParticleOutEvent);
   FS->RegisterPostKinAction(pdm);       

The resulting trees can be found in the output directory in sub directory particleData/ParticleVariables_X.root, where the X refers to the topology number.
