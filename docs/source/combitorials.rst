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

configure with

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
	  There are 4 type 1 combinations of +ve particles, for each there are 2 type 3 combinations
	  Total combinations = 3 x 2 x 4 x 2 = 48
	  For this event chanser will create 48 possible combitorial events which the user can reduce
	  through cuts on particle ID variables and exclusivity variables.


	  
.. container:: toggle

    .. container:: header

        **What do you think the answer is?**

    .. code-block:: xml
       :linenos:

	  is it this?
