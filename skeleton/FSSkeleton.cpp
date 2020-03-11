
#include "FSSkeleton.h"
#include <TSystem.h>
#include <iostream>

namespace chanser{
  using std::cout;
  using std::endl;
  ////////////////////////////////////////////////////
  /// Create a FinalState project. \n
  /// Set _finalTopo with SetFinalStateTopo() \n
  /// Set _finalParts with SetFinalStateParts() \n
  /// Set _finalName with SetFinalState() \n
  void FSSkeleton::MakeCode(){
    _place=0;
 
    if(_finalTopo==TString(""))cout<<"Warning No finalstate topologies set, you will have to edit the files yourself"<<endl;
    else{
      _topos=_finalTopo.Tokenize(",");
    }
 
    if(_finalParts==TString(""))cout<<"Warning No finalstate final state set, you will have to edit the files yourself"<<endl;
    else{
      _finals=_finalParts.Tokenize(",");
    }

    TString CHANSER=gSystem->Getenv("CHANSER");
    _strSkel=CHANSER+"/skeleton";
    _userName=gSystem->Getenv("USER");

    //Copy template class header file
    gSystem->Exec(Form("cp %s/templateFS.h %s.h",_strSkel.Data(),_finalName.Data()));

    //Open the header file
    _curMacro=TMacro(_finalName+".h");

    //Replace templateFS with new class name
    ReplaceAllMacroText("templateFS",_finalName);
    ReplaceAllMacroText("USERNAME",_userName);
    TString UpperFinalName=_finalName;
    UpperFinalName.ToUpper();
    //ReplaceAllMacroText("FINALTEMP",UpperFinalName);

    //Declare detected particles in .h
    DeclareDetected();
    //Now declare parent particles in .h
    DeclareParents();

    //For use in derived class to add additional functionalty
    DeclareExtras();
  
    //Save header code
    _curMacro.SaveSource(_finalName+".h");

    ///////////////////////////////////////////////////////
    //Copy the template class .C file
    gSystem->Exec(Form("cp %s/templateFS.cpp %s.cpp",_strSkel.Data(),_finalName.Data()));
    //Open code for editting
    _curMacro=TMacro(_finalName+".cpp");
  
    //replace all instances of templateFS with your defined name
    _place=0;
    ReplaceAllMacroText("templateFS",_finalName);
    ReplaceAllMacroText("USERNAME",_userName);

    //Define Detected particles
    DefineDetected();
    //Define their parents if any declared
    if(_finalParents!=TString("")) DefineParents(); 
    //Define each topology you have registered
    DefineTopologies();
    //Define topology functions
    //  DefineTopoFuncs(); //superceded by seperate TopoXXX.C file 
    //For use in derived class to add additional functionalty
    DefineExtras();

    //Save new source code
    _place=0;
    _curMacro.SaveSource(_finalName+".cpp");

    //Macros for running on data files
    CreateRunMacros();
 
    //Make the TreeData class
    CreateTreeData();
    //Now make a TopoFuncs file
    //CreateTopoFuncs();
 
  }



  void FSSkeleton::DeclareDetected(){
    //      //Final Particles Detected
    //      chanser::CLAS12Particle _PARTICLE=BaseParticle("PDG");//!

    //Declare detected particles in final state
    _place=0;
    FindNextLineLike("//Final Particles Detected");
    for(Int_t io=0;io<_finals->GetEntries();io++){
      TString sparticle=_finals->At(io)->GetName();
      TString pname=TString(sparticle(0,sparticle.First(":")));
      ContinueLineAfter(Form("    Particle   _%s = Particle{\"%s\"};",TString(sparticle(0,sparticle.First(":"))).Data(),TString(sparticle(sparticle.First(":")+1,sparticle.Sizeof())).Data()));
      }
    }
  
  void FSSkeleton::DeclareParents(){
    _place=0;
    if(_finalParents!=TString("")){
      _parents=_finalParents.Tokenize(",");
      _place=0;
      FindNextLineLike("//Final Parents");
      for(Int_t io=0;io<_parents->GetEntries();io++){
	TString sparticle=_parents->At(io)->GetName();
	ContinueLineAfter(Form("    Particle _%s = Particle{\"%s\"};",TString(sparticle(0,sparticle.First(":"))).Data(),TString(sparticle(sparticle.First(":")+1,sparticle.First(";")-sparticle.First(":")-1)).Data()));
      }
    }
  }
  void FSSkeleton::DefineTopologies(){
    _place=0;
    FindNextLineLike("//Set Possible Topologies");
    for(Int_t io=0;io<_topos->GetEntries();io++){
      ContinueLineAfter(Form("    _linkToTopo[\"%s\"]=[&](){",_topos->At(io)->GetName()));
      ContinueLineAfter("      //TOPOLOGY Define your topology dedendent code in here");
      ContinueLineAfter("      ///////+++++++++++++++++++++++++++++++++++///////");
      ContinueLineAfter("      ");
      ContinueLineAfter("      ");
      ContinueLineAfter("      ///////------------------------------------///////");
      
      ContinueLineAfter("    };");
    }
  }

 
 
  void FSSkeleton::DefineDetected(){
    _place=0;
    FindNextLineLike("//Set final state detected particles");
    _place+=3;
    for(Int_t io=0;io<_finals->GetEntries();io++){
      TString sparticle=_finals->At(io)->GetName();
      TString pname=TString(sparticle(0,sparticle.First(":")));
      ContinueLineAfter(Form("    AddParticle(\"%s\",&_%s,kTRUE,-1);",pname.Data(),pname.Data()));
    }
  }
  void FSSkeleton::DefineParents(){
    _place=0;
    FindNextLineLike("//Set final state parents");
    for(Int_t io=0;io<_parents->GetEntries();io++){
      TString sparticle=_parents->At(io)->GetName();
      TString pname=TString(sparticle(0,sparticle.First(":"))).Data();
      ContinueLineAfter(Form("    AddParticle(\"%s\",&_%s,kTRUE,-1);",pname.Data(),pname.Data()));
      TObjArray *childs=sparticle.Tokenize(";");
      for(Int_t ic=1;ic<childs->GetEntries();ic++)
	ContinueLineAfter(Form("    ConfigParent(&_%s,&_%s);",pname.Data(),childs->At(ic)->GetName()));
    }
  }

  void FSSkeleton::CreateRunMacros(){
    //Create some sample Run macros
  
    gSystem->Exec(Form("cp %s/Create_templateFS.C Create_%s.C",_strSkel.Data(),_finalName.Data()));
    _curMacro=TMacro(TString("Create_")+_finalName+".C");
    _place=0;
    ReplaceAllMacroText("templateFS",_finalName);
    ReplaceAllMacroText("USERNAME",_userName);

    _curMacro.SaveSource(TString("Create_")+_finalName+".C");

    gSystem->Exec(Form("cp %s/Run_templateFS.C Run_%s.C",_strSkel.Data(),_finalName.Data()));
    _curMacro=TMacro(TString("Run_")+_finalName+".C");
    _place=0;
    ReplaceAllMacroText("templateFS",_finalName);
    ReplaceAllMacroText("USERNAME",_userName);

    _curMacro.SaveSource(TString("Run_")+_finalName+".C");

  
  }
  void FSSkeleton::CreateTreeData(){
  
    gSystem->Exec(Form("cp %s/TreeDatatemplateFS.h TreeData%s.h",_strSkel.Data(),_finalName.Data()));
    _curMacro=TMacro(TString("TreeData")+_finalName+".h");
    _place=0;
    ReplaceAllMacroText("templateFS",_finalName);
    ReplaceAllMacroText("USERNAME",_userName);
   _curMacro.SaveSource(TString("TreeData")+_finalName+".h");
  }
  

  void FSSkeleton::CreateTopoFuncs(){
    _curMacro=TMacro();
    ContinueLineAfter(Form("#include \"%s.h\"",_finalName.Data()));
    for(Int_t io=0;io<_topos->GetEntries();io++){
      ContinueLineAfter(Form("void %s::Topo_%d(){",_finalName.Data(),io));
      ContinueLineAfter(Form("  //For topology %s",_topos->At(io)->GetName()));

      ContinueLineAfter("");
      ContinueLineAfter("  //Reconstruct missing or combined particles");
      ContinueLineAfter("  //HSLorentzVector miss=fBeam+fTarget-fElectron.P4()...;");
      ContinueLineAfter("  //fMissMass2=miss.M2();");
      ContinueLineAfter("  //fMissMass=miss.M();");
      ContinueLineAfter("}");
    
    }
    _curMacro.SaveSource(TString("Topos")+_finalName+".h");
  }
 
}
