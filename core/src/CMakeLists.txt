# core/src/CMakeLists.txt

# 1. Define the list of programs (names must match .cpp files exactly)
set(EXEC_PROGRAMS
  chanser
  chanser_proof
  chanser_farm
  chanser_mva
  chanser_skeleton
)

# 2. Loop through and create targets
foreach(prog IN LISTS EXEC_PROGRAMS)
  
  # Clean potential whitespace (just in case)
  string(STRIP "${prog}" prog)

  message(STATUS "Configuring executable target: ${prog}")

  # We assume the file exists. If not, CMake will error here with the full path.
  add_executable(${prog} ${prog}.cpp)

  # Link to the core library
  if(TARGET chanser_core)
    target_link_libraries(${prog} PRIVATE chanser_core)
  else()
    # Fallback if library failed to configure (shouldn't happen)
    message(WARNING "Target chanser_core not found for ${prog}")
    target_include_directories(${prog} PRIVATE ${CMAKE_SOURCE_DIR}/core)
  endif()

  # Link ROOT dependencies
  if(TARGET chanser::root_deps)
    target_link_libraries(${prog} PRIVATE chanser::root_deps)
  endif()

  # Include directories
  target_include_directories(${prog} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  )

  # Install instruction
  install(TARGETS ${prog}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

endforeach()
