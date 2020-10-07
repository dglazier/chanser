void RGA(CLAS12FinalState* FS){
  
  /*
   * Loose (9cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  ParticleCutsManager epfcl{"ePCALFiducialLoose",0};
  epfcl.AddParticleCut("e-", new FiducialCutPCAL_uvw(9));
  FS->RegisterPostTopoAction(epfcl);

  /*
   * Medium (14cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  ParticleCutsManager epfcm{"ePCALFiducialMedium",0};
  epfcm.AddParticleCut("e-", new FiducialCutPCAL_uvw(14));
  FS->RegisterPostTopoAction(epfcm);

  /*
   * Tight (19cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  ParticleCutsManager epfct{"ePCALFiducialTight",0};
  epfct.AddParticleCut("e-", new FiducialCutPCAL_uvw(19));
  FS->RegisterPostTopoAction(epfct);

  /*
   * Loose (9cm) cut on PCAL Fiducial region, applied to photons. 
   */
  ParticleCutsManager gpfcl{"gammaPCALFiducialLoose",0};
  gpfcl.AddParticleCut("gamma", new FiducialCutPCAL_uvw(9));
  FS->RegisterPostTopoAction(gpfcl);

  /*
   * Medium (14cm) cut on PCAL Fiducial region, applied to photons. 
   */
  ParticleCutsManager gpfcm{"gammaPCALFiducialMedium",0};
  gpfcm.AddParticleCut("gamma", new FiducialCutPCAL_uvw(14));
  FS->RegisterPostTopoAction(gpfcm);

  /*
   * Tight (19cm) cut on PCAL Fiducial region, applied to photons. 
   */
  ParticleCutsManager gpfct{"gammaPCALFiducialTight",0};
  gpfct.AddParticleCut("gamma", new FiducialCutPCAL_uvw(19));
  FS->RegisterPostTopoAction(gpfct);

  /*
   * Recommended cut on the electron z-vertex position.
   * -18, 10 for outbending 
   * -13, 12 for inbending
   */
  ParticleCutsManager zVertex{"ZVertex",0};
  zVertex.AddParticleCut("e-", new ZVertexCut(-13,12));
  FS->RegisterPostTopoAction(zVertex);

  /*
   * DC Fiducial cuts for inbending hadrons
   * across three layers of the DC (layer1)
   */
  ParticleCutsManager DCFidHadInbendL1{"DC_Fiducial_Hadron_Inbend_L1", 0};
  DCFidHadInbendL1.AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi(6, -1));
  DCFidHadInbendL1.AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi(6, -1));
  FS->RegisterPostTopoAction(DCFidHadInbendL1);

  /*
   * DC Fiducial cuts for inbending hadrons
   * across three layers of the DC (layer2)
   */
  ParticleCutsManager DCFidHadInbendL2{"DC_Fiducial_Hadron_Inbend_L2", 0};
  DCFidHadInbendL2.AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi(18, -1));
  DCFidHadInbendL2.AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi(18, -1));
  FS->RegisterPostTopoAction(DCFidHadInbendL2);

  /*
   * DC Fiducial cuts for inbending hadrons
   * across three layers of the DC (layer3)
   */
  ParticleCutsManager DCFidHadInbendL3{"DC_Fiducial_Hadron_Inbend_L3", 0};
  DCFidHadInbendL3.AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi(36, -1));
  DCFidHadInbendL3.AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi(36, -1));
  FS->RegisterPostTopoAction(DCFidHadInbendL3);
  
  /*
   * DC Fiducial cuts for inbending electrons
   * across three layers of the DC (layer1)
   */
  ParticleCutsManager DCFidElecInbendL1{"DC_Fiducial_Electron_InbendL1", 0};
  DCFidElecInbendL1.AddParticleCut("e-", new DC_FiducialCut_XY(6, -1));
  FS->RegisterPostTopoAction(DCFidElecInbendL1);

  /*
   * DC Fiducial cuts for inbending electrons
   * across three layers of the DC (layer2)
   */
  ParticleCutsManager DCFidElecInbendL2{"DC_Fiducial_Electron_InbendL2", 0};
  DCFidElecInbendL2.AddParticleCut("e-", new DC_FiducialCut_XY(18, -1));
  FS->RegisterPostTopoAction(DCFidElecInbendL2);

  /*
   * DC Fiducial cuts for inbending electrons
   * across three layers of the DC (layer3)
   */
  ParticleCutsManager DCFidElecInbendL3{"DC_Fiducial_Electron_InbendL3", 0};
  DCFidElecInbendL3.AddParticleCut("e-", new DC_FiducialCut_XY(36, -1));
  FS->RegisterPostTopoAction(DCFidElecInbendL3);
  

  /*
   * DC Fiducial cuts for outbending hadrons
   * across three layers of the DC (layer1)
   */
  ParticleCutsManager DCFidHadOutbendL1{"DC_Fiducial_Hadron_Outbend_L1", 0};
  DCFidHadOutbendL1.AddParticleCut("pi+", new DC_FiducialCut_XY(6, 1));
  DCFidHadOutbendL1.AddParticleCut("pi-", new DC_FiducialCut_XY(6, 1));
  FS->RegisterPostTopoAction(DCFidHadOutbendL1);

  /*
   * DC Fiducial cuts for outbending hadrons
   * across three layers of the DC (layer2)
   */
  ParticleCutsManager DCFidHadOutbendL2{"DC_Fiducial_Hadron_Outbend_L2", 0};
  DCFidHadOutbendL2.AddParticleCut("pi+", new DC_FiducialCut_XY(18, 1));
  DCFidHadOutbendL2.AddParticleCut("pi-", new DC_FiducialCut_XY(18, 1));
  FS->RegisterPostTopoAction(DCFidHadOutbendL2);

  /*
   * DC Fiducial cuts for outbending hadrons
   * across three layers of the DC (layer3)
   */
  ParticleCutsManager DCFidHadOutbendL3{"DC_Fiducial_Hadron_Outbend_L3", 0};
  DCFidHadOutbendL3.AddParticleCut("pi+", new DC_FiducialCut_XY(36, 1));
  DCFidHadOutbendL3.AddParticleCut("pi-", new DC_FiducialCut_XY(36, 1));
  FS->RegisterPostTopoAction(DCFidHadOutbendL3);

  /*
   * DC Fiducial cuts for outbending electrons
   * across three layers of the DC (layer1)
   */
  ParticleCutsManager DCFidElecOutbendL1{"DC_Fiducial_Electron_Outbend_L1", 0};
  DCFidElecOutbendL1.AddParticleCut("e-", new DC_FiducialCut_XY(6, 1));
  FS->RegisterPostTopoAction(DCFidElecOutbendL1);

  /*
   * DC Fiducial cuts for outbending electrons
   * across three layers of the DC (layer2)
   */
  ParticleCutsManager DCFidElecOutbendL2{"DC_Fiducial_Electron_Outbend_L2", 0};
  DCFidElecOutbendL2.AddParticleCut("e-", new DC_FiducialCut_XY(18, 1));
  FS->RegisterPostTopoAction(DCFidElecOutbendL2);

  /*
   * DC Fiducial cuts for outbending electrons
   * across three layers of the DC (layer3)
   */
  ParticleCutsManager DCFidElecOutbendL3{"DC_Fiducial_Electron_Outbend_L3", 0};
  DCFidElecOutbendL3.AddParticleCut("e-", new DC_FiducialCut_XY(36, 1));
  FS->RegisterPostTopoAction(DCFidElecOutbendL3);

  /*
   * Difference between hadron and electron vertex difference cut
   */
  ParticleCutsManager pcmVertexDiff{"ElHadVertexDiff", 0};
  pcmVertexDiff.AddParticleCut("pi-", new TwoParticleVertexCut("Electron", 20));
  pcmVertexDiff.AddParticleCut("pi+", new TwoParticleVertexCut("Electron", 20));
  FS->RegisterPostTopoAction(pcmVertexDiff);

  /*
   * Hadron chi2Pid cuts (standard)
   */
  ParticleCutsManager pcmChi2Pid{"HadronChi2Pid", 0};
  pcmChi2Pid.AddParticleCut("pi-", new HadronChi2PID(1));
  pcmChi2Pid.AddParticleCut("pi+", new HadronChi2PID(1));
  FS->RegisterPostTopoAction(pcmChi2Pid);

  /*
   * Hadron chi2Pid cuts (strict)
   */
  ParticleCutsManager pcmChi2PidStrict{"HadronChi2PidStrict", 0};
  pcmChi2PidStrict.AddParticleCut("pi-", new HadronChi2PID(2));
  pcmChi2PidStrict.AddParticleCut("pi+", new HadronChi2PID(2));
  FS->RegisterPostTopoAction(pcmChi2PidStrict);

  /*
   * Photon identification refinement 
   */
  ParticleCutsManager pcmPhotonRef{"PhotonRef", 0};
  pcmPhotonRef.AddParticleCut("gamma", new PhotonIDRefinementCut(0.1));
  FS->RegisterPostTopoAction(pcmPhotonRef);

  FS->WriteToFile("ALLALL.root");
  FS->Print();
}
