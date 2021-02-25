StartTime Actions
~~~~~~~~~~~~~~~~~

Different methods for defining the event start time may be used. This is
done by loading a StartTimeAction to PreTopoActions when configuring the
FinalState object.

It is recommended to try :ref:`Start time from particle` which should
be more reliable when trying different combinations of particles in an
event whihc may have come rfom different beam buckets.
|

You may just use the Event Builder particle vertex
time, which corrects the EventBuilder start time for individual particle
vertex position,

::

     auto FS = someone::Pi4::Make("NONE","ALL");
     ....
     ///StartTime
     StartTimeAction st("EBStartTime",new C12StartTimeFromVt());
     FS->RegisterPreTopoAction(st);

If you are using the Forward Tagger for start time
i.e. REC::FTParticle::vt

::

     ///StartTime
     StartTimeAction st("EBStartTime",new C12StartTimeFromVtFTB());
     FS->RegisterPreTopoAction(st);

If you just want to use EB start time without vertex correction

::

     ///StartTime
     StartTimeAction st("EBStartTime",new C12StartTime());
     FS->RegisterPreTopoAction(st);

Or FT based

::

     ///StartTime
     StartTimeAction st("EBStartTime",new C12StartTimeEBFTB());
     FS->RegisterPreTopoAction(st);

You may add you own class deriving from C12StartTime by defining the
function

::

      Double_t withParticle(CLAS12Particle* p) const

Where p is the particle to have the start time subtracted.


Start time from particle
========================

A more advanced example is given in C12StartTimeFromParticle, which
allows you to specify which particle to take the start time from and
recaclulates it for each combitorial event. This is useful in case you
want to analyse events with more than 1 electron candidate for example.

::

     ///StartTime
     StartTimeAction st("StartTime",new C12StartTimeFromParticle("Electron"));
     FS->RegisterPreTopoAction(st);

Or there is an option to choose the highest momentum particle from
your final state to calculate the start time from

::

   StartTimeAction st("StartTime",new C12StartTimeFromHighMomentum("Electron:Pip:Pim"));


   
