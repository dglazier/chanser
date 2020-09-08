void RGA(CLAS12FinalState* FS){
  
  /*
   * Loose (9cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  ParticleCutsManager epfcl{"ePCALFiducialLoose",0};
  epfcl.AddParticleCut("e-", new FiducialCutPCAL_uvw(9));
  FS->RegisterPostKinAction(epfcl);

  /*
   * Medium (14cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  ParticleCutsManager epfcm{"ePCALFiducialMedium",0};
  epfcm.AddParticleCut("e-", new FiducialCutPCAL_uvw(14));
  FS->RegisterPostKinAction(epfcm);

  /*
   * Tight (19cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  ParticleCutsManager epfct{"ePCALFiducialTight",0};
  epfct.AddParticleCut("e-", new FiducialCutPCAL_uvw(19));
  FS->RegisterPostKinAction(epfct);

  /*
   * Loose (9cm) cut on PCAL Fiducial region, applied to photons. 
   */
  ParticleCutsManager gpfcl{"gammaPCALFiducialLoose",0};
  gpfcl.AddParticleCut("gamma", new FiducialCutPCAL_uvw(9));
  FS->RegisterPostKinAction(gpfcl);

  /*
   * Medium (14cm) cut on PCAL Fiducial region, applied to photons. 
   */
  ParticleCutsManager gpfcm{"gammaPCALFiducialMedium",0};
  gpfcm.AddParticleCut("gamma", new FiducialCutPCAL_uvw(14));
  FS->RegisterPostKinAction(gpfcm);

  /*
   * Tight (19cm) cut on PCAL Fiducial region, applied to photons. 
   */
  ParticleCutsManager gpfct{"gammaPCALFiducialTight",0};
  gpfct.AddParticleCut("gamma", new FiducialCutPCAL_uvw(19));
  FS->RegisterPostKinAction(gpfct);

  /*
   * Recommended cut on the electron z-vertex position.
   */
  ParticleCutsManager zVertex{"ZVertex",0};
  zVertex.AddParticleCut("e-", new ZVertexCut(-13,12));
  FS->RegisterPostKinAction(zVertex);

  FS->WriteToFile("eep_config.root");
  FS->Print();
}
