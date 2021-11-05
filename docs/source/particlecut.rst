Particle Cut Actions
~~~~~~~~~~~~~~~~~~~~~
When using your own classes they should be put in a directory which is
linked to via the enviroment variable $CHANSER_CLASSES . Make sure this
variable is set before using the skeleton class maker.

Use chanser_skeleton to make template class code, this code will inherit
from BaseCut,

::

     chanser_skeleton CreateClassParticleCuts.C name=MyParticleCut

(Please choose something more meaningful than MyParticleCut, this is
just for example.)

The class .h and .cpp files should now be in $CHANSER_CLASSES

::

     ls $CHANSER_CLASSES
     LoadMyClasses.C  MyParticleCut.cpp  MyParticleCut.h

The file LoadMyClasses.C is generated automatically the first time you
make a class. And is used to compile and load your class at run time
with the line,

::

     Loader::Compile("$CHANSER_CLASSES/MyParticleCut.cpp");

You can now edit MyParticleCut.h to define your own cut. The principle
is you can access a clas12root region_particle in the ParticleCut
function and you can access any of the DST data for that particle from
this and so base your cut on any amount of particle detector
information. Here I will make a cut based on DeltaTime and particle
momentum from the CLAS12Particle class,

::

     Bool_t ParticleCut(const chanser::BaseParticle* part) const noexcept override{
       auto p = static_cast<const chanser::CLAS12Particle*>(part);
       //auto c12=p->CLAS12(); //if you require other DST data
       return (p->P4().P() <_cutP) && (TMath::Abs(p->DeltaTime())<_cutDtime) ?  kTRUE :  kFALSE;
     }

Note you do not have to make the return statement all on one line, you
can take several lines of code to define your cut if required. the LOGIC
? kTRUE : kFALSE part means kTRUE is returned if LOGIC is true, and
kFALSE if not. Note if I include auto *c12=p->CLAS12(); then I can use
c12 to access DST data, e.g. c12->cal(PCAL)->getEnergy();*

By default the skeleton code only provides 1 cut variable, but you can
add as many as you want through the constructor, adding each as a data
member of the class

i.e. default looks like

::

       MyParticleCut(Float_t cut){_cutVal=cut;}

For my momentum+DeltaTime cut I change it to

::

      MyParticleCut(Float_t p,Float dt)_cutP{p},_cutDTime{dt} {}

And replace the data member

::

     Float_t _cutVal=THIS_FLT_MAX;

with

::

     Float_t _cutP=THIS_FLT_MAX;
     Float_t _cutDtime=THIS_FLT_MAX;

You should also edit the print statement to give a meaningful summary of
your cut so the final state class can give a full report of its
algorithms

::

      void Print(Option* option = "")const final{
        std::cout<<"\t\t"<<ClassName()<<" with cut momentum "<<_cutP<<" and deltaTime "<<_cutDtime<<std::endl;
      }

To use it with a ParticleCutsManager,

::

     ParticleCutsManager pcm{"DeltaTimeCuts",1}; //1 means will actually apply this cut to data
     pcm.AddParticleCut("e-",new MyParticleCut(12,1));//just apply my cut to electrons
     DeltaTimeCut* dtc=new DeltaTimeCut(2); //Apply a Delta Time cut of 2ns
     pcm.SetDefaultCut(dtc);
     FS->RegisterPostTopoAction(pcm);

Note if pcm{"DeltaTimeCuts",1} has 0 instead of a 1 then the cut is not applied to the data, but it is just added to the final output tree.
