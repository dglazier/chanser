#include "Topology.h"
#include "TDatabasePDG.h"
#include "TObjString.h"


namespace chanser{

  using std::vector;
  /////////////////////////////////////////////////////////////////
  ///Contructor requires particle definition string
  ///This will be convertor into a vector of codes to check versus the
  ///detected particles in the event
  ///If UseChargeID is set the code is just the particle charge
  ///If not it is the PDG code
  ///incl is a string to particles that any number can be detected of for an event
  ///if incl=="ALL" then all particles are allowed inclusive
  Topology::Topology(const TString topo,const VoidFuncs funcE, TopologyManager* tm){
    //SetIter(std::bind(&Topology::TestFunc,this));
    //Set the pointers to the FinalState functions
    Exec=funcE;
    
  
    //split the string into an array of particles
    TObjArray* OptList = topo.Tokenize(":");

    for(Int_t i=0;i<OptList->GetEntries();i++){
      Int_t pdg=0;
      Int_t charge=0;
      if(!TDatabasePDG::Instance()->GetParticle(((TObjString*)OptList->At(i))->String()))Error("FinalState::AddTopology","Particle not found = %s",((TObjString*)OptList->At(i))->String().Data());
      else{
	pdg=TDatabasePDG::Instance()->GetParticle(((TObjString*)OptList->At(i))->String())->PdgCode();
      }
      _trueDefinition.push_back(pdg);
      _actualDefinition.push_back(tm->ParticleID(pdg));
    }
  
    //sort into order for checking
    std::sort(_trueDefinition.begin(),_trueDefinition.begin()+_trueDefinition.size());
    std::sort(_actualDefinition.begin(),_actualDefinition.begin()+_actualDefinition.size());
      
    

  }

  
 
  ///////////////////////////////////////////////////////////////////////
  void Topology::Print(Int_t verbose) const {
    cout<<" Topology::Print() : "<<_ID<<endl;
    cout<<"    particles = ";
    for(UInt_t i=0;i<_actualDefinition.size();i++)
      cout<<_actualDefinition[i]<<" ";
    cout<<endl;
    
    cout<<"PRINT TOPO ITER "<<endl;
    _particleIter.Print(verbose);

  }
  
  
}

