////////////////////////////////////////////////////////////////
///
///Class:               FiledTree
///Description:
///            A tree connected to its own TFile !
///            An attempt to stop file directory related crashes!
///            Note in interactive root session must create a new
///            object so we can call delete. If you let ROOT delete
///            after .q the file will not be writeable!
 
#pragma once

#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <iostream>

namespace chanser{

  using tfile_uptr=std::unique_ptr<TFile>;
  using ttree_uptr=std::unique_ptr<TTree>;
  using ttree_ptr=TTree*;

  class FiledTree  {
    enum class Mode_t{null,recreate,create,read,update,copyempty,copyfull};
    
    using filed_ptr=chanser::FiledTree*;
    using filed_uptr=std::unique_ptr<chanser::FiledTree>;

  public:
    //////////////////////////////////////////////////////////////////////
    ///Construct a new tree to be branched and filled
    FiledTree(TString tname,TString fname,TString opt="recreate"){
      _file.reset(TFile::Open(fname,opt));
      _tree.reset(new TTree(tname,"A FiledTree"));
      SetTreeDirectory();
    }
    //////////////////////////////////////////////////////////////////////
    ///Construct a tree based on an existing one
    ///if isfull is true copy all the events, if false just create empty tree
    FiledTree(Bool_t isfull,TTree *tree,TString fname,TString opt="recreate"){
      _file.reset(TFile::Open(fname,opt));
      if(isfull)_tree.reset(tree->CloneTree(-1,"fast"));
      else _tree.reset(tree->CloneTree(0));
      SetTreeDirectory();
    }
    
    FiledTree(const FiledTree&)=delete;
    FiledTree(FiledTree&&)=default;
    FiledTree& operator=(const FiledTree& other)= delete;
    FiledTree& operator=(FiledTree&& other) = default;

    virtual ~FiledTree();
    
    ttree_ptr Tree() const {return _tree.get();}
    
    static filed_uptr Recreate(const TString tname,const TString fname);
    static filed_uptr Create(const TString tname,const TString fname);
    static filed_uptr Read(const TString tname,const TString fname);
    static filed_uptr Update(const TString tname,const TString fname);
    // static filed_uptr CloneEmpty(TTree* tree,const TString fname);
    //static filed_uptr CloneFull(TTree* tree,const TString fname);
    static filed_uptr CloneEmpty(ttree_ptr tree,const TString fname);
    static filed_uptr CloneFull(ttree_ptr tree,const TString fname);
    //static filed_uptr RecreateCopyFull(TTree* tree,const TString fname,const TString selection="",const Long64_t nentries = TTree::kMaxEntries);
    static filed_uptr RecreateCopyFull(ttree_ptr tree,const TString fname,const TString selection="",const Long64_t nentries = TTree::kMaxEntries);

    void SetMode(const Mode_t m){fMode=m;}
    Mode_t Mode(){return fMode;}


    Long64_t Fill(){return _tree->Fill();}
  protected :

    TFile* File() const {return _file.get();}
    void SetTreeDirectory(){ Tree()->SetDirectory(File());}
    void CreateFile(TString fname,TString opt){ _file.reset(TFile::Open(fname,opt));}
    void CreateTree(TString tname){_tree.reset(new TTree(tname,"A FiledTree"));}
    void SetFile(TFile* f){ _file.reset(f);}
    void SetTree(TTree* t){ _tree.reset(t);}

  private:
    tfile_uptr _file={nullptr};//file before tree as is owner
    ttree_uptr _tree={nullptr};
    Mode_t fMode=Mode_t::null;

    FiledTree()=default;
    
  };

  using filed_ptr=chanser::FiledTree*;
  using filed_uptr=std::unique_ptr<chanser::FiledTree>;
}
