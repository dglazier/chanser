CLAS12 HIPO Analyser

## Installation

Prerequisites, ROOT and clas12root https://github.com/JeffersonLab/clas12root

git clone https://github.com/dglazier/chanser

cd chanser

setenv CHANSER $PWD

mkdir build

cd build

cmake ../

make install

#For some reason to copy the .pcm files this needs run twice...

cmake ../

make install

## Creating your own final state class

Note you should set the variable CHANSER to the install directory above

And add the bin to your path

setenv PATH "$PATH":"$CHANSER/bin"

mkdir /somewhere/to/create/my/finalstates

cd  /somewhere/to/create/my/finalstates

### Example 4 charged pion production

mkdir Pi4

cd Pi4

Create a script to produce skeleton code for this reaction, e.g. create a file called Skeleton.C with the following code

       {
       //skeleton code maker
       
       FSSkeleton s;

       //Give your class a name

       s.SetFinalState("Pi4");

       //Define the possible detected particles in the final state

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

Now run this script in ROOT, having loaded the skeleton library

    	 chanser_skeleton Skeleton.C

You should now have the following files

    ls
    Create_Pi4.C  Pi4.cpp  Pi4.h  Run_Pi4.C  Skeleton.C  TreeDataPi4.h

You can check it compiles and loads OK

    chanser_root Pi4:Pi4.cpp

Where Pi4 is the name of your final state class.

### Using your class

There are 3 main ways to develop this class

1) Define topology dependent behaviour

2) Define overall reaction kinematics and other general quantities

3) Define which of these to write to the output file

#### 1) Defining topology depednent behaviour

To do this we must define the _doForTopo["topology"] function body for each topology we defined. Here "topology" means specific detected particles so will be repalaced with something like "Electron:Proton:Pip1:Pip2:Pim1:Pim2" in the Pi4 example. Each of the particles you defined in your final state will have a data memeber in the final state class associated with it. e.g in Pi4 there are the following declartions in the Pi4 class in P4.h :

    //Final Particles Detected
    Particle   _electron = Particle{"e-"};
    Particle   _proton = Particle{"proton"};
    Particle   _pip1 = Particle{"pi+"};
    Particle   _pip2 = Particle{"pi+"};
    Particle   _pim1 = Particle{"pi-"};
    Particle   _pim2 = Particle{"pi-"};
 
In the _doForTopo functions the particles will be updated automatically and you may just use them directly. These particles contain things like 4-vector information as well as a direct link to the clas12root particle which then gives access to any of the DST information associated with that particle. For example I can calculate the missing mass for the event :

       _doToTopo["Electron:Proton:Pip1:Pip2:Pim1:Pim2"]=[&](){
       //TOPOLOGY Define your topology dedendent code in here
       ///////+++++++++++++++++++++++++++++++++++///////
      	   auto miss= _beam + _target - _Electron.P4() - _Proton.P4()
      	   -_Pip1.P4()-_Pip2.P4() -_Pim1.P4() -_Pim2.P4();

	   TD->MissMass=miss.M();
      	   TD->MissMass2=miss.M2();
      
      ///////------------------------------------///////
      };

Here _electron, _proton etc are CLAS12Particles and so we have to call the P4() function to get their lorentz vectors. _beam, _target and miss are lorentz vectors (although not TLorentzVectors, they are ROOT::MATH genvector LorentzVectors).

Anything prefixed by  TD-> has to be included in the TreeData and will be written to the output.

#### 2) Define overall reaction kinematics and other general quantities

This is done in the Kinematics function. This will only be called if previous cuts have been passed (for example from PostTopoAction ParticleCuts).

Some Electron scattering variables are given by default,

     //Use Kinematics to calculate electron variables
     _kinCalc.SetElecsTarget(_beam,_Electron.P4(),_target);
     TD->W=_kinCalc.W(); //photon bem energy
     TD->Q2=_kinCalc.Q2();
     TD->Pol=_kinCalc.GammaPol();
     TD->Egamma=_kinCalc.GammaE();

Again anything to be written to the output tree is prefixed with TD->

Here the kinematic calculator is used, it can also be used for resonance decay kinematics and other appropriate functions can be added.

     //calculate meson Lorentz Vector
     auto meson = _K1.P4() + _K2.P4();
     TD->MesonMass = meson.M(); 

     //Caclulate X->2K0 decay angles
     _kinCalc.SetMesonBaryon(meson,_Proton.P4());
     _kinCalc.SetMesonDecay(_K1.P4() , _K2.P4());
     _kinCalc.MesonDecayGJ();
     TD->MesonCosThGJ=_kinCalc.CosTheta();
     TD->MesonPhiGJ=_kinCalc.CosTheta();

#### 3) Define which of these to write to the output file

This is just done by adding data members to the TreeData class. Here it is called TreeDataPi4. Inclduing the extra variables I am using,

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

### Configuring an analysis

You can now configure an object of this class adding combitorial algorithms, cuts, outputs. There is a template for starting this in the Create_Pi4.C script.


    chanser_root Pi4:Pi4.cpp Create_Pi4.C

First you must choose the combitorial algorithm you would like for this conficuration and create an instance of your class using the Make function (this creates a unique_ptr which ROOT is respnsible for deleting at the end of the session,

The first "ALL" => use EventBuilder PID for all particles, other options can be "NONE"

The second "ALL" => particles to be inclusive for (i.e. any number of these particles allowed in the event). alternative "" for NONE, "e-:proton" for inclusive electrons and protons only


      auto FS = dglazier::Pi4::Make("ALL","ALL");

Now choose however many of the topologies you have defined that you want to include in this configuration

      FS->AddTopology("Electron:Proton:Pip1:Pip2:Pim1:Pim2");

Choose format for output tree, if none given no output tree written

      FS->UseOutputRootTree();
      ///FS->UseOutputHipoNtuple();

Now add different Actions.

### Particle cuts

For example you can add different particle ID cuts for different particles :

    ParticleCutsManager pcm{"EBCuts",1};
    EventBuilderCut ebc; //just use even  builder PID
    pcm.AddParticleCut("e-",ebc);
    pcm.AddParticleCut("proton",ebc);
    pcm.AddParticleCut("pi+",ebc);
    DeltaTimeCut dtc(2); //Apply a Delta Time cut of 2ns
    pcm.AddParticleCut("pi-",dtc);

    //register it with this final state instance
    FS->RegisterPostTopoAction(pcm);

### Particle data

Or output data related to each particle in the event to a root tree :

     ParticleDataManager pdm{"particle","/outdir/particleData",1};
     CLAS12ParticleOutEvent0 pout; //instance of class that defines particle output
     pdm.SetParticleOut(&pout);
     FS->RegisterPostTopoAction(pdm);

This will output a set of standard detector variables,  you may create you own ParticleOutEvent class for this purpose.

At the end you should write to a root file so it can be processed. The clas12_proof processor then just needs this root file to run as it extracts and compiles the source code from the file before running.

   FS->WriteToFile("ALLALL_configuration.root");


### Running the analyse

To run the FinalState analysis you must add your object and data to the FinalStateManager. There is a template for this copied as part of your skeleton code Run_*.C

To set the data file

      ////Set hipo file to be analysed
      HipoData hdata;
      hdata.SetFile("/input/dir/file.hipo");

To create FinalStateManager and give an output directory

      FinalStateManager fsm;
      fsm.SetBaseOutDir("/output/directory");

Note all outputs will be written to sub-directories within /output/directory.

To Load your final state analysis objects

      fsm.LoadFinalState("Pi4", "ALLALL_configuration1.root");
      fsm.LoadFinalState("Pi4", "NONEALL_configuration1.root");

And run

      fsm.ProcessAll();

### Running the analyse with PROOF

To take advantage of parallel processing and chains of many files you can use chanser_proof.
In place of the previous method o running tha analysis just create a tect file with your class name and object file, i.e. finalstate.txt >>

      Pi2 /full/path/ALLALL_configuration1.root
      Pi4 /full/path/NONEALL_configuration1.root


Note you can add as many analyses as you like, and they may be of different classes and from different users.

Create a script to allocate the data files, e.g. Processor.C

       clas12root::HipoChain chain;
       chain.Add("/full/path/files_*.hipo");

Create processor with list of final state analysis and output directory, remembering your ourtput files may be large.

       chanser::HipoProcessor processor(&chain,"finalstates.txt","/out/dir");

Then process all the files

      gProof->Process(&processor,chain.GetNRecords());

Note if you like you can replace chain.GetNRecords() with any number of records you wish to analyse as long as <chain.GetNRecords(). Typically for clas12 DSTs a record may consist of around 100 actual events