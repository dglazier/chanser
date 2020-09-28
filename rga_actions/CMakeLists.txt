set(CHANZER chanseractions_rga)
set (CLAS12ROOT $ENV{CLAS12ROOT})
set (HIPO $ENV{HIPO})
set(DIRNAME rga_actions)
set (CHANSERBASE $ENV{CHANSER})


include_directories(${CLAS12ROOT}/Clas12Banks)
include_directories(${CLAS12ROOT}/hipo4)
include_directories(core)
include_directories(tmva)
include_directories(actions)

ROOT_GENERATE_DICTIONARY(G__${CHANZER} ZVertexCut.h FiducialCutPCAL_uvw.h DC_FiducialCut_ThetaPhi.h DC_FiducialCut_XY.h HadronChi2PID.h PhotonIDRefinementCut.h LINKDEF RgaLinkDef.h)

add_library(${CHANZER} SHARED ZVertexCut.cpp FiducialCutPCAL_uvw.cpp DC_FiducialCut_ThetaPhi.cpp DC_FiducialCut_XY.cpp HadronChi2PID.cpp PhotonIDRefinementCut.cpp G__${CHANZER}.cxx)

target_link_libraries(${CHANZER} -lClas12Banks)
target_link_libraries(${CHANZER} ${ROOT_LIBRARIES} chanser)
target_link_libraries(${CHANZER} ${ROOT_LIBRARIES} chansermva)


install(TARGETS ${CHANZER} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_CHANZER_PCM ${CMAKE_BINARY_DIR}/*pcm ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)

install (FILES  ${ROOT_CHANZER_PCM}    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")


