#include "EventParticles.h"
#include "FinalState.h"

namespace chanser{

  EventParticles::EventParticles(){
    // _mapPDGtoParticle[2212]=&_vecProtons;
    // _mapPDGtoParticle[-2212]=&_vecAntiProtons;
    // _mapPDGtoParticle[2112]=&_vecNeutrons;
    // _mapPDGtoParticle[211]=&_vecPiPs;
    // _mapPDGtoParticle[-211]=&_vecPiMs;
    // _mapPDGtoParticle[111]=&_vecPi0s;
    // _mapPDGtoParticle[321]=&_vecKPs;
    // _mapPDGtoParticle[-321]=&_vecKMs;
    // _mapPDGtoParticle[11]=&_vecEls;
    // _mapPDGtoParticle[-11]=&_vecPos;
    // _mapPDGtoParticle[22]=&_vecGams;
    // _mapPDGtoParticle[1E4]=&_vecPlus;
    // _mapPDGtoParticle[-1E4]=&_vecMinus;
    // _mapPDGtoParticle[0]=&_vec0;
    // _mapPDGtoParticle[45]=&_vec45;
    
    SetMapVector(2212,(&_vecProtons));
    SetMapVector(-2212,(&_vecAntiProtons));
    SetMapVector(2112,(&_vecNeutrons));
    SetMapVector(211,(&_vecPiPs));
    SetMapVector(-211,(&_vecPiMs));
    SetMapVector(111,(&_vecPi0s));
    SetMapVector(321,(&_vecKPs));
    SetMapVector(-321,(&_vecKMs));
    SetMapVector(11,(&_vecEls));
    SetMapVector(-11,(&_vecPos));
    SetMapVector(22,(&_vecGams));
    SetMapVector(1E4,(&_vecPlus));
    SetMapVector(-1E4,(&_vecMinus));
    SetMapVector(0,(&_vec0));
    SetMapVector(45,(&_vec45));
  }


  ////////////////////////////////////////////////////////////
  ///Seperate all particles from data into species vector
  Bool_t EventParticles::ReadEvent(const particles_ptrs &event){
  
 //Clear all particle vectors
    std::map< Short_t , particles_ptrs* >::iterator itmap;
    for (itmap = _mapPDGtoParticle.begin(); itmap != _mapPDGtoParticle.end(); itmap++)
      {
	itmap->second->clear();
      }
    //Load particles from data into vectors
     for(auto& part : event){
      //check if this particle is OK from users check particle
      particles_ptrs* vecParts=_mapPDGtoParticle.at(part->PDG());
      //add this particle to its particle species vector
      auto ppdg=part->PDG();
      if(vecParts&&(ppdg!=0||ppdg!=UndefinedPDG||ppdg!=-UndefinedPDG))
	vecParts->push_back(part); //some events have PDG=0, just include these in charge0 vec
      //add particle to charge ID vector
      auto chpar= _chargetoParticle[part->Charge()+1];
      chpar->push_back(part);
      // cout<<"DEBUG EventParticles::ReadEvent  "<<" chpar "<<chpar<<" vecParts "<<vecParts<<" "<<part->PDG()<<" "<<part->Charge()<<endl;
      if(vecParts->size()==_maxPart) {return kFALSE;}
      if(chpar->size()==_maxPart) {return kFALSE;} //this should overide previous line ?! And max parts is just on charge
    }
     //  Print();
    return kTRUE;
    
  }
  void EventParticles::Print() const{
    std::cout<<"EventParticles::Print() "<<std::endl;
    std::map< Short_t , particles_ptrs* >::const_iterator itmap;
    for (itmap = _mapPDGtoParticle.begin(); itmap != _mapPDGtoParticle.end(); itmap++)
      std::cout<< itmap->first <<"has "<<itmap->second->size()<<" particles "<<std::endl;
  }
}
