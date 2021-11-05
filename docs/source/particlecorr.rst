Particle Correction Actions
~~~~~~~~~~~~~~~~~~~~~~~~~~~


When using your own classes they should be put in a directory which is
linked to via the enviroment variable $CHANSER\_CLASSES . Make sure this
variable is set before using the skeleton class maker.

Use chanser\_skeleton to make template class code, this code will
inherit from BaseCut,

::

      chanser_skeleton CreateClassParticleCorrection.C name=MyParticleCorrection

(Please choose something more meaningful than MyParticleCorrection, this
is just for example.)

The class .h and .cpp files should now be in $CHANSER\_CLASSES

::

      ls $CHANSER_CLASSES
      LoadMyClasses.C  MyParticleCorrection.cpp  MyParticleCorrection.h

The file LoadMyClasses.C is generated automatically the first time you
make a class. And is used to compile and load your class at run time
with the line,

::

      Loader::Compile("$CHANSER_CLASSES/MyParticleCorrection.cpp");

You can now edit MyParticleCorrection.h to define your own correction.
The principle is you can access a clas12root region\_particle in the
ParticleCorrect function and you can access any of the DST data for that
particle from this and so base your correction on any amount of particle
detector information. Most useful is probably getRegion as corrections
are likely to be different for FT, FD and CD. Here I will just multiply
the particle momentum by a constant factor,

::

        void ParticleCorrect(chanser::BaseParticle* part) const noexcept override{
          auto c12p = static_cast<const chanser::CLAS12Particle*>(part);
          auto c12=p->CLAS12(); //if you require other DST data
          if(c12->getRegion()!=clas12::FD) return; //e.g only FD correction
      
          Double_t p_mag = part->P4p()->P();
          p_mag*=_a0;  //apply correction...
          part->SetP(p_mag_corrected);
        }

Note if I include auto *c12=p->CLAS12(); *\ then I can use c12 to access
DST data, e.g.\_ c12->cal(PCAL)->getEnergy();\_

You can add as many parameters as you want through the constructor,
adding each as a data member of the class

i.e. I add the contructor looks like

::

      MyParticleCorrection(Float_t a0)_a0{a0}{}

And include the data member

::

      Float_t _a0{0};

You should also edit the print statement to give a meaningful summary of
your correction so the final state class can give a full report of its
algorithms

::

       void Print(Option_t* option = "")const final{
         std::cout<<"\t\t"<<ClassName()<<" with parameter ";
         std::cout<<_a0<<std::endl;
       }

To use it with a ParticleCorrectionManager I register it pretopo so it
is already applied before the topology function,

::

      ParticleCorrectionManager pcorrm{"IMadeThisCorrection"};
      pcorrm.AddParticle("e-",new MyParticleCorrection(1.1)); //use it on electrons
      pcorrm.AddParticle("pi+",new MyParticleCorrection(0.9)); //use it pi+
      FS->RegisterPreTopoAction(pcorrm); //PRETOPO
