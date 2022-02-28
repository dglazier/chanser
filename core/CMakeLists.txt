set(CHANZER chanser)
set (CLAS12ROOT $ENV{CLAS12ROOT})
set (HIPO $ENV{HIPO})
set(DIRNAME core)
set (CHANSERBASE $ENV{CHANSER})

include_directories(${CLAS12ROOT}/Clas12Banks)
include_directories(${CLAS12ROOT}/Clas12Root)
include_directories(${CLAS12ROOT}/hipo4)
include_directories(actions)
include_directories(tmva)

ROOT_GENERATE_DICTIONARY(G__${CHANZER}  Archive.h AnaDB.h Loader.h FiledTree.h BaseOutEvent.h BaseRunInfo.h RunInfo.h BaseEventInfo.h EventInfo.h BaseParticle.h CLAS12Particle.h DataManager.h HipoData.h HipoTruth.h LundData.h ParticleConfig.h  Selection.h ParticleIter.h  Topology.h TopologyManager.h EventParticles.h MaskedEventParticles.h   MaskCalorSplitOffs.h   MaskCalorNeutronSplitOffs.h MaskRadPhotons.h OutEventManager.h ActionManager.h BaseCut.h  BaseCorrection.h  FinalState.h CLAS12FinalState.h CLAS12Particle.h   FinalStateManager.h  HipoProcessor.h  LINKDEF ChanserLinkDef.h)

add_library(${CHANZER} SHARED Archive.cpp AnaDB.cpp Loader.cpp FiledTree.cpp BaseOutEvent.cpp RunInfo.cpp EventInfo.cpp BaseParticle.cpp  DataManager.cpp HipoData.cpp HipoTruth.cpp LundData.cpp ParticleConfig.cpp Selection.cpp ParticleIter.cpp  Topology.cpp TopologyManager.cpp EventParticles.cpp  MaskedEventParticles.cpp  MaskCalorSplitOffs.cpp  MaskCalorNeutronSplitOffs.cpp MaskRadPhotons.cpp OutEventManager.cpp ActionManager.cpp BaseCut.cpp BaseCorrection.cpp FinalState.cpp FinalStateManager.cpp HipoProcessor.cpp G__${CHANZER}.cxx)

target_link_libraries(${CHANZER} ${ROOT_LIBRARIES})
target_link_libraries(${CHANZER} -lClas12Banks)
target_link_libraries(${CHANZER} -L${CLAS12ROOT}/lib)


install(TARGETS ${CHANZER} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_CHANZER_PCM ${CMAKE_BINARY_DIR}/*pcm ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)

install (FILES  ${ROOT_CHANZER_PCM}    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)
