#include "FiledTree.h"
#include <TDirectory.h>

namespace chanzer{


  FiledTree::~FiledTree(){
    std::cout<<"FiledTree::~FiledTree()  tree name "<<_tree->GetName()<<" "<<_tree->GetEntries()<<" "<<_file->GetName()<<std::endl;
    if(fMode==Mode_t::recreate||fMode==Mode_t::create||
       fMode==Mode_t::update||fMode==Mode_t::copyfull||
       fMode==Mode_t::copyempty){
      if(_file){ 
	_file->cd();
	if(_file->IsWritable())
	  Tree()->Write();
	else{
	  std::cout<<"Warning deleting FiledTree but can't write to file!"<<std::endl;
	  std::cout<<"        this may mean you are letting ROOT cleanup "<<std::endl;
	  std::cout<<"        this object (i.e. after .q) rather than calling"<<std::endl;
	  std::cout<<"        its destructor somewhere in your code..."<<std::endl;
	  std::cout<<"       tree name "<<_tree->GetName()<<" "<<_file->GetName()<<std::endl;
	}
      }
    }
    _tree.reset();
  }
  filed_uptr FiledTree::Recreate(TString tname,TString fname){
    auto saveDir=gDirectory;
    filed_uptr f{new FiledTree()};
    f->CreateFile(fname,"recreate");
    f->CreateTree(tname);
    f->SetMode(Mode_t::recreate);
    f->SetTreeDirectory();
    saveDir->cd();
    return std::move(f);
  }
  filed_uptr FiledTree::Read(TString tname,TString fname){
    auto saveDir=gDirectory;
    filed_uptr f{new FiledTree()};
    auto file=TFile::Open(fname,"read");
    auto tree=dynamic_cast<TTree*>(file->Get(tname));
    f->SetFile(std::move(file));
    f->SetTree(std::move(tree));
    f->SetMode(Mode_t::read);
    f->SetTreeDirectory();
    saveDir->cd();
    return std::move(f);
  }
  filed_uptr FiledTree::Update(TString tname,TString fname){
    auto saveDir=gDirectory;
    filed_uptr f{new FiledTree()};
    auto file=TFile::Open(fname,"update");
    auto tree=dynamic_cast<TTree*>(file->Get(tname));
    f->SetFile(std::move(file));
    f->SetTree(std::move(tree));
    f->SetMode(Mode_t::update);
    f->SetTreeDirectory();
    saveDir->cd();
    return f;
  }
  filed_uptr FiledTree::Create(TString tname,TString fname){
    auto saveDir=gDirectory;
    filed_uptr f{new FiledTree()};
    f->CreateFile(fname,"create");
    f->CreateTree(tname);
    f->SetMode(Mode_t::create);
    f->SetTreeDirectory();
    saveDir->cd();
    return std::move(f);
  }
  // filed_uptr FiledTree::CloneEmpty(TTree* tree,TString fname){
  //   auto saveDir=gDirectory;
  //   filed_uptr f{new FiledTree()};
  //   f->CreateFile(fname,"recreate");
  //   f->SetTree(tree->CloneTree(0));
  //   f->SetMode(Mode_t::copyempty);
  //   f->SetTreeDirectory();
  //   saveDir->cd();
  //   return std::move(f);
  // }
  // filed_uptr FiledTree::CloneFull(TTree* tree,TString fname){
  //   auto saveDir=gDirectory;
  //   filed_uptr f{new FiledTree()};
  //   f->CreateFile(fname,"recreate");
  //   f->SetTree(tree->CloneTree(-1,"fast"));
  //   f->SetMode(Mode_t::copyfull);
  //   f->SetTreeDirectory();
  //   saveDir->cd();
  //   return std::move(f);
  // }
  filed_uptr FiledTree::CloneEmpty(ttree_ptr tree,TString fname){
    auto saveDir=gDirectory;
    filed_uptr f{new FiledTree()};
    f->CreateFile(fname,"recreate");
    f->SetTree(tree->CloneTree(0));
    f->SetMode(Mode_t::copyempty);
    f->SetTreeDirectory();
    saveDir->cd();
    return std::move(f);
  }
  filed_uptr FiledTree::CloneFull(ttree_ptr tree,TString fname){
    auto saveDir=gDirectory;
    filed_uptr f{new FiledTree()};
    f->CreateFile(fname,"recreate");
    f->SetTree(tree->CloneTree(-1,"fast"));
    f->SetMode(Mode_t::copyfull);
    f->SetTreeDirectory();
    saveDir->cd();
    return std::move(f);
  }
  filed_uptr FiledTree::RecreateCopyFull(ttree_ptr tree,const TString fname,const TString selection,const Long64_t nentries){
    //using copy tree allows use of tentrylists to filter
    auto saveDir=gDirectory;
    filed_uptr f{new FiledTree()};
    f->CreateFile(fname,"recreate");
    f->SetTree(tree->CopyTree(selection,"",nentries));
    f->SetMode(Mode_t::copyfull);
    f->SetTreeDirectory();
    saveDir->cd();
    return std::move(f);
  }
 
}
