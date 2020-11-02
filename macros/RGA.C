void RGA(CLAS12FinalState* FS){
  /* in this convention we break c++11 protocols and use "new" to
     create objects on the heap, so they do not go out of scope
     at the end of this function and the finalstate can be written
     to file.
  */


  /*
   * Loose (9cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  auto fc_pcal_loose = new ParticleCutsManager{"RGA_PCALFiducialLoose",0};
  fc_pcal_loose->AddParticleCut("e-", new FiducialCutPCAL_uvw(9));
  fc_pcal_loose->AddParticleCut("gamma", new FiducialCutPCAL_uvw(9));
  FS->RegisterPostTopoAction(*fc_pcal_loose);
  /*
   * Medium (14cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  auto fc_pcal_med = new ParticleCutsManager{"RGA_PCALFiducialMedium",0};
  fc_pcal_med->AddParticleCut("e-", new FiducialCutPCAL_uvw(14));
  fc_pcal_med->AddParticleCut("gamma", new FiducialCutPCAL_uvw(14));
  FS->RegisterPostTopoAction(*fc_pcal_med);
  /*
   * Tight (19cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  auto fc_pcal_tight = new ParticleCutsManager{"RGA_PCALFiducialTight",0};
  fc_pcal_tight->AddParticleCut("e-", new FiducialCutPCAL_uvw(19));
  fc_pcal_tight->AddParticleCut("gamma", new FiducialCutPCAL_uvw(19));
  FS->RegisterPostTopoAction(*fc_pcal_tight);

  /*
   * Recommended cut on the electron z-vertex position.
   * -18, 10 for outbending 
   * -13, 12 for inbending
   */
  auto zVertex = new ParticleCutsManager {"RGA_elZVertex",0};
  zVertex->AddParticleCut("e-", new ZVertexCut());
  FS->RegisterPostTopoAction(*zVertex);
  /*
   * Difference between hadron and electron vertex difference cut
   */
  auto pcmVertexDiff = new ParticleCutsManager{"RGA_ElHadVertexDiff", 0};
  pcmVertexDiff->AddParticleCut("pi-", new TwoParticleVertexCut("Electron", 20));
  pcmVertexDiff->AddParticleCut("pi+", new TwoParticleVertexCut("Electron", 20));
  FS->RegisterPostTopoAction(*pcmVertexDiff);

  /*
   * Photon identification refinement 
   */
  auto pcmPhotonRef = new ParticleCutsManager {"RGA_PhotonBetaCut", 0};
  pcmPhotonRef->AddParticleCut("gamma", new PhotonIDRefinementCut(0.9,1.1));
  FS->RegisterPostTopoAction(*pcmPhotonRef);

  /*
   * Electron identification refinement 
   */
  auto pcmElRef = new ParticleCutsManager{"RGA_ElectronCut", 0};
  pcmElRef->AddParticleCut("e-", new Cut_RefineElectronID());
  FS->RegisterPostTopoAction(*pcmElRef);

  /*
   * Pion chi2Pid cuts (standard)
   */
  auto pcmChi2Pid=new ParticleCutsManager {"RGA_PionChi2Pid", 0};
  pcmChi2Pid->AddParticleCut("pi-", new HadronChi2PID(1,0.93));
  pcmChi2Pid->AddParticleCut("pi+", new HadronChi2PID(1,0.88));
  FS->RegisterPostTopoAction(*pcmChi2Pid);

  /*
   * Pion chi2Pid cuts (strict)
   */
  auto pcmChi2PidStrict= new ParticleCutsManager {"RGA_PionChi2PidStrict", 0};
  pcmChi2PidStrict->AddParticleCut("pi-", new HadronChi2PID(2,0.93));
  pcmChi2PidStrict->AddParticleCut("pi+", new HadronChi2PID(2,0.88));
  FS->RegisterPostTopoAction(*pcmChi2PidStrict);


  /*
   * DC Fiducial cuts,with ThetaPhi for e-
   * For layer 1,  of the DC (layer1 = 1 = DST bank 6 )
   */

  auto DCFidL1_ThPh=new ParticleCutsManager {"RGA_DC_Fiducial_ThPhL1", 0};
  DCFidL1_ThPh->AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi("pi+",1));
  DCFidL1_ThPh->AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi("pi-",1));
  DCFidL1_ThPh->AddParticleCut("K+", new DC_FiducialCut_ThetaPhi("K+",1));
  DCFidL1_ThPh->AddParticleCut("K-", new DC_FiducialCut_ThetaPhi("K-",1));
  DCFidL1_ThPh->AddParticleCut("e-", new DC_FiducialCut_ThetaPhi("e-", 1));
  FS->RegisterPostTopoAction(*DCFidL1_ThPh);

  /*
   * DC Fiducial cuts,  with ThetaPhi for e-
   * For layer 2,  of the DC (layer = 2 = DST bank 18 )
   */

  auto DCFidL2_ThPh=new ParticleCutsManager {"RGA_DC_Fiducial_ThPhL2", 0};
  DCFidL2_ThPh->AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi("pi+",2));
  DCFidL2_ThPh->AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi("pi-",2));
  DCFidL2_ThPh->AddParticleCut("K+", new DC_FiducialCut_ThetaPhi("K+",2));
  DCFidL2_ThPh->AddParticleCut("K-", new DC_FiducialCut_ThetaPhi("K-",2));
  DCFidL2_ThPh->AddParticleCut("e-", new DC_FiducialCut_ThetaPhi("e-", 2));
  FS->RegisterPostTopoAction(*DCFidL2_ThPh);

  /*
   * DC Fiducial cuts, with ThetaPhi for e-
   * For layer 3,  of the DC (layer = 3 = DST bank 36 )
   */

  auto DCFidL3_ThPh=new ParticleCutsManager {"RGA_DC_Fiducial_ThPhL3", 0};
  DCFidL3_ThPh->AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi("pi+",3));
  DCFidL3_ThPh->AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi("pi-",3));
  DCFidL3_ThPh->AddParticleCut("K+", new DC_FiducialCut_ThetaPhi("K+",3));
  DCFidL3_ThPh->AddParticleCut("K-", new DC_FiducialCut_ThetaPhi("K-",3));
  DCFidL3_ThPh->AddParticleCut("e-", new DC_FiducialCut_ThetaPhi("e-", 3));
  FS->RegisterPostTopoAction(*DCFidL3_ThPh);

  /*
   * DC Fiducial cuts, including XY for e-
   * For layer 1,  of the DC (layer1 = 1 = DST bank 6 )
   */

  auto DCFidL1_XY=new ParticleCutsManager {"RGA_DC_Fiducial_XYL1", 0};
  DCFidL1_XY->AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi("pi+",1));
  DCFidL1_XY->AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi("pi-",1));
  DCFidL1_XY->AddParticleCut("K+", new DC_FiducialCut_ThetaPhi("K+",1));
  DCFidL1_XY->AddParticleCut("K-", new DC_FiducialCut_ThetaPhi("K-",1));
  DCFidL1_XY->AddParticleCut("e-", new DC_FiducialCut_XY("e-", 1));
  FS->RegisterPostTopoAction(*DCFidL1_XY);

  /*
   * DC Fiducial cuts, including XY for e-
   * For layer 2,  of the DC (layer = 2 = DST bank 18 )
   */

  auto DCFidL2_XY=new ParticleCutsManager {"RGA_DC_Fiducial_XYL2", 0};
  DCFidL2_XY->AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi("pi+",2));
  DCFidL2_XY->AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi("pi-",2));
  DCFidL2_XY->AddParticleCut("K+", new DC_FiducialCut_ThetaPhi("K+",2));
  DCFidL2_XY->AddParticleCut("K-", new DC_FiducialCut_ThetaPhi("K-",2));
  DCFidL2_XY->AddParticleCut("e-", new DC_FiducialCut_XY("e-", 2));
  FS->RegisterPostTopoAction(*DCFidL2_XY);

  /*
   * DC Fiducial cuts, including XY for e-
   * For layer 3,  of the DC (layer = 3 = DST bank 36 )
   */

  auto DCFidL3_XY=new ParticleCutsManager {"RGA_DC_Fiducial_XYL3", 0};
  DCFidL3_XY->AddParticleCut("pi+", new DC_FiducialCut_ThetaPhi("pi+",3));
  DCFidL3_XY->AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi("pi-",3));
  DCFidL3_XY->AddParticleCut("K+", new DC_FiducialCut_ThetaPhi("K+",3));
  DCFidL3_XY->AddParticleCut("K-", new DC_FiducialCut_ThetaPhi("K-",3));
  DCFidL3_XY->AddParticleCut("e-", new DC_FiducialCut_XY("e-", 3));
  FS->RegisterPostTopoAction(*DCFidL3_XY);


}
