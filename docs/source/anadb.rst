CHANSER analysis database
=========================

Any action that is dependent on some parameters that change over time can be
configured via the analysis data base class AnaDB.

AnaDB allows the changing of parameters prior to a new run file starting.
Currently these parameters can be dependent on,

1. Run period, e.g. **fall_2018**
2. Data type : real experiment (use **EXP**); or simulation (use **SIM**)
3. Toroidal Field (+ve, -ve): **INBEND** ; or **OUTBEND**
4. Miscallanous tag. This may include, for example, a particle species.
5. Table name, so the classes that will use them can access them.


For each combination of the above settings, or any subset of, you can
provide parameters to be read in by an action class.

It is the developers responsibility to write the method for assigning
the parameters retrieved from AnaDB, dependent on these settings. AnaDB can
provide a table consisting up to 4 dimensions of doubles for the developer
to use.

The parameters are then written in a AnaDB text file for loading into an
analysis.

To include a set of parameters in you analysis you must provide the parameter
file to the HipoData object. You may provide any number of parameter files this
way or include them all in a top level file and just provide that,

::
   
   HipoData hdata;
   hdata.AddFile("/input/data1.hipo");
   hdata.AddFile("/input/data2.hipo");
   ...
   hdata.LoadAnaDB("$CHANSER/rga_actions/anadb/RGA_ACTIONS_PASS1.db");
   hdata.LoadAnaDB("$CHANSER/anadbs/RunPeriodPass1.db");
   hdata.SetRunPeriod("fall_2018");

If using chanser_proof you must supply the databases as PROOF options,

::
   
   clas12root::HipoChain chain;
   chain.Add("/input/data1.hipo");
   chain.Add("/input/data2.hipo");
   ...
   chain.SetReaderTags({0});
     
   chanser::HipoProcessor processor(&chain,"finalstates.txt","/output/");

   ...
   ...
   //Load database for run conditions and parameters
   processor.AddOption("HIPOPROCESSOR_ANADB","$CHANSER/rga_actions/anadb/RGA_ACTIONS_PASS1.db:$CHANSER/anadbs/RunPeriodPass1.db");
   //Set run period for particular data set you are processing
   //All data must be from the same run period
   processor.AddOption("HIPOPROCESSOR_RUNPERIOD","fall_2018");

Currently the RunPeriod for the data you are analysing must be specified by
hand as it is not included in the DST files, or other databases (in an easy to access way).

The contents of RunPeriodPass1.db look like,

::

   include $CHANSER/anadbs/TARGET_POSITION.db
   include $CHANSER/anadbs/RF_BUCKET_LENGTH.db

The contents of TARGET_POSITION.db looks like,

::
   
   PERIOD=fall_2018 SIM
   TARGET_POSITION
   0 0 -0.03

   PERIOD=fall_2018 EXP
   TARGET_POSITION
   0 0 -0.03

So this table will be 1 dimensional with 3 elements. When a class asks for
the **TARGET_POSITION** table it will be provided with these numbers for
fall_2018, simulation or data, depending what is in the data file. If the
run period is different the table will not be read and parameters not used.

If you wanted to make target position field dependent you may add an entry for **INBEND** and **OUTBEND** and add these to the **PERIOD=** string : **PERIOD=fall_2018 EXP OUTBEND**

If parameters are not loaded you may see some warnings like

::
   HipoData::FillRunInfo, need valid run period for AnaDB
   Warning in <HipoData::FillRunInfoExp()>:  no TARGET_POSITION db entry for _EXP
   ERROR AnaDBTable shape does not fit value vector 2 asked for but we  have 0



   
Including Parameter Lookup in Code
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you have a cut or correction (or other action) class that depends on some
analysis parameters, then to use AnaDB you must add an additional inheritance
for you class to CLAS12Base.

CLAS12Base provides the following methods to help in parameter configuration

::

    const clas12::ccdb_reader* CCDB()const noexcept {return GetCLAS12()->ccdb();}
    const clas12::rcdb_reader* RCDB()const noexcept {return GetCLAS12()->rcdb();}
    const clas12::qadb_reader* QADB()const noexcept {return GetCLAS12()->qadb();}

    const RunInfo* GetRunInfo()const {return _finalstate->GetRunInfo();}
    const EventInfo* GetEventInfo()const {return _finalstate->GetEventInfo();}
    const AnaDB& GetAnaDB() const {return GetRunInfo()->GetAnaDB();}
    
    virtual void ChangeRun(){};
    
 
    string RunPeriod()const {
      if(GetRunInfo())  return  GetRunInfo()->_runPeriod ;
      return "";
    }
    string RunPeriodType()const {
      if(GetRunInfo())  return  RunPeriod()+"_"+ GetRunInfo()->_dataType ;
      return "";
    }
    string RunPeriodField()const {
      if(GetRunInfo())  return  RunPeriod()+"_"+ GetRunInfo()->_fieldSetting;
      return "";
    }
    string RunPeriodTypeField()const {
      if(GetRunInfo())  return  RunPeriodType()+"_"+ GetRunInfo()->_fieldSetting;
      return "";
    }


And so gives you access to the other run databases (ccdb, rcdb, qadb); the run and event info; and the period information (period(fall_2018), type(EXP), Field(INBEND)).

You must then implement the ChangeRun() function in your class to check the
parameters whenever a run changes.
....


Defining Parameters in .db files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Top level file
