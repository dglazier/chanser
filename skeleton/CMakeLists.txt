# skeleton/CMakeLists.txt - library target renamed to avoid conflict with executable chanser_skeleton

file(GLOB_RECURSE SKEL_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*Skeleton*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*Skeleton*.cc"
)

if(NOT SKEL_SRCS)
  message(STATUS "skeleton: no source files found in ${CMAKE_CURRENT_SOURCE_DIR}; skipping chanser_skeleton_lib target")
else()
  # Unique library target name to avoid colliding with executable 'chanser_skeleton'
  add_library(chanser_skeleton_lib SHARED ${SKEL_SRCS})
  # 1. Link to the core library (gives access to symbols)
  target_link_libraries(chanser_skeleton_lib PRIVATE chanser_core)

  # 2. Add the path to the core headers
  target_include_directories(chanser_skeleton_lib
   PUBLIC
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
       PRIVATE
    ${CMAKE_SOURCE_DIR}/core  # This allows finding "FiledTree.h"
  )

  # Optional: keep the on-disk library base name as before
  set_target_properties(chanser_skeleton_lib PROPERTIES OUTPUT_NAME "chanser_skeleton")

  target_include_directories(chanser_skeleton_lib
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )
  target_link_libraries(chanser_skeleton_lib PRIVATE chanser::root_deps)

  install(TARGETS chanser_skeleton_lib
    EXPORT chanserTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
 # install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
