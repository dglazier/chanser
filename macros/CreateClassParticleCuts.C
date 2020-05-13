void CreateClassParticleCuts(TString className){
 
  TString classes(gSystem->Getenv("CHANSER_CLASSES"));
  if(classes.Length()==0)
    Fatal("CreateClassParticleCuts"," destination directory must be set from enviroment variable $CHANSER_CLASSES");
  
  cout<<"chanser_skeleton : going to make ParticleCuts class "<<className<<" in "<<classes<<endl;

  SkeletonClass skel(className,"ParticleCut");  
  skel.MakeCode();
  
}
