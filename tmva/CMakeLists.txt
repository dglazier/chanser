# tmva/CMakeLists.txt - adjust to create a target and link dependencies

file(GLOB_RECURSE TMVA_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_library(chanser_tmva SHARED ${TMVA_SRCS})

# 1. Link to the core library (gives access to symbols)
target_link_libraries(chanser_tmva PRIVATE chanser_core)

# 2. Add the path to the core headers
target_include_directories(chanser_tmva
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_SOURCE_DIR}/core  # This allows finding "FiledTree.h"
)

target_include_directories(chanser_tmva
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(chanser_tmva PRIVATE chanser::root_deps)
if(TARGET chanser::rcdb_deps)
  target_link_libraries(chanser_tmva PRIVATE chanser::rcdb_deps)
endif()

install(TARGETS chanser_tmva
  EXPORT chanserTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
#install(FILES ${TMVA_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
