# core/CMakeLists.txt
set(CHANZER chanser)
set(DIRNAME core)

set(CLAS12ROOT $ENV{CLAS12ROOT})

# ==============================================================================
# 1. Find Dependencies
# ==============================================================================

# Find Clas12Banks
find_library(CLAS12_BANKS_LIB NAMES Clas12Banks
  HINTS ${CLAS12ROOT}
  PATH_SUFFIXES lib lib64 lib32
)
if(NOT CLAS12_BANKS_LIB)
  message(FATAL_ERROR "Could not find Clas12Banks library in ${CLAS12ROOT}")
endif()

# Find Clas12Root
find_library(CLAS12_ROOT_LIB NAMES Clas12Root
  HINTS ${CLAS12ROOT}
  PATH_SUFFIXES lib lib64 lib32
)
if(NOT CLAS12_ROOT_LIB)
  message(FATAL_ERROR "Could not find Clas12Root library in ${CLAS12ROOT}. Check your CLAS12ROOT path.")
endif()

# Find Clas12Root-Hipo4
find_library(HIPO4_ROOT_LIB NAMES Hipo4
  HINTS ${CLAS12ROOT}
  PATH_SUFFIXES lib lib64 lib32
)
if(NOT HIPO4_ROOT_LIB)
  message(FATAL_ERROR "Could not find Clas12Root/Hipo4 library in ${CLAS12ROOT}. Check your CLAS12ROOT path. You may need to configure Clas12Root cmake build with cmake -DWITH_HIPO4_WRAPPER=ON ")
endif()

# ==============================================================================
# 2. Source & Header Gathering
# ==============================================================================
file(GLOB CORE_ROOT_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.C"
)

# FIX: Glob all headers...
file(GLOB DICTIONARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# ... THEN identifying the LinkDef file ...
set(LINKDEF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChanserLinkDef.h")

# ... AND REMOVING IT from the header list so it doesn't get passed twice.
if(EXISTS ${LINKDEF_FILE})
  list(REMOVE_ITEM DICTIONARY_HEADERS ${LINKDEF_FILE})
else()
  message(FATAL_ERROR "LinkDef file not found at: ${LINKDEF_FILE}")
endif()

# ==============================================================================
# 3. ROOT Dictionary Generation
# ==============================================================================

# Force load ROOT macros
if(NOT COMMAND ROOT_GENERATE_DICTIONARY)
  if(EXISTS "$ENV{ROOTSYS}/cmake/modules/RootNewMacros.cmake")
    include("$ENV{ROOTSYS}/cmake/modules/RootNewMacros.cmake")
  elseif(EXISTS "${ROOTSYS}/cmake/modules/RootNewMacros.cmake")
    include("${ROOTSYS}/cmake/modules/RootNewMacros.cmake")
  endif()
endif()

if(COMMAND ROOT_GENERATE_DICTIONARY)
  message(STATUS "Generating ROOT dictionary for ${CHANZER}...")
  
  ROOT_GENERATE_DICTIONARY(G__${CHANZER}
    ${DICTIONARY_HEADERS}
    LINKDEF ${LINKDEF_FILE}
  )
  list(APPEND CORE_ROOT_SRCS "${CMAKE_CURRENT_BINARY_DIR}/G__${CHANZER}.cxx")
else()
  message(FATAL_ERROR "ROOT macros not found. Cannot generate dictionary.")
endif()

# ==============================================================================
# 4. Target Definition
# ==============================================================================
if(NOT CORE_ROOT_SRCS)
  message(WARNING "core: no root-level source files found; skipping chanser_core target")
else()
  add_library(chanser_core SHARED ${CORE_ROOT_SRCS})
  set_target_properties(chanser_core PROPERTIES OUTPUT_NAME ${CHANZER})

  # --- Includes ---
  target_include_directories(chanser_core
    PUBLIC
      # Include current dir so generated dictionary finds headers
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/actions
      ${CMAKE_CURRENT_SOURCE_DIR}/tmva
  )

  if(CLAS12ROOT)
     target_include_directories(chanser_core PUBLIC 
        ${CLAS12ROOT}/Clas12Banks 
        ${CLAS12ROOT}/Clas12Root 
        ${CLAS12ROOT}/hipo4 
     )
  endif()

  # --- Linking ---
  if(TARGET hipo::hipo)
    target_link_libraries(chanser_core PUBLIC hipo::hipo)
  endif()

  if(TARGET chanser::root_deps)
    target_link_libraries(chanser_core PRIVATE chanser::root_deps)
  endif()

  target_link_libraries(chanser_core PUBLIC ${CLAS12_BANKS_LIB} ${CLAS12_ROOT_LIB} ${HIPO4_ROOT_LIB})

  # --- Installation ---
  install(TARGETS chanser_core
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  #FILE(GLOB ROOT_CHANZER_PCM ${CMAKE_BINARY_DIR}/*pcm ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)
  file(GLOB ROOT_CHANZER_PCM "${CMAKE_CURRENT_BINARY_DIR}/*${MODULE_NAME}*pcm" "${CMAKE_CURRENT_BINARY_DIR}/*${MODULE_NAME}*rootmap")
  if(ROOT_CHANZER_PCM)
    install(FILES ${ROOT_CHANZER_PCM} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
  endif()
endif()

add_subdirectory(src)
