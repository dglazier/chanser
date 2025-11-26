# rga_actions/CMakeLists.txt - create a library and link deps
set(MODULE_NAME chanser_rga_actions)

file(GLOB_RECURSE RGA_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# --- DICTIONARY SETUP START ---
file(GLOB MODULE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(LINKDEF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/RgaLinkDef.h")

if(EXISTS ${LINKDEF_FILE})
  list(REMOVE_ITEM MODULE_HEADERS ${LINKDEF_FILE})
else()
  # Fatal because we need the dictionary for this module
  message(FATAL_ERROR "rga_actions/RgaLinkDef.h not found! Please create it.")
endif()

if(COMMAND ROOT_GENERATE_DICTIONARY)
  message(STATUS "Generating ROOT dictionary for ${MODULE_NAME}...")
  
  # FIX: Include core and actions paths so rootcling can find base classes
  ROOT_GENERATE_DICTIONARY(G__${MODULE_NAME}
    ${MODULE_HEADERS}
    LINKDEF ${LINKDEF_FILE}
    OPTIONS -I${CMAKE_SOURCE_DIR}/core -I${CMAKE_SOURCE_DIR}/actions
  )
  list(APPEND RGA_SRCS "${CMAKE_CURRENT_BINARY_DIR}/G__${MODULE_NAME}.cxx")
endif()
# --- DICTIONARY SETUP END ---

add_library(chanser_rga_actions SHARED ${RGA_SRCS})

# 1. Link to the core library (gives access to symbols)
target_link_libraries(chanser_rga_actions PRIVATE chanser_core chanser_actions)

# 2. Add the path to the core headers
target_include_directories(chanser_rga_actions
  PUBLIC
    # FIX: Add current dir so dictionary compiles correctly
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
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

# --- INSTALLATION ADDITIONS ---
# We do NOT install headers (as requested), but we MUST install the PCM
# for the dictionary to function at runtime.
file(GLOB MODULE_PCM ${CMAKE_BINARY_DIR}/*${MODULE_NAME}*pcm)
if(MODULE_PCM)
  install(FILES ${MODULE_PCM} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
