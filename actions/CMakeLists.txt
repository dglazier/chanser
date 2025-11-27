# actions/CMakeLists.txt - library/executable layout
set(MODULE_NAME chanseractions)

file(GLOB_RECURSE ACTIONS_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)
list(REMOVE_ITEM ACTIONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/TemplateAction.cpp")

# --- DICTIONARY SETUP START ---
file(GLOB MODULE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(LINKDEF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ActionsLinkDef.h")
list(REMOVE_ITEM MODULE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/TemplateAction.h")

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

add_library(${MODULE_NAME} SHARED ${ACTIONS_SRCS})

# 1. Link to the core library (gives access to symbols)
target_link_libraries(${MODULE_NAME} PRIVATE chanser chansermva)

# 2. Add the path to the core headers
target_include_directories(${MODULE_NAME}
  PUBLIC
    # FIX: Add current dir so dictionary compiles correctly
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_SOURCE_DIR}/core  # This allows finding "FiledTree.h"
    ${CMAKE_SOURCE_DIR}/tmva  # This allows finding "ResultInterface.h"
)

target_include_directories(${MODULE_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(${MODULE_NAME} PRIVATE chanser::root_deps)
if(TARGET chanser::clasqadb_deps)
  target_link_libraries(${MODULE_NAME} PRIVATE chanser::clasqadb_deps)
endif()

install(TARGETS ${MODULE_NAME}
  EXPORT chanserTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# --- INSTALLATION ADDITIONS ---
##install(FILES ${MODULE_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file(GLOB MODULE_PCM "${CMAKE_CURRENT_BINARY_DIR}/*${MODULE_NAME}*pcm" "${CMAKE_CURRENT_BINARY_DIR}/*${MODULE_NAME}*rootmap")
if(MODULE_PCM)
  install(FILES ${MODULE_PCM} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
