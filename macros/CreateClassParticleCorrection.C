void CreateClassParticleCorrection(TString className){
 
  TString classes(gSystem->Getenv("CHANSER_CLASSES"));
  if(classes.Length()==0)
    Fatal("CreateClassParticleCorrection"," destination directory must be set from enviroment variable $CHANSER_CLASSES");
  
  cout<<"chanser_skeleton : going to make ParticleCorrection class "<<className<<" in "<<classes<<endl;

  SkeletonClass skel(className,"ParticleCorrection"); //for templateParticleCorrectoin.h  
  skel.MakeCode();
  
}
