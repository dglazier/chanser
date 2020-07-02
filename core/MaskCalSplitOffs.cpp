#include "MaskCalSplitOffs.h"


namepsace chanser{


  void AssignVectors(const EventParticles* ep){
    //Just Copy all EventParticle vectors
    MaskedEventParticles::AssignVectors(ep);
    //use my own gamma
    SetMapVector(22,_vecGams);
    //which I am going to make from
    _originalGams=&ep->vecGams;
    
  }


}
