### RGA Macro:

All the procedures recommended in the RGA analysis note can be loaded with the `$CHANSER/macros/RGA.C`. This is done in your `Create_xxx.C` file by adding:

    RGA(FS.get());

and removing:

    FS->WriteToFile("eep_config.root");
    FS->Print();


### FiducialCutPCAL_uvw: 

A cut on the fiducial region of the PCAL U, V and W strips. The cut values recommended in the RG_A analysis note are 9cm for a loose cut, 14cm for a medium cut and 19cm for a tight cut applied to electrons and photons. This cut is instantiated in your `Create_xxx.C` file with: 

    ParticleCutsManager pcalFiducial{"PCALCutsLoose",0}; //0 as we do not want to apply these cuts to output file
    pcalFiducial.AddParticleCut("e-", new FiducialCutPCAL_uvw(9));
    FS->RegisterPostKineAction(pcalFiducial);
     
### DC Fiducial Cut: Robert

### Quality Assurance: Richard

### Electron Z-vertex cut: Richard
