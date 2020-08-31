FiducialCutPCAL_uvw: A cut on the fiducial region of the PCAL U, V and W strips. The cut values recommended in the RG_A analysis note are 9cm for a loose cut, 14cm for a medium cut and 19cm for a tight cut applied to electrons and photons. This cut is instantiated in your `Create_xxx.C` file with: 
`ParticleCutsManager pcm{"PCALCutsLoose",1};
 pcm.AddParticleCut("e-", new FiducialCutPCAL_uvw(9));`