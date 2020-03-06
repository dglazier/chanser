set(CHANZER chanzer)
set (CLAS12ROOT $ENV{CLAS12ROOT})
set (HIPO $ENV{HIPO})
set(DIRNAME core)

include_directories(${CLAS12ROOT}/Clas12Banks)
include_directories(${CLAS12ROOT}/Clas12Root)
include_directories(${CLAS12ROOT}/hipo4)

ROOT_GENERATE_DICTIONARY(G__${CHANZER}  Archive.h Loader.h FiledTree.h BaseOutEvent.h BaseRunInfo.h RunInfo.h BaseEventInfo.h EventInfo.h BaseParticle.h DataManager.h HipoData.h ParticleConfig.h  Selection.h ParticleIter.h  Topology.h TopologyManager.h EventParticles.h ActionManager.h BaseCut.h ParticleCuts.h ParticleCutsManager.h ParticleData.h ParticleDataManager.h FinalState.h   FinalStateManager.h  TreeDataPi2.h anaPi2.h HipoProcessor.h  LINKDEF ChanzerLinkDef.h)

add_library(${CHANZER} SHARED Archive.cpp Loader.cpp FiledTree.cpp BaseOutEvent.cpp RunInfo.cpp EventInfo.cpp BaseParticle.cpp  DataManager.cpp HipoData.cpp ParticleConfig.cpp Selection.cpp ParticleIter.cpp  Topology.cpp TopologyManager.cpp EventParticles.cpp  ActionManager.cpp BaseCut.cpp  ParticleCuts.cpp ParticleCutsManager.cpp   ParticleData.cpp ParticleDataManager.cpp FinalState.cpp FinalStateManager.cpp anaPi2.cpp HipoProcessor.cpp G__${CHANZER}.cxx)

target_link_libraries(${CHANZER} ${ROOT_LIBRARIES})
target_link_libraries(${CHANZER} -lClas12Banks)
target_link_libraries(${CHANZER} -L${CLAS12ROOT}/lib)


install(TARGETS ${CHANZER} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_CHANZER_PCM ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)

install (FILES  ${ROOT_CHANZER_PCM}    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)
