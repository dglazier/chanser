#ifdef __CINT__
 
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;
//#pragma link C++ nestedclass;
//#pragma link C++ nestedtypedef;

#pragma link C++ namespace chanser;
//#pragma link C++ namespace chanser::loader;
//#pragma link C++ global  chanser::loader::gCompilesList;
#pragma link C++ defined_in "Loader.h";
#pragma link C++ defined_in "Archive.h";

#pragma link C++ class chanser::BaseParticle+;
#pragma link C++ class chanser::TruthParticle+;
#pragma link C++ class chanser::FinalState+;
#pragma link C++ class chanser::EventParticles+;
#pragma link C++ class chanser::MaskedEventParticles+;
#pragma link C++ class chanser::MaskCalorSplitOffs+;
#pragma link C++ class chanser::MaskCalorNeutronSplitOffs+;
#pragma link C++ class chanser::MaskRadPhotons+;
#pragma link C++ class chanser::MaskSecondaryNeutrons+;
#pragma link C++ class chanser::FinalStateManager+;
#pragma link C++ class chanser::DataManager+;
#pragma link C++ class chanser::HipoData+;
#pragma link C++ class chanser::HipoTruth+;
#pragma link C++ class chanser::LundData+;
#pragma link C++ class chanser::Topology+;
#pragma link C++ class chanser::TopologyManager+;
#pragma link C++ class chanser::ParticleConfig+;
#pragma link C++ class chanser::ParticleIter+;
#pragma link C++ class chanser::Selection+;
#pragma link C++ class chanser::HipoProcessor+;
#pragma link C++ class chanser::Archive+;
#pragma link C++ class chanser::Loader+;
#pragma link C++ class chanser::ActionManager+;
#pragma link C++ class chanser::OutEventManager+;
#pragma link C++ class chanser::BaseOutEvent+;
#pragma link C++ class chanser::BaseCut+;
#pragma link C++ class chanser::DeltaTimeCut+;
#pragma link C++ class chanser::DeltaTimeVerCut+;
#pragma link C++ class chanser::EventBuilderCut+;
#pragma link C++ class chanser::BaseCorrection+;
//#pragma link C++ class chanser::AnaDB+;
//#pragma link C++ class chanser::AnaDBTable+;

#pragma link C++ class chanser::CLAS12FinalState+;
#pragma link C++ class chanser::CLAS12Particle+;
#pragma link C++ class chanser::CLAS12Neutron+;

//#pragma link C++ class chanser::+;


#endif
