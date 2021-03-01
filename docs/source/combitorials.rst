Combitorial Events
------------------

One key feature of chanser is the ability to automatically generate
combitorial events out of single DST events. This removes the need
for the users to write specific loops which can often complicate code
and become a source for hard to track errors.
In fact the user is not required to write any extra code, but should be
aware of what is going on.
The need for combitorial events arises when your event topology does not
have a clear unambigous selection of particles. This can occur when,

- there are more particles of a particular type than you require
- there is no clear particle identification for a particle/track (or you want to try you own)

In theses cases you may either reject the event or try all possible
combinations of particles which fit your event hypothesis, chanser does the latter.

Combitorials of the first kind
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


If there are multiple particles in an event that meet the requirements of
a particle you ask for in you final state topology then a combitorial event
wil be made for each candidate and written as a seperate event in the output files.

*Examples,*

- final state contains 1 electron, but event has 3 => create 3 events
- final state contains 1 electron and 1 proton, but event has 3 e- and 2 p  => create 3x2=6 events
       both cases must subtract false combinations via exclusivity variables


Combitorials of the second kind
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If there are multiple particles in your final state of the same particle species.
This now depends on whether the particles came from a common intermediate parent
or not.
i.e. If they came from the same parent only 1 combitorial event is produced. If they
came from no parents then only 1 combitorial event is produced. If 1 or more came from
an parent which subsequently decayed then multiple combitorial events are produced.

*Examples*

- final state contains 2 photons from pi0 (parent) decay => 1 combitorial event
   
- final state contains 3 photons from omega->pi0+gamma decay => 3 comitorial event
       must subtract false combinations via pi0 mass distribution
- final state contains 2pi+ and 2pi- from omega->pi+pi0pi- and additional pi+pi => 4 combitorial events
       must subtract background via omega mass distribution
- final state contains 2pi+1pi- from decay of broad meson resonaces => 1 combitorial event
       as this must be dealt with at amplitude level by symmetrising pions, even if there is a isobar intermediate state


  
Combitorials of the third kind
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     
If you do not have full particle identification for you tracks you can make hypothesis based
on their assigned charge. The leads to making combitorial events based on the actual type
of the particles in your final state.

*Examples*

If identifying particles by charge

- final state contains proton and pi+ => 2 combitorial events from 2 +ve tracks
       can reject combitorials based on DeltaTime or some other track ID variables



Combining Combitorials of different kinds
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It may be your final state and data contain two or three of these types of combitorial. chanser
will combine these and write out however many events they create.

*Examples*

.. container:: toggle

    .. container:: header

        **Final state is e-,pi+pi-proton, event contains 3 -ve and 4 +ve tracks, how many combitorial events should be produced?**

    .. code-block:: xml
       :linenos:

	  There are 3 type 1 combinations of -ve particles, for each there are 2 type 3 combinations
	  There are 4 type 1 combinations of +ve particles, for each there are 3 type 3 combinations
	  Total combinations = 3 x 2 x 4 x 3 = 72
	  For this event chanser will create 72 possible combitorial events which the user can reduce
	  through cuts on particle ID variables and exclusivity variables.

|

	  
You can ask chanser to print the final state particles for each combitorial event to allow you to check it is doing what you woud like. To do this in your Run.C script call FinalStateManger::CheckCombitorials()

::

   {
   ...
   FinalStateManager fsm;
   fsm.SetBaseOutDir("/hdd/dump/Test");
   ////Connect the data to the manager
   fsm.LoadData(&hdata);
   ////load one or more FinalStates 
   fsm.LoadFinalState("Pi2","example.root");
   ...
   fsm.CheckCombitorials();
   
   fsm.ProcessAll(100); //show first 100 events

   }

|

	  
.. container:: toggle

    .. container:: header

        **To see an example chanser print out of these combitorials for one hipo event click here**

    .. code-block:: xml
       :linenos:
  
	  Printing Perm 1 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 2 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.0451417)E( 2.9292)       
	  Printing Perm 3 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 4 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 5 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 6 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 7 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 8 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 9 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 10 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 11 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 12 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 13 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 14 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 15 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 16 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 17 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 18 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 19 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 20 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 21 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 22 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 23 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 24 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 25 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 26 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 27 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 28 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 29 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 30 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 31 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 32 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 33 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 34 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 35 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 36 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 37 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 38 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 39 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 40 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 41 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 42 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 43 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 44 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 45 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 46 of topology 
	  PDG(11)Th(0.356374)E( 2.24458)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 47 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 48 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.356374)E( 2.24891)  
	  Printing Perm 49 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 50 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 51 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 52 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 53 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 54 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 55 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 56 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 57 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 58 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 59 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.202987)E( 2.63672)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 60 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.202987)E( 2.46808)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 61 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 62 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 63 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 64 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 65 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 66 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 67 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.73788)E( 1.17115)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 68 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.73788)E( 0.714648)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 69 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 70 of topology 
	  PDG(11)Th(0.0451417)E( 3.0794)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.317323)E( 1.27651)  
	  Printing Perm 71 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.589328)E( 1.16367)   PDG(211)Th(0.996051)E( 0.454749)   PDG(-211)Th(0.0451417)E( 2.9292)  
	  Printing Perm 72 of topology 
	  PDG(11)Th(0.317323)E( 1.26885)   PDG(2212)Th(0.996051)E( 1.03328)   PDG(211)Th(0.589328)E( 0.702326)   PDG(-211)Th(0.0451417)E( 2.9292)

|

Setting combitorial behaviour for configured FinalState objects
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The combitorial behaviour is set when you configure your final state object
in the Create.C script before writing an instance to a ROOT file. In this
way it is straightforward to create final state objects with different behaviour
and save them to different ROOT files which can then be loaded when processing
the data resuting in different ouput trees for each combitirial behaviour.

Example code for configuring your final state combitorials :

::
   
   {
   auto useEBPidFor = "ALL"; //or "NONE"
   auto useInclusiveFilterFor = "ALL";
   auto FS = dglazier::Pi2::Make(useEBPidFor,useInclusiveFilterFor);
   ...

|

Where useEBPidFor may be "ALL" or "NONE" and tells the iterator whether
to use hipo DST EvenBuilder Pid to determine combitorials ("ALL") or
just use the track charge ("NONE").
See :ref:`Combitorials of the third kind`

|

And useInclusiveFilterFor specifies which particles do not need an exact numerical match with your reuested topology. So "ALL" means you can have any number of any type of particles and it will produce events for all the possible combinations i.e. :ref:`Combitorials of the first kind` . If it is set to "NONE" then only events with exact particle matches will be processed and there will be no first kind combitorials.

|

:ref:`Combitorials of the second kind` depend on whether particles in your final state come from common short-lived parents or not and so you must tell your code if this is the case. In general this is setup at the skeleton code stage via the line

::

   s.SetFinalStateParents("Lambda:Lambda0;Proton;Pim");

In this example the final state proton and pi- come from a common Lambda parent. If there was only an additional K+ in this final state then no combitorials wold be required for this. But if there was another pi- additional combinations would be required.

Note this creates the following lines of code in the FinalState class Define() function

::

   //Set final state parents
    AddParticle("Lambda",&_lambda,kTRUE,-1);
    ConfigParent(&_lambda,&_proton);
    ConfigParent(&_lambda,&_pim);


A more common example would be for example 2pi0 production

::

   s.SetFinalStateParts("Electron:e-,Proton:proton,Gamma1:gamma,Gamma2:gamma,Gamma3:gamma,Gamma4:gamma");
   s.SetFinalStateTopo("Electron:Proton:Gamma1:Gamma2:Gamma3:Gamma4");
   s.SetFinalStateParents("Pi0_1:pi0;gamma1;gamma2,Pi0_2:pi0;gamma3;gamma4");


Now we have 4 gammas to make 2 pi0s which gives 4 permuations. If we did
not give the gammas parents then chanser would only produce 1 combitorial event.
