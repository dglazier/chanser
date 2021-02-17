set(CHANZER chanseractions_rga)
set (CLAS12ROOT $ENV{CLAS12ROOT})
set (HIPO $ENV{HIPO})
set(DIRNAME rga_actions)
set (CHANSERBASE $ENV{CHANSER})


include_directories(${CLAS12ROOT}/Clas12Root)
include_directories(${CLAS12ROOT}/Clas12Banks)
include_directories(${CLAS12ROOT}/hipo4)
include_directories(core)
include_directories(tmva)
include_directories(actions)

ROOT_GENERATE_DICTIONARY(G__${CHANZER} Cut_ZVertex.h FiducialCut_PCAL_uvw.h FiducialCut_DC_ThetaPhi.h FiducialCut_DC_XY.h Cut_PionChi2Pid.h Cut_RefinePhotonID.h Cut_RefineElectronID.h LINKDEF RgaLinkDef.h)

add_library(${CHANZER} SHARED Cut_ZVertex.cpp FiducialCut_PCAL_uvw.cpp FiducialCut_DC_ThetaPhi.cpp FiducialCut_DC_XY.cpp Cut_PionChi2Pid.cpp Cut_RefinePhotonID.cpp Cut_RefineElectronID.cpp G__${CHANZER}.cxx)

target_link_libraries(${CHANZER} -lClas12Banks)
target_link_libraries(${CHANZER} -lClas12Root)
target_link_libraries(${CHANZER} ${ROOT_LIBRARIES} chanser)
target_link_libraries(${CHANZER} ${ROOT_LIBRARIES} chansermva)


install(TARGETS ${CHANZER} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install (FILES  ${CMAKE_CURRENT_BINARY_DIR}/libchanseractions_rga_rdict.pcm
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")
