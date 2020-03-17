#include "FinalState.h"
#include "AutoIterUtils.h"
#include <TFile.h>
#include <memory>

namespace chanser{
    
   
  /////////////////////////////////////////////////////////////////
  FinalState::~FinalState(){
    
    if(_ownsActions){ //if loaded from file we will own action pointers
      //this happens anytime the default constructro is used 
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
  void FinalState::ShowParticles(){
    std::cout<<GetName()<<" ShowParticles "<<endl;
    for(auto& cp: _pconfigs){
      std::cout<<cp.GetName()<<" "<<cp.PDG()<<std::endl;
    }

  }
   void FinalState::CreateFinalTree(TString fname){
      if(fname==TString()) return;
      _finalTreeFile.SetName(fname);
      _finalTree.reset();
      _finalTree=FiledTree::Recreate("finalstate",fname+WorkerName());
      cout<<"FinalState Creating Tree in file "<<fname+WorkerName()<<endl;
      ConfigureOutTree(_finalTree->Tree());
    }
    void FinalState::CreateFinalHipo(TString filename){
       if(filename==TString()) return;
      _finalHipoFile.SetName(filename);
      //Note in case of PROOF add worker ID to the end
      _finalHipo.reset(new hipo::ntuple_writer((filename+WorkerName()).Data()));
      cout<<"FinalState Creating Hipo in file "<<filename+WorkerName()<<endl;
      ConfigureOutHipo(_finalHipo.get());
    }
  void  FinalState::ConfigureOutTree(TTree* tree) {
    tree->Branch("Topo",&_currTopoID,"Topo/I");
    // tree->Branch("Correct",&fCorrect,"Correct/I");
    tree->Branch("NPerm",&_nPerm,"NPerm/I");
    //	tree->Branch("Final",&fFinal);
     GetOutEvent()->Branches(tree);
  }; //save particles to final state output
  //////////////////////////////////////////////////////////////////
  /// Define general final state data and initialise
  /// To be called at end of derived class OutHipo
  void  FinalState::ConfigureOutHipo(hipo::ntuple_writer* writer){
      
    auto bname{"FSInfo"};
    writer->bank(bname,"Topo/I,NPerm/I");
    writer->linkItemFunc(bname,"Topo/I",&_currTopoID);
    writer->linkItemFunc(bname,"NPerm/I",&_nPerm);
    GetOutEvent()->Hipo(writer);
    //to do last !!!
    // writer->open();
  }
  //////////////////////////////////////////////////////////////////
  void  FinalState::AddTopology(const TString names,const VoidFuncs funcE){
    _currTopo=_topoMan.AddTopology(names,funcE);
  }
    
  void  FinalState::SetTopologies(){
    for(auto& name:_usedTopos){
      _currTopo=_topoMan.AddTopology(name,_doToTopo[name]);
    }
    //AddTopology(name,_doToTopo[name]);
 
  }
  void  FinalState::ConfigureIters(Topology *tt){
     cout<<"FinalState::ConfigureIters() "<<endl;
   _currTopo=tt;
    _currIter=nullptr;
    AutoIter();
    _currTopo->Iter().ConfigureIters();
    // _currTopo->Iter().Print(9);
    _itersConfigured++;
  }
  void FinalState::Init(){
    cout<<"FinalState::Init() "<<endl;
    //if(!_itersConfigured)
    for(auto& topo: _topoMan.Topos())
      ConfigureIters(&topo);

    for(auto pt : _postTopoAction) {
      //Let action manager link to this final state
      pt->Configure(this);	
    }
    for(auto pt : _postKinAction) {
      //Let action manager link to this final state
      pt->Configure(this);	
    }

    if(_finalHipo) _finalHipo->open();//hipo writer must be opened after all banks defined!
      
    _currTopo=nullptr;
      
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
  ////////////////////////////////////////////////////////////////
  ///Initialise all particles as missing
  // void FinalState::InitDetected(){
  //   for(UInt_t i=0;i<_pconfigs.size();i++)
  // 	_pconfigs[i].Particle()->SetDetector(_MISSING_DET);
  // }
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

    auto validTopos =_topoMan.ValidTopos();
     
    for(auto* topo : validTopos){
      _currTopo=topo;
      _currTopoID=_currTopo->ID();
      //First combination
      auto singleCombination=(!_currTopo->FirstParticles()); //true if no other combination. Calls ParticleIter::FirstCombitorial0
      FSProcess();
      //	cout<<"Is this a single combo "<<singleCombination<<endl;
      if(singleCombination) continue;
      //Now iterate over others
      auto piter=_currTopo->Iter();
      //cout<<"FinalState::ProcessEvent() process another combi going in...."<<endl<<endl<<endl; 
      while(piter.NextCombitorial0()){
	FSProcess();
      }
    }
    // cout<<"DONE FinalState::ProcessEvent()"<<endl;
    // exit(0);
      
    if(_isGenerated){
      /* 	FSGenProcess(); //If only analysing generated events */
      return; 
    }
      
    _TotPerm+=_nPerm;
    _nEvents++;
  }
  void FinalState::FSProcess(){
    // cout<<"GOT ANOHTER ONE "<<endl;
    //CheckCombitorial();
    _nPerm++;

    auto tid=_currTopo->ID();
      
    _currTopo->Exec();
      
    //Check for any user loaded post topo actions
    //e.g, making cuts on particles
    for(const auto& act : _postTopoAction)
      if(act->Execute(tid)==kFALSE) {
	_rejectEvent=kFALSE;
	return;
      }
      
    if(_rejectEvent) return;
     
    UserProcess();
    if(_rejectEvent) return;
      
    Kinematics();
    //Check for any user loaded post topo actions
    //e.g, making cuts on particles
    for(const auto& act : _postKinAction)
      if(act->Execute(tid)==kFALSE) {
	_rejectEvent=kFALSE;
	return;
      }
    if(_rejectEvent) return;

  }
  ///////////////////////////////////////////////////////////////
  void FinalState::InitTruth(){

    for(UInt_t ip=0;ip<_pconfigs.size();ip++){
      auto place =_pconfigs[ip].GenID();
      if(place>-1){ //only check particles assigned via ConfigParti
	// 	  if(_isTruth)
	// 	    fConfigs[ip]->SetParticleVal(&frGenParts->at(fConfigs[ip]->GenID()));
	// 	  else
	_pconfigs[ip].Particle()->SetTruth(_truth->at(place));
      }
    }
  }
  ///////////////////////////////////////////////////////////////
  ///Next combination of the particle iterator
  // Bool_t FinalState::NextCombitorial(){
  //   cout<<"FinalState::NextCombitorial()"<<endl;
  //   auto piter=_currTopo->Iter();
  //   if(piter.NextCombitorial0()){cout<<"Skip this "<<piter._skipThis<<endl;return kTRUE;}
  //   return kFALSE;
  // }
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
    cout<<"FinalState::AutoIter() "<<endl;
    // auto  thisTopo = _currTopo->Definition(); //sorted detected pids (charge or pdg)
    //auto Ndet = thisTopo.size(); //total number of detectred particles for this topology
    
    //for(UInt_t ip=0;ip<Ndet;){
    // Short_t pid=thisTopo.at(ip); //get next detected particle species (charge or pdg)
    //  Short_t N_pid=std::count(thisTopo.begin(), thisTopo.end(), pid);
    // ip+=N_pid; //move to next particle species for next loop
  
     
    auto pids = whichParticleConfigPIDs(_currTopo->GetParticleConfigs()); //returns vector of pdgs
    cout<<"FinalState::AutoIter() "<<pids.size()<<endl;
    for(const auto& pid:pids){
         cout<<"FinalState::AutoIter() lopp "<<pid<<endl;
 
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
    cout<<"FinalState::Particle "<<pid<<endl;
   //Get particles with this Pid
    auto configs_pid=filterDetParticleConfigsByPid(_currTopo->GetParticleConfigs(),pid);
    //create a Particle Iterator for this species (InnerParticle)
    //need to give the vector of particles for this species
    //and the number to select from it
    ParticleIter* diter0=CreateParticleIter(_eventParts->GetParticleVector(pid),configs_pid.size());
    cout<<"FinalState::Particle "<<endl;
    //get the pointers to the particles of this species
    vector<BaseParticle* > species_parts;
    
    cout<<"FinalState::Particle "<<configs_pid.size()<<endl;
    for(const auto& particle_conf:configs_pid){
      species_parts.push_back(particle_conf.Particle());
    }
    
    //Connect actual FinalState particles to iterator
    //These particle will be updated for each combitorial
    return AddSelectToSelected(diter0,species_parts.size(),&species_parts);
    
  }
  void FinalState::InnerSelect(ParticleIter* recursiter,Int_t pid){
     cout<<"FinalState::InnerSelect "<<pid<<endl;
     //get particle configs for this pid
    auto configs_pid=filterDetParticleConfigsByPid(_currTopo->GetParticleConfigs(),pid);
    cout<<"FinalState::InnerSelect "<<configs_pid.size()<<endl;
    //how many final state particles with pid but different PDG
    auto pdgs = whichParticleConfigPDGs(configs_pid); //returns vector of pdgs

    //we need to select particles belonging to each pdg
    //get the pointers to the particles of this species
    auto Nleft=configs_pid.size();
    cout<<"FinalState::InnerSelect number of pdgs "<<pdgs.size()<<endl;
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
      cout<<"FinalState::InnerSelect PDG "<<pdg<<" has "<<Npdg<<" particles "<<endl;
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
      cout<<"FinalState::InnerSelect Parents "<<parent_names.size()<<endl;
      auto parLoopIter=selIter;
      for(auto& name:parent_names){
	cout<<"FinalState::InnerSelect Parent  "<<name<<endl;
      	//get particles with this pdg and this parent (generally should be 1!)
      	auto configs_parent=filterDetParticleConfigsByParent(configs_pdg,name);
      	//get a vector of their base particle pointers
      	vector<BaseParticle* > child_parts;
      	for(const auto& particle_conf : configs_parent){
      	  child_parts.push_back(particle_conf.Particle());
      	}
      	auto Nchild=child_parts.size();
      	auto Nleft_after_parent=Npdg-Nchild;
  	cout<<"FinalState::InnerSelect Parent  "<<name<<" "<<Nchild<<" "<<Nleft_after_parent<<endl;
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
  
  
  
  /*
  //////////////////////////////////////////////////////////////////
  ///Automatically generate iterator based on topology and particle configs
  void FinalState::AutoIter(){
    //Topo definition will be list of PDG values expect for particles
    //specified by charge, where the charge will be given instead
    auto  thisTopo = _currTopo->Definition();
    auto trueTopo = _currTopo->True(); //always PDG values

    cout<<" WARNING running HS::FinalState::AutoIter() you  should check the topology print output and make sure the displayed iterator is waht you want .."<<endl;
  
    for(UInt_t ip=0;ip<thisTopo.size();){
      //Loop over all the declared particles in the topology
      //group like particles according to their particle ID (PDG or charge)
      Short_t pid=thisTopo.at(ip);
      Short_t N_pid=std::count(thisTopo.begin(), thisTopo.end(), pid);
      if(N_pid==0){ip++;continue;}
      ip+=N_pid; //move on to next particle type for next loop


      //cout<<"Auto Iter "<<pid<<" "<<N_pid<<endl;
      //give the particles of this ID type to the iterator
      ParticleIter* diter0=CreateParticleIter(_eventParts->GetParticleVector(pid),N_pid);
      diter0->SetName(Form("PITER:%d",pid));
      diter0->SetPDG(pid);
      //cout<<"Auto Iter made iter"<< endl;

      //Look for which particle can belong to this particle ID
      //(==which pdg values have this charge)
      //if particle ID is the PDG value there will only be 1 type
      vector<vector<ParticleConfig*>> subConfigs;
      Int_t ntype=0;
      Int_t Nconfig_pid=0;
      vector<Short_t> types;
      //cout<<"Look through configs "<<_pconfigs.size()<<endl;
      for(UInt_t jp=0;jp<_pconfigs.size();jp++){
	if(_pconfigs[jp].GetNChild())continue; //not a detected particle

	//make sure this particle is in the true topology
	if(std::count(trueTopo.begin(),trueTopo.end(),_pconfigs[jp].PDG())==0) continue;
	  
	//found a particle with this particle identification
	//cout<<pid<<" "<<_pconfigs[jp]->PDG()<<" "<<_currTopo->ParticleID(_pconfigs[jp]->PDG())<<endl;
	if(_topoMan.ParticleID(_pconfigs[jp].PDG())==pid){
	  Nconfig_pid++;
	  //	cout<<"Check NConfit_pid "<< Nconfig_pid<<endl;
	  if(std::count(types.begin(),types.end(),_pconfigs[jp].PDG())==0){
	    //Found a new PDG type
	    //cout<<"new type "<<_pconfigs[jp]->PDG()<<endl;
	    ntype++;
	    types.push_back(_pconfigs[jp].PDG());
	    vector<ParticleConfig* > new_one;
	    new_one.push_back(&_pconfigs[jp]);
	    subConfigs.push_back(new_one);
	  }
	  else{//same type
	    subConfigs[ntype-1].push_back(&_pconfigs[jp]);
	  }
	}
      }
      //   subConfigs contains final state particles with same topology ID
      //   each element contains a vector of different actual PDG particles 
      //   Now need to loop over subConfigs and select them from diter
      for(Int_t it=0;it<ntype;it++){
	Int_t typePDG=subConfigs[it][0]->PDG();
	Int_t NtruePDG=_currTopo->HowManyTrue(typePDG);//number of this pdg in curr topo
	//Look for particles of this type which may have parents
	//And first select those
	Bool_t NotEnough=kFALSE;
	for(UInt_t ic=0;ic<subConfigs[it].size();){
	  ParticleConfig* parent=nullptr;
	  ParticleConfig* child = subConfigs[it][ic];
	  if((parent=child->Parent())){
	    //get children from this parent with same PDG
	    vector<BaseParticle* > child_pdg=parent->Children(child->PDG());
	    N_pid-=child_pdg.size();
	    Nconfig_pid-=child_pdg.size();
	    //Now look to see if there are identical particle of this parent type
	    //We do not want to double count these
	    vector<ParticleConfig* > all_parents=HowManyParticles(parent->PDG());
	    //cout<<"Parents "<<all_parents.size()<<" "<<child->PDG()<<" "<<child_pdg.size()<<endl;
	    Int_t NUsedParents=1;
	    for(UInt_t io=0;io<all_parents.size();io++){
	      if(all_parents[io]==parent)continue; //don't count this one again
	      if(all_parents[io]->Parent()||parent->Parent())continue; //Has a parent so should not include here as it is not identical
	      vector<BaseParticle* > child_other=all_parents[io]->Children(child->PDG());
	      //   if(child_pdg.size()+child_other.size()>N_pid)//not enough for all parents
	      if(N_pid==0)//not enough for all parents
		{
		  cout<<"WARNING AutoIter Not enough "<<child->PDG()<<" detected particles for all parents "<<parent->PDG()<<endl;
		  NotEnough=kTRUE;
		  break;
		}
	      for(UInt_t ioc=0;ioc<child_other.size();ioc++){
	      
		if(child_pdg.size()<_currTopo->HowManyTrue(typePDG)){
		  child_pdg.push_back(child_other[ioc]);
		  N_pid--; //number of this pid left in topo
		  Nconfig_pid--; //number of configured particles left
		  if(N_pid==0) break;
		  if(Nconfig_pid==0) break;
		  NtruePDG--;  //number of this pdg left in curr topo
		}	
	      }
	      NUsedParents++;
	    }
	    Int_t NtheseChild=child_pdg.size();
	    ic+=NtheseChild;
	    //remove the children from the subconfigs
	    for(UInt_t isp=0;isp<subConfigs[it].size();){
	      for(UInt_t ich=0;ich<child_pdg.size();ich++){
		if(child_pdg[ich]==subConfigs[it][isp]->Particle()){
		  subConfigs[it].erase(subConfigs[it].begin()+isp);
		  break;
		}
		if(ich==child_pdg.size()-1)isp++; //didn't find one, ove to next one
	      }
	      //Don't incerement isp in case we removed one, this will move on to the next one
	    }
	  
	    ParticleIter* diter_s=AddSelectToSelected(diter0,NUsedParents,NtheseChild/NUsedParents,&child_pdg);
	    
	    // Nconfig_pid-=NtheseChild;
	    // N_pid-=NtheseChild;
	    if(Nconfig_pid==0) break; //selected everything already
	  
	    if(NotEnough){
	      if(N_pid>0){
		//might be the odd one or two left
		ParticleIter* diter_r=AddSelectToRemainder(diter_s,1,N_pid);
		diter0=diter_r;
		NotEnough=kFALSE;
	      }
	      else break;
	    }
	    // else {
	    if(N_pid>0){
	      ParticleIter* diter_r=AddSelectToRemainder(diter_s,1,Nconfig_pid);
	      diter0=diter_r;
	    }
	  
	  
	  }
	  ic++;
	  //continue;
	}
      
	if(NotEnough) break;
	if(Nconfig_pid==0) break; //selected everything already
	if(N_pid==0) break; //selected everything already
	//Once all particle with parents have been selected, select the remainder
	//cout<<"true number "<<pid<<" "<<_currTopo->HowManyTrue(typePDG)<<" "<<typePDG<<" "<<N_pid<<" "<<Nconfig_pid<<endl;
	vector<BaseParticle* > evtparts;
	for(UInt_t isp=0;isp<subConfigs[it].size();isp++){
	  if(evtparts.size()<_currTopo->HowManyTrue(typePDG)){
	    evtparts.push_back(subConfigs[it][isp]->Particle());
	    N_pid--; //number of this pid left in topo
	    Nconfig_pid--; //number of configured particles left
	    if(N_pid==0) break;
	    if(Nconfig_pid==0) break;
	    NtruePDG--;  //number of this pdg left in curr topo
	  }
	}
	// if(NtruePDG==0) continue;
	if(evtparts.size()==0)continue;
	//cout<<"n selct "<<evtparts.size()<<" "<<N_pid<<" "<<Nconfig_pid<<endl;
      
	//Connect actual FinalState particles to iterator
	//These particle will be updated for each combitorial
	ParticleIter* diter_s=AddSelectToSelected(diter0,1,evtparts.size(),&evtparts);
      
	// Nconfig_pid-=evtparts.size();
	// N_pid-=evtparts.size();
	if(Nconfig_pid==0) break; //selected everything already
	if(N_pid==0) break; //selected everything already
	//pass the remainder on for selection
	ParticleIter* diter_r=AddSelectToRemainder(diter_s,1,Nconfig_pid);
	diter0=diter_r;
	  
      }
    }

 	  
  }
  */
  void FinalState::EndAndWrite(){
    _finalTree.reset();
    if(_finalHipo.get())_finalHipo->close();
    _finalHipo.reset();

    //end any action managers, e.g save trees
    for(auto pt : _postTopoAction) {
      pt->End();	
    }
      
    //end any action managers, e.g save trees
    for(auto pt : _postKinAction) {
      pt->End();	
    }
      
  };
       
}
