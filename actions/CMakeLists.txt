# actions/CMakeLists.txt - library/executable layout

file(GLOB_RECURSE ACTIONS_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_library(chanser_actions SHARED ${ACTIONS_SRCS})

# 1. Link to the core library (gives access to symbols)
target_link_libraries(chanser_actions PRIVATE chanser_core chanser_tmva)

# 2. Add the path to the core headers
target_include_directories(chanser_actions
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_SOURCE_DIR}/core  # This allows finding "FiledTree.h"
    ${CMAKE_SOURCE_DIR}/tmva  # This allows finding "ResultInterface.h"
)

target_include_directories(chanser_actions
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(chanser_actions PRIVATE chanser::root_deps)
if(TARGET chanser::clasqadb_deps)
  target_link_libraries(chanser_actions PRIVATE chanser::clasqadb_deps)
endif()

install(TARGETS chanser_actions
  EXPORT chanserTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
