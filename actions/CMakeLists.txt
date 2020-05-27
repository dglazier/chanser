set(CHANZER chanseractions)
set (CLAS12ROOT $ENV{CLAS12ROOT})
set (HIPO $ENV{HIPO})
set(DIRNAME actions)


include_directories(${CLAS12ROOT}/Clas12Banks)
include_directories(${CLAS12ROOT}/hipo4)
include_directories(core)

ROOT_GENERATE_DICTIONARY(G__${CHANZER}   ParticleCuts.h ParticleCutsManager.h FTel_pol4_ECorrection.h ParticleCorrection.h ParticleCorrectionManager.h ParticleData.h  ParticleOutEvent.h CLAS12ParticleOutEvent.h CLAS12ParticleOutEvent0.h CLAS12SimPartOutEvent.h ParticleDataManager.h StartTimeAction.h C12StartTimeFromParticle.h EventTruthAction.h LINKDEF ActionsLinkDef.h)

add_library(${CHANZER} SHARED  ParticleCuts.cpp ParticleCutsManager.cpp ParticleCorrection.cpp ParticleCorrectionManager.cpp ParticleData.cpp ParticleDataManager.cpp StartTimeAction.cpp  EventTruthAction.cpp G__${CHANZER}.cxx)

target_link_libraries(${CHANZER} -lClas12Banks)
target_link_libraries(${CHANZER} ${ROOT_LIBRARIES} chanser)


install(TARGETS ${CHANZER} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_CHANZER_PCM ${CMAKE_BINARY_DIR}/*pcm ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)

install (FILES  ${ROOT_CHANZER_PCM}    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")

