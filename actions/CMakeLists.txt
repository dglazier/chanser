set(CHANZER chanseractions)
set (CLAS12ROOT $ENV{CLAS12ROOT})
set (HIPO $ENV{HIPO})
set(DIRNAME actions)
set (CHANSERBASE $ENV{CHANSER})


include_directories(${CLAS12ROOT}/Clas12Banks)
include_directories(${CLAS12ROOT}/Clas12Root)
include_directories(${CLAS12ROOT}/hipo4)
include_directories(core)
include_directories(tmva)

ROOT_GENERATE_DICTIONARY(G__${CHANZER}   ParticleCuts.h ParticleCutsManager.h CLAS12ParticleCut.h CLAS12NeutronParticleCut.h TwoParticleVertexCut.h CLAS12Base.h FTel_pol4_ECorrection.h FDn_pathCorrection.h FTel_VzCorrection.h ParticleCorrection.h ParticleCorrectionManager.h ParticleData.h  ParticleOutEvent.h CLAS12ParticleOutEvent.h CLAS12ParticleOutEvent0.h CLAS12SimPartOutEvent.h ParticleDataManager.h MVASignalIDAction.h StartTimeAction.h C12StartTimeFromParticle.h EventTruthAction.h TriggerAction.h MCMatchAction.h WriteMomentumAction.h LINKDEF ActionsLinkDef.h)

add_library(${CHANZER} SHARED  ParticleCuts.cpp CLAS12ParticleCut.cpp TwoParticleVertexCut.cpp FDn_pathCorrection.cpp ParticleCutsManager.cpp ParticleCorrection.cpp ParticleCorrectionManager.cpp ParticleData.cpp ParticleDataManager.cpp MVASignalIDAction.cpp StartTimeAction.cpp  C12StartTimeFromParticle.cpp EventTruthAction.cpp  TriggerAction.cpp MCMatchAction.cpp WriteMomentumAction.cpp G__${CHANZER}.cxx)

target_link_libraries(${CHANZER} -lClas12Banks)
target_link_libraries(${CHANZER} ${ROOT_LIBRARIES} chanser)
target_link_libraries(${CHANZER} ${ROOT_LIBRARIES} chansermva)


install(TARGETS ${CHANZER} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_CHANZER_PCM ${CMAKE_BINARY_DIR}/*pcm ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)

install (FILES  ${ROOT_CHANZER_PCM}    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")

