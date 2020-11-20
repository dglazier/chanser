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
  fc_pcal_loose->AddParticleCut("e-", new FiducialCut_PCAL_uvw(9));
  fc_pcal_loose->AddParticleCut("gamma", new FiducialCut_PCAL_uvw(9));
  FS->RegisterPostTopoAction(*fc_pcal_loose);
  /*
   * Medium (14cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  auto fc_pcal_med = new ParticleCutsManager{"RGA_PCALFiducialMedium",0};
  fc_pcal_med->AddParticleCut("e-", new FiducialCut_PCAL_uvw(14));
  fc_pcal_med->AddParticleCut("gamma", new FiducialCut_PCAL_uvw(14));
  FS->RegisterPostTopoAction(*fc_pcal_med);
  /*
   * Tight (19cm) cut on PCAL Fiducial region, applied to electrons. 
   */
  auto fc_pcal_tight = new ParticleCutsManager{"RGA_PCALFiducialTight",0};
  fc_pcal_tight->AddParticleCut("e-", new FiducialCut_PCAL_uvw(19));
  fc_pcal_tight->AddParticleCut("gamma", new FiducialCut_PCAL_uvw(19));
  FS->RegisterPostTopoAction(*fc_pcal_tight);

  /*
   * Recommended cut on the electron z-vertex position.
   * -18, 10 for outbending 
   * -13, 12 for inbending
   */
  auto zVertex = new ParticleCutsManager {"RGA_elZVertex",0};
  zVertex->AddParticleCut("e-", new Cut_ZVertex());
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
  pcmPhotonRef->AddParticleCut("gamma", new Cut_RefinePhotonID(0.9,1.1));
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
  pcmChi2Pid->AddParticleCut("pi-", new Cut_PionChi2Pid(1,0.93));
  pcmChi2Pid->AddParticleCut("pi+", new Cut_PionChi2Pid(1,0.88));
  FS->RegisterPostTopoAction(*pcmChi2Pid);

  /*
   * Pion chi2Pid cuts (strict)
   */
  auto pcmChi2PidStrict= new ParticleCutsManager {"RGA_PionChi2PidStrict", 0};
  pcmChi2PidStrict->AddParticleCut("pi-", new Cut_PionChi2Pid(2,0.93));
  pcmChi2PidStrict->AddParticleCut("pi+", new Cut_PionChi2Pid(2,0.88));
  FS->RegisterPostTopoAction(*pcmChi2PidStrict);


  /*
   * DC Fiducial cuts in local XY co-ordinates
   */
 
  auto DC_Fiducial_XY=new ParticleCutsManager {"RGA_DC_Fiducial_XY", 0};
  DC_Fiducial_XY->AddParticleCut("pi+", new FiducialCut_DC_XY("pi+"));
  DC_Fiducial_XY->AddParticleCut("pi-", new FiducialCut_DC_XY("pi-"));
  DC_Fiducial_XY->AddParticleCut("K+", new FiducialCut_DC_XY("K+"));
  DC_Fiducial_XY->AddParticleCut("K-", new FiducialCut_DC_XY("K-"));
  DC_Fiducial_XY->AddParticleCut("e-", new FiducialCut_DC_XY("e-"));
  FS->RegisterPostTopoAction(*DC_Fiducial_XY);


  /*
   * DC Fiducial cuts in local Theta,Phi co-ordinates
   */

  auto DC_Fiducial_TP=new ParticleCutsManager {"RGA_DC_Fiducial_TP", 0};
  DC_Fiducial_TP->AddParticleCut("pi+", new FiducialCut_DC_ThetaPhi("pi+"));
  DC_Fiducial_TP->AddParticleCut("pi-", new FiducialCut_DC_ThetaPhi("pi-"));
  DC_Fiducial_TP->AddParticleCut("K+", new FiducialCut_DC_ThetaPhi("K+"));
  DC_Fiducial_TP->AddParticleCut("K-", new FiducialCut_DC_ThetaPhi("K-"));
  DC_Fiducial_TP->AddParticleCut("e-", new FiducialCut_DC_ThetaPhi("e-"));
  FS->RegisterPostTopoAction(*DC_Fiducial_TP);
  

}
