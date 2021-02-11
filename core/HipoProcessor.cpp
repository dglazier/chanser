#include "HipoProcessor.h"
#include "Archive.h"
#include <TH2.h>
#include <TSystem.h>
#include <TProofServ.h>

namespace chanser{
  
 
  HipoProcessor::HipoProcessor(clas12root::HipoChain* chain,TString fsfile,TString base) :
    HipoSelector{chain},_baseDir(new TNamed{"FSBASEDIR",base.Data()}),_boss{kTRUE} {

    //prepare extra options list
    _options.reset(new TList{});
    _options->SetName("HIPOPROCESSOR_OPTIONS");
    _options->SetOwner(kTRUE);

    //    chain->GetNRecords();//needed to count total records etc.

    //make sure we have full path for file
    if(!fsfile.BeginsWith('/'))
      fsfile = TString(gSystem->Getenv("PWD"))+"/"+fsfile;

    //read which final states to process from text file
    GetFinalStates(fsfile);

    Info(" HipoProcessor::HipoProcessor",Form(" from %s found %d entries",fsfile.Data(),_listOfFinalStates->GetEntries()),"");

  }

  void HipoProcessor::Begin(TTree * /*tree*/)
  {
    /////Add to the input list. This is created in HipoSelector
    /////fInput is no the owner of its ptrs, so we use unique_ptrs
    ///// and give the raw ptr to fInput

    //Give list of final states to input so it can be initiated on slaves
    fInput->Add(_listOfFinalStates.get());
    //Give the base output directory
    fInput->Add(_baseDir.get());
    //additional options
    fInput->Add(_options.get()); 
    
    HipoSelector::Begin(0); //Do not remove this line!

  }
  
  void HipoProcessor::SlaveBegin(TTree * /*tree*/)
  {

    TDatabasePDG *pdgDB = TDatabasePDG::Instance();
   //name,title,mass,stable,width,charge,type.code
   pdgDB->AddParticle("deuteron","deuteron", 1.875612, kTRUE,0, 1, "Baryon", 45);  

    HipoSelector::SlaveBegin(0); //Do not remove this line!

    //give the hipor data reader to FinalStateManager
    //Data files are opened in Notify (see also HipoSelector)
    _fsm.LoadData(&_hipo);
  
    //now initiliase all final states
    auto listFinalStates=(dynamic_cast<TList*>(fInput->FindObject("LISTOFFINALSTATES")));

    //Read all the finalstate analysis objects
    for(Int_t ifs=0;ifs<listFinalStates->GetEntries();++ifs){
      TString workerName;
      if(gProofServ) workerName=gProofServ->GetOrdinal();
      _fsm.LoadFinalState(listFinalStates->At(ifs)->GetName(),listFinalStates->At(ifs)->GetTitle(),workerName);
    }

    
    //Get the output directory
    auto bDir=(dynamic_cast<TNamed*>(fInput->FindObject("FSBASEDIR")));
    _fsm.SetBaseOutDir(bDir->GetTitle());

    //read options 
    ApplyOptions();
    
    //_fsm.Init();
     
  }
  ////////////////////////////////////////////////
  Bool_t HipoProcessor::Notify(){
    HipoSelector::Notify();
    
    cout<<"HipoProcessor::Notify() "<<GetCurrentRecord()<<" "<<GetCurrentFileNum()<<" "<<GetCurrentFileRecords()<<endl;
    
    //This function is called whenever there is a new file
    _hipo.SetReader(_c12.get()); //use it to set the reader ptr

    if(_FSNotInitialsed==kTRUE){
      _fsm.Init();
      _FSNotInitialsed=kFALSE;
    }

    // run and final states now initialised
    // update final states run dependent information
    _fsm.Notify();
    
    return kTRUE;
  }

  Bool_t HipoProcessor::ProcessEvent(){
    //Equivalent to TSelector Process
    //Fill in what you would like to do with
    //the data for each event....
    if(_hipo.FetchPids()){
      _fsm.ProcessEvent();
    }

    return kTRUE;
  }

  void HipoProcessor::SlaveTerminate()
  {
 
    //write all outputs
    _fsm.EndAndWrite();
 
    //give all files to be merged to the output list
    auto fstates = _fsm.GetFinalStates();
    Int_t ListNumber = 0;
    for(auto& fs: fstates){
      auto mergers = fs->UniqueMergeLists(); //we now own this list
        for(auto& mergeList: mergers){
	auto forOutList=new TList();
	forOutList->SetOwner();
	forOutList->SetName(Form("MERGELIST_%d",ListNumber++));
	forOutList->AddAll(mergeList.get());
	forOutList->Add(new TNamed("DESTINATION",fs->FinalDirectory().Data()));
	fOutput->Add(forOutList);
      }
    }
  }

  void HipoProcessor::Terminate()
  {
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.
    cout<<"HipoProcessor::Terminate() "<<endl;
    MergeFinalOutput();
    
    //Tidy up
    gROOT->ProcessLine(".! rm -fr chanser_FinalStates/");
  }
  /////////////////////////////////////////////////////////
  ///Loop over final states and merge trees from workers
  void HipoProcessor::MergeFinalOutput(){

    for(const auto&& obj : *fOutput){
      if(TString(obj->GetName()).Contains("MERGELIST")){
	auto merge=dynamic_cast<TList*>(obj);
	auto dest = dynamic_cast<TNamed*>(merge->FindObject("DESTINATION"));
	TString hadd(".! $ROOTSYS/bin/hadd ");
	hadd+=TString(dest->GetTitle())+"/";
	
	TString rm(".! rm  ");
	TString baseName;
	bool gotMerge=kFALSE;
	for(const auto&& mergefile : *merge){
	  if(TString(mergefile->GetName()).Contains("DESTINATION"))
	    continue;//List contains destination objects
	  if(baseName.Length()==0){
	    baseName=gSystem->BaseName(mergefile->GetName());
	    hadd+=baseName+ " -f ";
	  }
	  gotMerge=kTRUE;
	  hadd+=mergefile->GetName();
	  hadd+=" ";
	  rm+=mergefile->GetName();
	  rm+=" ";
	}

	if(gotMerge){//other than DESTINATION
	  cout<<hadd<<endl;
	  gROOT->ProcessLine(hadd);
	  cout<<rm<<endl;
	  gROOT->ProcessLine(rm);
	}

      }
    }
 }
  /////////////////////////////////////////////////////////

  void HipoProcessor::GetFinalStates(TString fsfile){
    _listOfFinalStates.reset(new TList{});
    _listOfFinalStates->SetName("LISTOFFINALSTATES");
    _listOfFinalStates->SetOwner(kTRUE);

#include <fstream>
    std::ifstream infile(fsfile.Data());
    
    if(!infile.is_open()) {
      const char* messa=Form("No final states list file found %s",fsfile.Data());
      Fatal("HipoProcessor::GetFinalStates",messa,"");
    }
    std::string fss, fis; //finalstate name and root filename
    while (infile >> fss >> fis){
      if(fss[0]=='#') //comment out with #
      	continue; 
      Info("HipoProcessor::LoadFinalStates",fss.data(),fis.data());
      _listOfFinalStates->Add(new TNamed(fss,fis));
      Archive::ExtractFinalState(fis,fss); //finalstate name, filename (full path)
    }
    //Do any compilation that is needed
    Archive::doCompileThese();
  }

  
  ////////////////////////////////////////////////
  ///Some configuration option can be passes as envirment variables
  void HipoProcessor::ApplyOptions(){
    
    auto options=dynamic_cast<TList*>(fInput->FindObject("HIPOPROCESSOR_OPTIONS"));
    if(options==nullptr) return;

    /////////////////////////////////////////////////
    ///Max particles for event particle
    auto opt=dynamic_cast<TNamed*>(options->FindObject("HIPOPROCESSOR_MAXPARTICLES"));
    if(opt!=nullptr){
      auto maxParts=TString(opt->GetTitle()).Atoi();
      Info("HipoProcessor::ApplyOptions()",Form("Setting Max particles to %d",maxParts),"");
     
      _fsm.GetEventParticles().SetMaxParticles(maxParts);
    }
    /////////////////////////////////////////////////
    /////////////////////////////////////////////////
    ///Write filtered hipo output file
    opt=dynamic_cast<TNamed*>(options->FindObject("HIPOPROCESSOR_FILTERHIPO"));
    if(opt!=nullptr){
      TString workerName;
      if(gProofServ) workerName=gProofServ->GetOrdinal();
      
      cout<<"Writing hipo filtered to "<<_fsm.BaseOutDir()+opt->GetTitle()<<endl;
      _hipo.SetWriteToFile(_fsm.BaseOutDir()+"worker_"+workerName+opt->GetTitle());
    }
    /////////////////////////////////////////////////
    ///Write filtered hipo output file
    opt=dynamic_cast<TNamed*>(options->FindObject("HIPOPROCESSOR_ANADB"));
    if(opt!=nullptr){
      TString pathsStr=opt->GetTitle();
      auto paths = pathsStr.Tokenize(":");
      for(auto* path:*paths)
	_hipo.LoadAnaDB(path->GetName());
    }
  }

}
