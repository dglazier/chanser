### RGA Macro:

All the procedures recommended in the RGA analysis note can be loaded with the `$CHANSER/macros/RGA.C`. This is done in your `Create_xxx.C` file by adding:

    RGA(FS.get());

and removing from your `Create_xxx.C`:

    FS->WriteToFile("xxx_config.root");
    FS->Print();

In `$CHANSER/macros/RGA.C`, you must also change the line: 

    FS->WriteToFile("eep_config.root");

to the desired config name.

### FiducialCutPCAL_uvw: 

A cut on the fiducial region of the PCAL U, V and W strips. The cut values recommended in the RG_A analysis note are 9cm for a loose cut, 14cm for a medium cut and 19cm for a tight cut applied to electrons and photons. This cut is instantiated in your `Create_xxx.C` file with: 

    ParticleCutsManager pcalFiducial{"PCALCutsLoose",0}; //0 as we do not want to apply these cuts to output file
    pcalFiducial.AddParticleCut("e-", new FiducialCutPCAL_uvw(9));
    FS->RegisterPostKineAction(pcalFiducial);

### ZVertexCut:

A cut on the z-vertex position. The cut values for electrons recommended in the RG_A analysis note are -13cm and 12cm for in-bending runs and -18 cm and 10 cm for outbending runs. This cut is instantiated in your `Create_xxx.C` file with: 

    ParticleCutsManager zVertex{"ZVertexCut",0}; //0 as we do not want to apply these cuts to output file
    zVertex.AddParticleCut("e-", new ZVertexCut(-13,12));
    FS->RegisterPostKineAction(zVertex);
     
### DC Fiducial Cut: Robert

### Quality Assurance: Richard

