# actions/CMakeLists.txt - library/executable layout
set(MODULE_NAME chanser_actions)

file(GLOB_RECURSE ACTIONS_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# --- DICTIONARY SETUP START ---
file(GLOB MODULE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(LINKDEF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ActionsLinkDef.h")

if(EXISTS ${LINKDEF_FILE})
  list(REMOVE_ITEM MODULE_HEADERS ${LINKDEF_FILE})
else()
  message(FATAL_ERROR "actions/LinkDef.h not found! Please create it.")
endif()

if(COMMAND ROOT_GENERATE_DICTIONARY)
  message(STATUS "Generating ROOT dictionary for ${MODULE_NAME}...")
  
  # FIX: Include both core and tmva paths so rootcling can find base classes
  ROOT_GENERATE_DICTIONARY(G__${MODULE_NAME}
    ${MODULE_HEADERS}
    LINKDEF ${LINKDEF_FILE}
    OPTIONS -I${CMAKE_SOURCE_DIR}/core -I${CMAKE_SOURCE_DIR}/tmva
  )
  list(APPEND ACTIONS_SRCS "${CMAKE_CURRENT_BINARY_DIR}/G__${MODULE_NAME}.cxx")
endif()
# --- DICTIONARY SETUP END ---

add_library(chanser_actions SHARED ${ACTIONS_SRCS})

# 1. Link to the core library (gives access to symbols)
target_link_libraries(chanser_actions PRIVATE chanser_core chanser_tmva)

# 2. Add the path to the core headers
target_include_directories(chanser_actions
  PUBLIC
    # FIX: Add current dir so dictionary compiles correctly
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
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

# --- INSTALLATION ADDITIONS ---
##install(FILES ${MODULE_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file(GLOB MODULE_PCM ${CMAKE_BINARY_DIR}/*${MODULE_NAME}*pcm)
if(MODULE_PCM)
  install(FILES ${MODULE_PCM} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
