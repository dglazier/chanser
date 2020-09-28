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

  /*
   * DC Fiducial cuts for inbending hadrons
   * across three layers of the DC
   */
  ParticleCutsManager DCFidHadInbend{"DC_Fiducial_Hadron_Inbend", 0}
  DCFidHadInbend.AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi(6, -1));
  DCFidHadInbend.AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi(6, -1));
  DCFidHadInbend.AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi(18, -1));
  DCFidHadInbend.AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi(18, -1));
1DC1FidHadInbend.AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi(36, -1));
  DCFidHadInbend.AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi(36, -1));
  FS->RegisterPostKinAction(DCFidHadInbend);

  /*
   * DC Fiducial cuts for inbending electrons
   * across three layers of the DC
   */
  ParticleCutsManager DCFidElecInbend{"DC_Fiducial_Electron_Inbend", 0}
  DCFidElecInbend.AddParticleCut("e-", new DC_FiducialCut_XY(6, -1));
  DCFidElecInbend.AddParticleCut("e-", new DC_FiducialCut_XY(18, -1));
  DCFidElecInbend.AddParticleCut("e-", new DC_FiducialCut_XY(36, -1));
  FS->RegisterPostKinAction(DCFidElecInbend);

  /*
   * DC Fiducial cuts for outbending hadrons
   * across three layers of the DC
   */
  ParticleCutsManager DCFidHadOutbend{"DC_Fiducial_Hadron_Outbend", 0}
  DCFidHadOutbend.AddParticleCut("pi+", new DC_FiducialCut_XY(6, 1));
  DCFidHadOutbend.AddParticleCut("pi-", new DC_FiducialCut_XY(6, 1));
  DCFidHadOutbend.AddParticleCut("pi+", new DC_FiducialCut_XY(18, 1));
  DCFidHadOutbend.AddParticleCut("pi-", new DC_FiducialCut_XY(18, 1));
  DC1FidHadOutbend.AddParticleCut("pi+", new DC_FiducialCut_XY(36, 1));
  DCFidHadOutbend.AddParticleCut("pi-", new DC_FiducialCut_XY(36, 1));
  FS->RegisterPostKinAction(DCFidHadOutbend);

  /*
   * DC Fiducial cuts for outbending electrons
   * across three layers of the DC
   */
  ParticleCutsManager DCFidElecOutbend{"DC_Fiducial_Electron_Outbend", 0}
  DCFidElecOutbend.AddParticleCut("e-", new DC_FiducialCut_XY(6, 1));
  DCFidElecOutbend.AddParticleCut("e-", new DC_FiducialCut_XY(18, 1));
  DCFidElecOutbend.AddParticleCut("e-", new DC_FiducialCut_XY(36, 1));
  FS->RegisterPostKinAction(DCFidElecOutbend);

  /*
   * Differnce between hadron and electron vertex difference cut
   */
  ParticleCutsManager pcmVertexDiff{"ElHadVertexDiff", 0};
  pcmVertexDiff.AddParticleCut("pi-", new TwoParticleVertexCut("Electron", 20));
  pcmVertexDiff.AddParticleCut("pi+", new TwoParticleVertexCut("Electron", 20))
  FS->RegisterPostTopoAction(pcmVertexDiff);

  /*
   * Hadron chi2Pid cuts (standard)
   */
  ParticleCutsManager pcmChi2Pid{"HadronChi2Pid", 0};
  pcmChi2Pid.AddParticleCut("pi-", new HadronChi2PID(1));
  pcmChi2Pid.AddParticleCut("pi+", new HadronChi2PID(1));
  FS->RegisterPostTopoAction(pcmChi2Pid);

  /*
   * Hadron chi2Pid cuts (standard)
   */
  ParticleCutsManager pcmChi2Pid{"HadronChi2Pid", 0};
  pcmChi2Pid.AddParticleCut("pi-", new HadronChi2PID(2));
  pcmChi2Pid.AddParticleCut("pi+", new HadronChi2PID(2));
  FS->RegisterPostTopoAction(pcmChi2Pid);

  /*
   * Photon identification refinement 
   */
  ParticleCutsManager pcmPhotonRef{"PhotonRef", 0};
  pcmPhotonRef.AddParticleCut("gamma", new PhotonIDRefinementCut(0.1));
  FS->RegisterPostTopoAction(pcmPhotonRef);

  FS->WriteToFile("eep_config.root");
  FS->Print();
}
