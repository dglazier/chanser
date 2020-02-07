#include "TreeData.h"
#include "TDataMember.h"
#include <iostream>
#include <map>


////////////////////////////////////////////////////////
/// Make branches in a tree for each datamember
/// we do this rather than just creating an object branch
/// as only this way works in RooFit!

void chanzer::TreeData::Branches(TTree* tree,TList* dmList){
  if(tree==nullptr) return;
  
  std::map<TString,TString> typelabel;
  typelabel["Double_t"]="/D";typelabel["double"]="/D";
  typelabel["Float_t"]="/F";typelabel["float"]="/F";
  typelabel["Int_t"]="/I";typelabel["int"]="/I";
  typelabel["Long64_t"]="/L";typelabel["long64"]="/L";
  typelabel["Short_t"]="/S"; typelabel["short"]="/S";

  auto sizeOfShort=2;
  for(Int_t i=0;i<dmList->GetEntries();i++){
    auto member=dynamic_cast<TDataMember*> (dmList->At(i));
    auto moffset = member->GetOffset()/sizeOfShort;
    auto mtype=TString(member->GetFullTypeName());
    TString mname=_name+member->GetName();
    if(typelabel.find(mtype)==typelabel.end()){
      continue;
    }
    if(_forBranch.size()) //if list of allowed branches 
      if(std::find(_forBranch.begin(),_forBranch.end(),mname)==_forBranch.end())
	continue;  //if not in list don't make branch
    //make the branch, giving the address of this member
    tree->Branch(mname,(((Short_t*)&(*this))+moffset),mname+typelabel[mtype]);
    
  }
}

void chanzer::TreeData::Hipo(hipo::ntuple_writer* writer,TList* dmList){
  if(writer==nullptr) return;

  std::map<TString,TString> typelabel;
  typelabel["Double_t"]="/D";typelabel["double"]="/D";
  typelabel["Float_t"]="/F";typelabel["float"]="/F";
  typelabel["Int_t"]="/I";typelabel["int"]="/I";
  typelabel["Long64_t"]="/L";typelabel["long64"]="/L";
  typelabel["Short_t"]="/S"; typelabel["short"]="/S";

  TString schemaString;
     
  auto sizeOfShort=2;
  std::vector<TString > items;
  
  for(Int_t i=0;i<dmList->GetEntries();i++){
    auto member=dynamic_cast<TDataMember*> (dmList->At(i));
    auto moffset = member->GetOffset()/sizeOfShort;
    auto mtype=TString(member->GetFullTypeName());
    TString mname=member->GetName();
    if(typelabel.find(mtype)==typelabel.end()){
      continue;
    }
    if(_forBranch.size()) //if list of allowed branches 
      if(std::find(_forBranch.begin(),_forBranch.end(),mname)==_forBranch.end())
	continue;  //if not in list don't make branch
    //make the branch, giving the address of this member

    items.push_back(mname);
    schemaString+=mname+typelabel[mtype]+",";
    
    _linksHipo.push_back((((Short_t*)&(*this))+moffset));
  }
  schemaString.Chop();//remove last ,
  
  //create ntuple_writer bank with the full string
  writer->bank(_name.Data(),schemaString.Data());

  uint item=0;
  //go back over items and link data members to writer
  for(auto& addr : _linksHipo){
    std::cout<<items[item]<<std::endl;
    writer->linkItemFunc(_name.Data(),items[item].Data(),addr);
    item++;
  }
 }

