CLAS12 DataBases
****************

Databases are interfaced via clas12root::clas12reader. See the clas12root guthib page for more details.

ccdb
++++

rcdb
++++


qadb
++++

Example of running qadb with Run script

::

    HipoData hdata;
    hdata.AddFile("/hdd/jlab/clas12data/skim3_005644.hipo");
    ...
    ...
    hdata.Reader()->db()->qadb_requireOkForAsymmetry(true);
    hdata.Reader()->db()->qadb_requireGolden(true);
    hdata.Reader()->db()->qadb_addQARequirement("MarginalOutlier");
    hdata.Reader()->db()->qadb_addQARequirement("TotalOutlier");
    hdata.Reader()->applyQA();
  
