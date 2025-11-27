# tmva/CMakeLists.txt - adjust to create a target and link dependencies

set(MODULE_NAME chansermva)

# 1. Gather Sources
file(GLOB_RECURSE TMVA_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# 2. Gather Headers and Handle LinkDef
file(GLOB TMVA_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(LINKDEF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/MvaLinkDef.h")

# Exclude LinkDef from header list so it isn't passed twice to rootcling
if(EXISTS ${LINKDEF_FILE})
  list(REMOVE_ITEM TMVA_HEADERS ${LINKDEF_FILE})
else()
  # It is fatal if we intend to build a dictionary but lack the definition file
  message(FATAL_ERROR "tmva/LinkDef.h not found! Please create it.")
endif()

# 3. Generate Dictionary
if(COMMAND ROOT_GENERATE_DICTIONARY)
  message(STATUS "Generating ROOT dictionary for ${MODULE_NAME}...")
  
  # FIX: Added OPTIONS -I... so rootcling can find headers in 'core'
  ROOT_GENERATE_DICTIONARY(G__${MODULE_NAME}
    ${TMVA_HEADERS}
    LINKDEF ${LINKDEF_FILE}
    OPTIONS -I${CMAKE_SOURCE_DIR}/core
  )
  # Add the generated dictionary source to the build list
  list(APPEND TMVA_SRCS "${CMAKE_CURRENT_BINARY_DIR}/G__${MODULE_NAME}.cxx")
endif()

# 4. Create Library
add_library(${MODULE_NAME} SHARED ${TMVA_SRCS})

# 5. Link Dependencies
# Link to the core library (gives access to symbols)
target_link_libraries(${MODULE_NAME} PRIVATE chanser_core)

# 6. Include Directories
target_include_directories(${MODULE_NAME}
  PUBLIC
    # Add current dir so dictionary generator finds the headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_SOURCE_DIR}/core  # This allows finding "FiledTree.h" during compilation
)

target_link_libraries(${MODULE_NAME} PRIVATE chanser::root_deps)

if(TARGET chanser::rcdb_deps)
  target_link_libraries(${MODULE_NAME} PRIVATE chanser::rcdb_deps)
endif()

# 7. Installation
install(TARGETS ${MODULE_NAME}
  EXPORT chanserTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install Headers
#install(FILES ${TMVA_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install PCM (Required for ROOT 6 dictionaries)
file(GLOB MODULE_PCM "${CMAKE_CURRENT_BINARY_DIR}/*${MODULE_NAME}*pcm" "${CMAKE_CURRENT_BINARY_DIR}/*${MODULE_NAME}*rootmap")
if(MODULE_PCM)
  install(FILES ${MODULE_PCM} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
