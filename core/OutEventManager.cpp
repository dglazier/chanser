#include "OutEventManager.h"

namespace chanser {

  void OutEventManager::Init(TString outDir){
    std::cout<<" OutEventManager::Init "<<outDir<<" "<<(_outputType==FSOutputType::ROOTTREE)<<std::endl;
    if(_outputType==FSOutputType::ROOTTREE){
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
      _finalTree=FiledTree::Recreate("finalstate",finalTreeFile);
     
      ConfigureOutTree(_finalTree->Tree());
    }
    void OutEventManager::CreateFinalHipo(const TString& filename){
       if(filename==TString()) return;
       auto finalHipoFile=filename+"FinalState.hipo";
      //Note in case of PROOF add worker ID to the end
      _finalHipo.reset(new hipo::ntuple_writer((finalHipoFile).Data()));
    
      ConfigureOutHipo(_finalHipo.get());
      _finalHipo->open();//hipo writer must be opened after all banks defined!
    }
  void  OutEventManager::ConfigureOutTree(TTree* tree)  const{
    //tree->Branch("Topo",&_currTopoID,"Topo/I");
    // tree->Branch("Correct",&fCorrect,"Correct/I");
    //tree->Branch("NPerm",&_nPerm,"NPerm/I");
    //	tree->Branch("Final",&fFinal);

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
      
    auto bname{"FSInfo"};
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
