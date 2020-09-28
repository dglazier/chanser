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
     
### DC Fiducial Cut (local theta/phi variables): Robert

In local theta/phi variables a four parameter fit is used to determine a cut on the fiducial volume in the DC. The RG-A analysis note recommends this cut for hadrons in the inbending data (electrons and all outbending data cuts are recommended to use the cut in local XY variables. 
To apply this cut to region R1, R2, R3 of the DC, one uses the parameter 6, 18, 36 in the cut, respectively. This cut is instantiated in your 'Create_xxx.C' file with:

    ParticleCutsManager DC_FidCut{"DCFidCutsTP_R3", 0};
    DC_FidCut.AddParticleCut("pi-", new DC_FiducialCut_ThetaPhi(36));
    FS->RegisterPostTopoAction(DC_FidCut);

### DC Fiducial Cut (local X/Y variables): Robert


In local XY coords a two parameter linear fit is used to cut the fiducial volume of the DC. The RG-A analysis note recommends using this cut on electrons for the inbending data and on all particles for the outbending data.
To apply this cut to region R1, R2, R3 of the DC, one uses the parameter 6, 18, 36 in the cut, respectively. The field parameter is called such that outbending == 1, inbending == -1 This cut is instantiated in your 'Create_xxx.C' file with:

    ParticleCutsManager DC_FidCut{"DCFidCutsXY_R3", 0};
    DC_FidCut.AddParticleCut("e-", new DC_FiducialCut_XY(36, 1));
    FS->RegisterPostTopoAction(DC_FidCut);


### Quality Assurance: Richard

### Electron PCAL Min Energy Dep Cut: Adam

### Electron Sampling fraction: Adam

### Vertex difference between electrons and hadrons cut: Robert

### chi2PID from event builder cut: Robert
