#include "OutEventManager.h"

namespace chanser {
  
  void OutEventManager::Init(TString outDir){
    std::cout<<" OutEventManager::Init "<<outDir<<" "<<(_outputType==FSOutputType::ROOTTREE)<<std::endl;

   
    
    if(_outputType==FSOutputType::ROOTTREE){
      //create output list for merging
      _listOfFinalTrees.reset(new TList{});
      _listOfFinalTrees->SetName("FINALOUTTREE");
     
      
      CreateFinalTree(outDir);
    }
    if(_outputType==FSOutputType::HIPONTUPLE){
      CreateFinalHipo(outDir);
    }
  }
  
  void OutEventManager::CreateFinalTree(const TString& fname){
      if(fname==TString()) return;
      std::cout<<"CreateFinalTree "<<fname<<std::endl;
      auto finalTreeFile=fname+"FinalState.root";
      _finalTree.reset();
      _finalTree=FiledTree::Recreate(_listOfFinalTrees->GetName(),finalTreeFile);

      _listOfFinalTrees->Add(new TObjString(finalTreeFile));
      
      ConfigureOutTree(_finalTree->Tree());
    }
    void OutEventManager::CreateFinalHipo(const TString& filename){
       if(filename==TString()) return;
       auto finalHipoFile=filename+"FinalState.hipo";
      //Note in case of PROOF add worker ID to the end
      _finalHipo.reset(new hipo::ntuple_writer((finalHipoFile).Data()));
    
      ConfigureOutHipo(_finalHipo.get());
    }
  void  OutEventManager::ConfigureOutTree(TTree* tree)  const{
    if(_realTD.get()!=nullptr)_realTD->Branches(tree);
    
    if(_truTD.get()!=nullptr){
      _truTD->SetName(TString("tru")+_truTD->GetName());
      _truTD->Branches(tree);
    }
  
  }; //save particles to final state output
  //////////////////////////////////////////////////////////////////
  /// Define general final state data and initialise
  /// To be called at end of derived class OutHipo
  void  OutEventManager::ConfigureOutHipo(hipo::ntuple_writer* writer) const{
      
    std::string bname{"FSInfo"};
    writer->bank(bname,"Topo/I,NPerm/I");
    //writer->linkItemFunc(bname,"Topo/I",&_currTopoID);
    //writer->linkItemFunc(bname,"NPerm/I",&_nPerm);
    if(_realTD.get()!=nullptr)_realTD->Hipo(writer);
    
  }
  
  void OutEventManager::Fill(){
    if(_finalTree.get())_finalTree->Fill();
    if(_finalHipo.get())_finalHipo->fill();
  }
  void OutEventManager::Finish(){

    _finalTree.reset();
    if(_finalHipo.get())_finalHipo->close();
    _finalHipo.reset();
  }


}
