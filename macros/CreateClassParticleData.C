void CreateClassParticleData(TString className){
 
  TString classes(gSystem->Getenv("CHANSER_CLASSES"));
  if(classes.Length()==0)
    Fatal("CreateClassParticleData"," destination directory must be set from enviroment variable $CHANSER_CLASSES");
  
  cout<<"chanser_skeleton : going to make ParticleData class "<<className<<" in "<<classes<<endl;

  SkeletonClass skel(className,"ParticleData");  
  skel.MakeCode();
  
}
