#include "HipoProcessor.h"
#include "Archive.h"
#include <TH2.h>
#include <TSystem.h>
#include <TProofServ.h>

namespace chanser{
  
 
  HipoProcessor::HipoProcessor(clas12root::HipoChain* chain,TString fsfile,TString base) :
    HipoSelector(chain),_baseDir(base) {

    //prepare extra options list
    _options.reset(new TList{});
    _options->SetName("HIPOPROCESSOR_OPTIONS");
    _options->SetOwner();
    
    chain->GetNRecords();//needed to count total records etc.

    if(!fsfile.BeginsWith('/'))
      fsfile = TString(gSystem->Getenv("PWD"))+"/"+fsfile;
    GetFinalStates(fsfile); //read which final states to process from text file
    Info(" HipoProcessor::HipoProcessor",Form(" from %s found %d entries",fsfile.Data(),_listOfFinalStates->GetEntries()),"");
  }
  HipoProcessor::~HipoProcessor() {
    if(_listOfFinalStates) delete _listOfFinalStates;_listOfFinalStates=nullptr;
  }
  void HipoProcessor::Begin(TTree * /*tree*/)
  {

    //Give list of final states to input so it can be initiated on slaves
    fInput->Add(_listOfFinalStates);//make chain of files avaialbel on slaves
    //Give the base output directory
    fInput->Add(new TNamed("FSBASEDIR",_baseDir.Data()));
    //additional options
    fInput->Add(_options.get());
    
    HipoSelector::Begin(0); //Do not remove this line!


  }
  
  void HipoProcessor::SlaveBegin(TTree * /*tree*/)
  {
    HipoSelector::SlaveBegin(0); //Do not remove this line!

    _fsm.LoadData(&_hipo);
    //now initiliase all final states
    _listOfFinalStates=(dynamic_cast<TList*>(fInput->FindObject("LISTOFFINALSTATES")));

    cout<<"HipoProcessor::SlaveBegin "<<_listOfFinalStates->GetEntries()<<endl;
    for(Int_t ifs=0;ifs<_listOfFinalStates->GetEntries();++ifs){
      TString workerName;
      if(gProofServ) workerName=gProofServ->GetOrdinal();
      _fsm.LoadFinalState(_listOfFinalStates->At(ifs)->GetName(),_listOfFinalStates->At(ifs)->GetTitle(),workerName);
    }

    //read options 
    ApplyOptions();
    
    //Get the output directory
    _baseDir=(dynamic_cast<TNamed*>(fInput->FindObject("FSBASEDIR")))->GetTitle();
    _fsm.SetBaseOutDir(_baseDir);

    _fsm.Init();
     
  }
  ////////////////////////////////////////////////
  Bool_t HipoProcessor::Notify(){
    cout<<"HipoProcessor::Notify() "<<GetCurrentRecord()<<" "<<GetCurrentFileNum()<<" "<<GetCurrentFileRecords()<<endl;
    
    //This function is called whenever there is a new file
    _hipo.SetReader(_c12.get()); //use it to set the reader ptr
     
    //_hipo->addExactPid(11,1);    //exactly 1 electron
    return kTRUE;
  }

  Bool_t HipoProcessor::ProcessEvent(){
    //Equivalent to TSelector Process
    //Fill in what you would like to do with
    //the data for each event....
   
    // cout<<"HipoProcessor::ProcessEvent"<<endl;
    if(_hipo.FetchPids()){
      _fsm.ProcessEvent();
    }

    return kTRUE;
  }
 
  void HipoProcessor::SlaveTerminate()
  {
    cout<<"HipoProcessor::SlaveTerminate() "<<endl;
 
    _fsm.EndAndWrite();
 
    //give all files to be merged to the output list
    auto fstates = _fsm.GetFinalStates();
    cout<<"N final states "<<fstates.size()<<endl;
    Int_t ListNumber = 0;
    for(auto& fs: fstates){
      auto mergers = fs->UniqueMergeLists(); //we now own this list
      cout<<"Go tmergers "<<mergers.size()<<endl;
      for(auto& mergeList: mergers){
	cout<<"add ouput "<<fOutput <<" "<<mergeList->GetName()<<endl;
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
    gROOT->ProcessLine(".! rm -r chanser_FinalStates/");
  }
  /////////////////////////////////////////////////////////
  ///Loop over final states and merge trees from workers
  void HipoProcessor::MergeFinalOutput(){

    for(const auto&& obj : *fOutput){
      if(TString(obj->GetName()).Contains("MERGELIST")){
	auto merge=dynamic_cast<TList*>(obj);
	auto dest = dynamic_cast<TNamed*>(merge->FindObject("DESTINATION"));
	TString hadd(".! hadd ");
	hadd+=TString(dest->GetTitle())+"/";
	
	TString rm(".! rm  ");
	TString baseName;
	
	for(const auto&& mergefile : *merge){
	  if(TString(mergefile->GetName()).Contains("DESTINATION"))
	    continue;//List contains destination object
	  if(baseName.Length()==0){
	    baseName=gSystem->BaseName(mergefile->GetName());
	    hadd+=baseName+ " -f ";
	  }
	  hadd+=mergefile->GetName();
	  hadd+=" ";
	  rm+=mergefile->GetName();
	  rm+=" ";
	}
	
	cout<<hadd<<endl;
	gROOT->ProcessLine(hadd);
	cout<<rm<<endl;
	gROOT->ProcessLine(rm);
 
      }
    }
 }
  /////////////////////////////////////////////////////////

  void HipoProcessor::GetFinalStates(TString fsfile){
    _listOfFinalStates=new TList();
    _listOfFinalStates->SetName("LISTOFFINALSTATES");
    _listOfFinalStates->SetOwner(kTRUE);

#include <fstream>
    std::ifstream infile(fsfile.Data());
    
    if(!infile.is_open()) {
      const char* messa=Form("No final states list file found %s",fsfile.Data());
      Fatal("HipoProcessor::GetFinalStates",messa,"");
    }
    cout<<" HipoProcessor::GetFinalStates("<<endl;
    std::string fss, fis; //finalstate name and root filename
    while (infile >> fss >> fis){
      if(fss[0]=='#') //comment out with #
      	continue; 
      std::cout<<"HipoProcessor::LoadFinalStates : "<<fss << " "<<fis<<std::endl;
      _listOfFinalStates->Add(new TNamed(fss,fis));
      Archive::ExtractFinalState(fis,fss); //finalstate name, filename (full path)
    }
    //Do any compilation that is needed
    Archive::doCompileThese();
  }

  
  ////////////////////////////////////////////////
  ///Some configuration option can be passes as envirment variables
  void HipoProcessor::ApplyOptions(){
    _options.reset((dynamic_cast<TList*>(fInput->FindObject("HIPOPROCESSOR_OPTIONS"))));
    
    /////////////////////////////////////////////////
    ///Max particles for event particle
    auto opt=dynamic_cast<TNamed*>(_options->FindObject("HIPOPROCESSOR_MAXPARTICLES"));
    cout<<"HipoProcessor::ApplyOptions() "<<opt<<endl;
    if(opt!=nullptr){
      auto maxParts=TString(opt->GetTitle()).Atoi();
      Info("HipoProcessor::ApplyOptions()",Form("Setting Max particles to %d",maxParts),"");
     
      _fsm.GetEventParticles().SetMaxParticles(maxParts);
    }
    //else exit(0);
    /////////////////////////////////////////////////
    
  }
}
