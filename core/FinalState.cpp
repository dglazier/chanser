#include "FinalState.h"
#include "AutoIterUtils.h"
#include <TFile.h>
#include <TSystem.h>
#include <memory>

namespace chanser{
    
   
  /////////////////////////////////////////////////////////////////
  FinalState::~FinalState(){
    
    if(_ownsActions){ //if loaded from file we will own action pointers
      //this happens anytime the default constructro is used 
      for(auto pt : _preTopoAction) {delete pt; pt=nullptr;}
      _preTopoAction.clear();
	
      for(auto pt : _postTopoAction) {delete pt; pt=nullptr;}
      _postTopoAction.clear();
	
      for(auto pt : _postKinAction) {delete pt; pt=nullptr;}
      _postKinAction.clear();
    }
      
  }
    
  //////////////////////////////////////////////////////////////////
  ///Write to ROOT file
  void FinalState::WriteToFile(TString filename){
    auto file=TFile::Open(filename,"recreate");
     
    this->Write(GetName());

    //Archive all code needed to reproduce this class
    Archive::ArchiveIt(GetName(),file);
      
    delete file;

  }
  //////////////////////////////////////////////////////////////////
  //output particles
  void FinalState::ShowParticles()const{
    std::cout<<GetName()<<" ShowParticles "<<endl;
    for(auto& cp: _pconfigs){
      std::cout<<cp.GetName()<<" "<<cp.PDG()<<std::endl;
    }

  }
 
  //////////////////////////////////////////////////////////////////
  void  FinalState::AddTopology(const TString names,const VoidFuncs funcE){
    _currTopo=_topoMan.AddTopology(names,funcE);
  }
    
  void  FinalState::SetTopologies(){
    for(auto& name:_usedTopos){
      _currTopo=_topoMan.AddTopology(name,_doToTopo[name]);
    }
   
  }
  void  FinalState::ConfigureIters(Topology *tt){
   
   _currTopo=tt;
    _currIter=nullptr;
    AutoIter();
    _currTopo->Iter().ConfigureIters();
    _itersConfigured++;
  }
 
  void FinalState::Init(const TString& baseDir){
    
    auto outdir=baseDir+GetUSER()+'/'; //Add user name directory
    gSystem->Exec(Form("mkdir -p %s",outdir.Data()));
    Info("FinalState::Init",Form("Using %s ",InputFileName().Data()),"");
    
    //add final state configuration name directory
    _finalDirectory= outdir+GetName()+'_'+gSystem->BaseName(InputFileName());
    _outputDir=_finalDirectory+"__"+WorkerName()+'/';
    
    gSystem->Exec(Form("mkdir -p %s",_finalDirectory.Data()));
    gSystem->Exec(Form("mkdir -p %s",_outputDir.Data()));
    
    //Now contruct output event
    //this can include truth data if simulation
    PrepareOutEvent();
    _outEvent.Init(_outputDir);
    if(_outEvent.FinalTree()){
      //add branches with FinalState datamembers
      auto tree=_outEvent.FinalTree();
      tree->Branch("Topo",&_currTopoID,"Topo/I");
      tree->Branch("NPerm",&_nPerm,"NPerm/I");
      _listOfOutTrees.push_back(_outEvent.FinalTree());
    }
    if(_outEvent.FinalHipo()){
      //add banks with FinalState datamembers
      auto tree=_outEvent.FinalHipo();
      tree->linkItemFunc("FSInfo","Topo/I",&_currTopoID);
      tree->linkItemFunc("FSInfo","NPerm/I",&_nPerm);
      tree->open();//hipo writer must be opened after all banks defined!
 
    }

    for(auto& topo: _topoMan.Topos())
      ConfigureIters(&topo);

    for(auto pt : _preTopoAction) {
      //Let action manager link to this final state
      pt->Configure(this);	
    }
   for(auto pt : _postTopoAction) {
      //Let action manager link to this final state
      pt->Configure(this);	
    }
    for(auto pt : _postKinAction) {
      //Let action manager link to this final state
      pt->Configure(this);	
    }

    _currTopo=nullptr;
      
  }
  ///////////////////////////////////////////////////////////////
  void  FinalState::PrepareOutEvent(){
      _outEvent.TakeReal(TreeDataFactory());
      SetOutEvent(_outEvent.GetReal());
      if(HasTruth()&&_isGenerated==kFALSE)
	//only take truth if we have a sim file and not analysing just generated
	_outEvent.TakeTruth(TreeDataFactory());
  }
  
  //////////////////////////////////////////////////////////////////
  ///Used to configure iterators and generated particles
  void FinalState::AddParticle(TString name,BaseParticle* part,Bool_t AddToFinal,Int_t genID){
    auto pc= ParticleConfig(name,part,genID,_topoMan.ParticleID(part->PDG()));
    _pconfigs.push_back(std::move(pc));

    if(AddToFinal) _final.push_back(part);
  }
  /////////////////////////////////////////////////////////////////////
  ///Add a child particle to the ParticleConfig of parent
  void FinalState::ConfigParent(BaseParticle* parent,BaseParticle* child){
    UInt_t i=0;
    ParticleConfig* parentConfig=nullptr;
    for(i=0;i<_pconfigs.size();i++){
      if(_pconfigs[i].Particle()==parent){
	parentConfig=&_pconfigs[i];
	parentConfig->AddChild(child);
      }
    }
    if(!parentConfig)cout<<"WARNING HS::FinalState::ConfigParent parent not found, has it been configured yet?"<<endl;
    for(i=0;i<_pconfigs.size();i++)
      if(_pconfigs[i].Particle()==child)
	_pconfigs[i].SetParent(parentConfig);

    if(i!=_pconfigs.size())cout<<"WARNING HS::FinalState::ConfigParent child not found, has it been configured yet? "<<i<<" "<<_pconfigs.size()<<endl;

  
  }

  /////////////////////////////////////////////////////////////
  ///Return final state particle with this name
  BaseParticle* FinalState::GetParticle(const TString name) const{
    for(auto const& part: _pconfigs){
      if(part.GetName()==name) return part.Particle();
    }
    return nullptr;
  }
  /////////////////////////////////////////////////////////////
  ///Return configured particles with this name
  ///used to work to give to topology
  ParticleConfig FinalState::GetParticleConfig(const TString name) const{
    for(auto& part: _pconfigs){
      if(part.GetName()==name) return part;
    }
    return ParticleConfig();
  }
  ////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////
  ///Analyse one event 
  void FinalState::ProcessEvent(){

    InitEvent();
    if(_hasTruth) InitTruth();

    if(_isGenerated){//Just analysing generated events
      FSTruthProcess(); //If only analysing generated events
     _nEvents++;
     return; 
    }

    auto validTopos =_topoMan.ValidTopos();
   
    for(auto* topo : validTopos){
      _currTopo=topo;
      _currTopoID=_currTopo->ID();
      //First combination
      auto singleCombination=(!_currTopo->FirstParticles()); //true if no other combination. Calls ParticleIter::FirstCombitorial0
      FSProcess();
   
      if(singleCombination) continue;
      //Now iterate over others
      auto piter=_currTopo->Iter();
      //cout<<"FinalState::ProcessEvent() process another combi going in...."<<endl<<endl<<endl; 
      while(piter.NextCombitorial0()){
	FSProcess();
      }
    }
      
    _TotPerm+=_nPerm;
    _nEvents++;
  }
  //////////////////////////////////////////////////////
  void FinalState::FSTruthProcess(){
    _rejectEvent=kFALSE;//in principle will save this combi
    UseTruth();
    Kinematics();
    if(_rejectEvent) return;
    UserProcess();

  }
  ///////////////////////////////////////////////////////
  void FinalState::FSProcess(){
    
    _rejectEvent=kFALSE;//in principle will save this combi
    //CheckCombitorial();
    
    _nPerm++;

    auto tid=_currTopo->ID();
      
   //Check for any user loaded post topo actions
    //e.g, making cuts on particles
    for(const auto& act : _preTopoAction)
      if(act->Execute(tid)==kFALSE) {
	_rejectEvent=kTRUE;
	return;
      }

    _currTopo->Exec();
      
    //Check for any user loaded post topo actions
    //e.g, making cuts on particles
    for(const auto& act : _postTopoAction)
      if(act->Execute(tid)==kFALSE) {
	_rejectEvent=kTRUE;
	return;
      }
      
    if(_rejectEvent) return;
     
       
    Kinematics();
    if(_rejectEvent) return;
    
    if(_hasTruth)TruthKinematics();
    
    //Check for any user loaded post topo actions
    //e.g, making cuts on particles
    for(const auto& act : _postKinAction)
      if(act->Execute(tid)==kFALSE) {
	_rejectEvent=kTRUE;
	return;
      }
    if(_rejectEvent) return;
    
    //Finally call UserProcess, this should include tree filling etc.
    UserProcess();
    if(_rejectEvent) return;

  }
  void FinalState::TruthKinematics(){
      //fill truth tree data
    SetOutEvent(_outEvent.GetTruth( ));
    UseTruth();
    Kinematics();
 
    //revert back to real tree data
    NotTruth();
    SetOutEvent(_outEvent.GetReal( ));
  }
  
  ///////////////////////////////////////////////////////////////
  void FinalState::InitTruth(){
    if(_truth->empty()) return;
    for(UInt_t ip=0;ip<_pconfigs.size();ip++){
      auto place =_pconfigs[ip].GenID();
      if(place>-1){ //only check particles assigned via ConfigParti
	_pconfigs[ip].Particle()->SetTruth(_truth->at(place));
      }
    }
  }
  ///////////////////////////////////////////////////////////////
  void FinalState::UseTruth(){
    if(_truth->empty()) return;
    for(UInt_t ip=0;ip<_pconfigs.size();ip++){
      auto place =_pconfigs[ip].GenID();
      if(place>-1){ //only check particles assigned via ConfigParti
	_pconfigs[ip].Particle()->UseTruth();
      }
    }
  }
 ///////////////////////////////////////////////////////////////
  void FinalState::NotTruth(){
    for(UInt_t ip=0;ip<_pconfigs.size();ip++){
      auto place =_pconfigs[ip].GenID();
      if(place>-1){ //only check particles assigned via ConfigParti
	_pconfigs[ip].Particle()->NotTruth();
      }
    }
  }

  //////////////////////////////////////////////////////////////
  ///Print permuations to screen for debugging
  void FinalState::CheckCombitorial(){
    cout<<" Printing Perm "<<_nPerm<<" of topology "<<endl;
    for(UInt_t i=0;i<_pconfigs.size();i++){
      auto *part=_pconfigs[i].Particle();
      //if(IsMissing(part))continue;
      cout<<" PDG("<<part->PDG()<<")Th("<<part->P4p()->Theta()<<")E( "<<part->P4p()->E()<<")  ";
    }
    cout<<endl;
  }
  /////////////////////////////////////////////////////////////
  ///Return configured particles with this pdg
  ///used to work out how many identical particles to iterate over
  vector<ParticleConfig* > FinalState::HowManyParticles(Int_t pdg){
    Int_t np=0;
    vector<ParticleConfig* > configs;
    for(UInt_t i=0;i<_pconfigs.size();i++){
      if(_pconfigs[i].PDG()==pdg) configs.push_back(&_pconfigs[i]);
    }
    return configs;
  }
  ////////////////////////////////////////////////////////////////
  ///Interface to ParticleIter
  ///Create particle iterator for a particticular species
  ///Given by the particle vector parts (e.g. fVecMinus)
  ParticleIter* FinalState::CreateParticleIter(vector<BaseParticle*> *parts,Int_t Nsel){
    ParticleIter *diter=nullptr;
    if(!_currIter){//get base iterator from topology
      diter=&_currTopo->Iter();
      _currIter=diter;
    }
    else{ //create new iterators after the base
      diter= new ParticleIter();
      _currIter->SetNextInnerIter(diter);

    }
    diter->SetParticles(parts);
    diter->SetNSel(Nsel);

    diter->Print(1);
    return diter;
  }
  //////////////////////////////////////////////////////////////////
  ///Automatically generate iterator based on topology and particle configs
  void FinalState::AutoIter(){

    //pid => can be pdg code or charge, depending on what the user want to use for particle identification
    auto pids = whichParticleConfigPIDs(_currTopo->GetParticleConfigs()); //returns vector of pdgs
    // cout<<"FinalState::AutoIter() "<<pids.size()<<endl;
    for(const auto& pid:pids){
       
      //   Create the particle iterator for species pid
      //   This just selects from the species vector for each
      //   combitorial event
      auto piter = InnerParticle(pid);
      //   Create particular iterator for this species
      //   This acts on the particles already selected
      //   for this combitorial event
      InnerSelect(piter,pid);
      
    }
  }
    

  //////////////////////////////////////////////////////////////////
  ///Create the particle iterator for species pid
  ParticleIter*  FinalState::InnerParticle(Int_t pid){
     //Get particles with this Pid
    auto configs_pid=filterDetParticleConfigsByPid(_currTopo->GetParticleConfigs(),pid);
    //create a Particle Iterator for this species (InnerParticle)
    //need to give the vector of particles for this species
    //and the number to select from it
    ParticleIter* diter0=CreateParticleIter(_eventParts->GetParticleVector(pid),configs_pid.size());
  
    //get the pointers to the particles of this species
    vector<BaseParticle* > species_parts;
    
    for(const auto& particle_conf:configs_pid){
      species_parts.push_back(particle_conf.Particle());
    }
    
    //Connect actual FinalState particles to iterator
    //These particle will be updated for each combitorial
    return AddSelectToSelected(diter0,species_parts.size(),&species_parts);
    
  }
  void FinalState::InnerSelect(ParticleIter* recursiter,Int_t pid){
    
     //get particle configs for this pid
    auto configs_pid=filterDetParticleConfigsByPid(_currTopo->GetParticleConfigs(),pid);
    
    //how many final state particles with pid but different PDG
    auto pdgs = whichParticleConfigPDGs(configs_pid); //returns vector of pdgs

    //we need to select particles belonging to each pdg
    //get the pointers to the particles of this species
    auto Nleft=configs_pid.size();
   
    auto loopIter=recursiter;
    for(const auto& pdg:pdgs){
      if(Nleft<=0) cout<<"None left but still PDGs"<<Nleft<<" "<<pdg<< " "<<pdgs.size()<<endl;
      //get particles with this pdg
      auto configs_pdg=filterDetParticleConfigsByPdg(configs_pid,pdg);
      //get a vector of their base particle pointers
      vector<BaseParticle* > pdg_parts;
      for(const auto& particle_conf : configs_pdg){
	pdg_parts.push_back(particle_conf.Particle());
      }
      auto Npdg=pdg_parts.size(); //number to select
      
      Nleft-=Npdg; //Number remaining
      //select particles with this pdg
      auto selIter=AddSelectToSelected(loopIter,Npdg,&pdg_parts);
      if(Nleft>0){
	//get the remaining particles
	//this just assigns the remaining BaseParticle ptrs
	auto remIter=AddSelectToRemainder(selIter,Nleft);
	loopIter=remIter;//left over pdgs for next loop iteration
     }

      
      //get particles of this pdg which have parents
      auto parent_names = whichParticleConfigParents(configs_pdg); //returns vector of pdgs
     
      auto parLoopIter=selIter;
      for(auto& name:parent_names){

      	//get particles with this pdg and this parent (generally should be 1!)
      	auto configs_parent=filterDetParticleConfigsByParent(configs_pdg,name);
      	//get a vector of their base particle pointers
      	vector<BaseParticle* > child_parts;
      	for(const auto& particle_conf : configs_parent){
      	  child_parts.push_back(particle_conf.Particle());
      	}
      	auto Nchild=child_parts.size();
      	auto Nleft_after_parent=Npdg-Nchild;
  
    	//select particle for this parent pdg from the particles with this pdg
      	auto parIter=AddSelectToSelected(parLoopIter,Nchild,&child_parts);
      	if(Nleft_after_parent>0){
      	  //get the remaining particles
      	  //this just assigns the remaining BaseParticle ptrs
      	  auto premIter=AddSelectToRemainder(parIter,Nleft_after_parent);
	  parLoopIter=premIter; //left over childern for next parent (if there is one)
      	}
      }
      
    }
     
    
  }  
  ////////////////////////////////////////////////////////////
  void FinalState::EndAndWrite(){
    // _finalTree.reset();
    // if(_finalHipo.get())_finalHipo->close();
    // _finalHipo.reset();
    _outEvent.Finish();
    _mergeLists.push_back(_outEvent.UniqueFinalTreeList());
      
    //end any action managers, e.g save trees
    for(auto pt : _preTopoAction) {
      pt->End();	
    }
    //end any action managers, e.g save trees
    for(auto pt : _postTopoAction) {
      pt->End();	
    }
      
    //end any action managers, e.g save trees
    for(auto pt : _postKinAction) {
      pt->End();	
    }
  }
  /////////////////////////////////////////////////////////////
  ///Call this if you have an output file which will need
  ///to be merged when using proof
  void FinalState::AddMergeList(TString name, TString filename){
      tlist_uptr li{new TList()};
      li->SetName(FinalDirectory()+name);
      li->Add(new TObjString(filename));
      _mergeLists.push_back(std::move(li));
  }

  //////////////////////////////////////////////////////////////
   void FinalState::Print(Option_t* option)const{
     Info("FinalState::Print",Form("From file %s ",InputFileName().Data()),"");
     ShowParticles();
     ShowTopologies();
     //_currTopo->Iter().Print(9);
     //_outEvent.Finish();
     
     //end any action managers, e.g save trees
     Info("FinalState::Print","PreTopoActions :");
     for(auto pt : _preTopoAction) {
       pt->PrintAction();	
     }
     //end any action managers, e.g save trees
     Info("FinalState::Print","PostTopoActions :");
     for(auto pt : _postTopoAction) {
       pt->PrintAction();	
     }
     
     //end any action managers, e.g save trees
     Info("FinalState::Print","PostKinActions :");
     for(auto pt : _postKinAction) {
       pt->PrintAction();	
     }
   }
  
}
