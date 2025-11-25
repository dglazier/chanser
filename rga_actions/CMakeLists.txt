# rga_actions/CMakeLists.txt - create a library and link deps

file(GLOB_RECURSE RGA_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_library(chanser_rga_actions SHARED ${RGA_SRCS})

# 1. Link to the core library (gives access to symbols)
target_link_libraries(chanser_rga_actions PRIVATE chanser_core chanser_actions)

# 2. Add the path to the core headers
target_include_directories(chanser_rga_actions
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_SOURCE_DIR}/core  # This allows finding "FiledTree.h"
    ${CMAKE_SOURCE_DIR}/actions  # This allows finding "BaseCut.h"
)

target_include_directories(chanser_rga_actions
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(chanser_rga_actions PRIVATE chanser::root_deps)
if(TARGET chanser::rcdb_deps)
  target_link_libraries(chanser_rga_actions PRIVATE chanser::rcdb_deps)
endif()

install(TARGETS chanser_rga_actions
  EXPORT chanserTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
#install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
