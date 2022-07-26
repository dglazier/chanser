.. chanser documentation master file, created by
   sphinx-quickstart on Tue Feb  9 11:51:12 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to chanser's documentation!
===================================

.. toctree::
   :maxdepth: 2

   gettingstarted.rst
   combitorials.rst
   hipodata.rst
   actions.rst
   masks.rst
   anadb.rst
   clas12db.rst
   standardactions.rst
   simulations.rst
   machinelearning.rst

Some preamble
==================

* :ref:`FinalState`
  
  Analysing the Final State of a particular reaction
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  chanser is designed to analyse specific particle final states. In practise this
  can be achieved by detecting a subset of the final particles (missing particles)
  and so you may simulatanously define various topologies for your final state.

  For example, you may be interested in reactions with an e-, proton, pi+ and pi-.
  These are your final state particles. But you may be able to analyse events where,
  for example, the proton has not been detected. So you may define 1 topology with an
  exclusive final state (detect e-,p,pi+,pi-) and one with just (e-,pi+,pi-).

