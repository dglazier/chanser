# core/CMakeLists.txt
set(CHANZER chanser)
set(DIRNAME core)

set(CLAS12ROOT $ENV{CLAS12ROOT})

# ==============================================================================
# 1. Find Dependencies (The Fix for Linker Errors)
# ==============================================================================

# Find Clas12Banks (Existing)
find_library(CLAS12_BANKS_LIB NAMES Clas12Banks
  HINTS ${CLAS12ROOT}
  PATH_SUFFIXES lib lib64 lib32
)
if(NOT CLAS12_BANKS_LIB)
  message(FATAL_ERROR "Could not find Clas12Banks library in ${CLAS12ROOT}")
endif()

# Find Clas12Root (NEW: Required for HipoChain/HipoSelector errors)
find_library(CLAS12_ROOT_LIB NAMES Clas12Root
  HINTS ${CLAS12ROOT}
  PATH_SUFFIXES lib lib64 lib32
)
if(NOT CLAS12_ROOT_LIB)
  message(FATAL_ERROR "Could not find Clas12Root library in ${CLAS12ROOT}. Check your CLAS12ROOT path.")
endif()

# ==============================================================================
# 2. Source Gathering
# ==============================================================================
file(GLOB CORE_ROOT_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.C"
)

# ==============================================================================
# 3. ROOT Dictionary (The Fix for Streamer/Class errors)
# ==============================================================================

# FORCE load ROOT macros if the environment didn't do it
if(NOT COMMAND ROOT_GENERATE_DICTIONARY)
  if(EXISTS "$ENV{ROOTSYS}/cmake/modules/RootNewMacros.cmake")
    include("$ENV{ROOTSYS}/cmake/modules/RootNewMacros.cmake")
  elseif(EXISTS "${ROOTSYS}/cmake/modules/RootNewMacros.cmake")
    include("${ROOTSYS}/cmake/modules/RootNewMacros.cmake")
  endif()
endif()

# Define headers for dictionary generation
set(DICTIONARY_HEADERS
    Archive.h AnaDB.h Loader.h FiledTree.h BaseOutEvent.h BaseRunInfo.h RunInfo.h
    BaseEventInfo.h EventInfo.h BaseParticle.h CLAS12Particle.h DataManager.h
    HipoData.h HipoTruth.h ParticleConfig.h
)

if(COMMAND ROOT_GENERATE_DICTIONARY)
  # IMPORTANT: Ensure this file exists! I am assuming it is named LinkDef.h
  # If you named it ChanserLinkDef.h, change the line below.
  set(LINKDEF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChanserLinkDef.h")

  if(NOT EXISTS ${LINKDEF_FILE})
     message(FATAL_ERROR "LinkDef file missing at: ${LINKDEF_FILE}")
  endif()

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
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/actions
      ${CMAKE_CURRENT_SOURCE_DIR}/tmva
  )

  if(CLAS12ROOT)
     # Added Clas12Root and hipo4 to includes
     target_include_directories(chanser_core PUBLIC 
        ${CLAS12ROOT}/Clas12Banks 
        ${CLAS12ROOT}/Clas12Root 
        ${CLAS12ROOT}/hipo4 
     )
  endif()

  # --- Linking ---
  
  # Link HIPO (Public because headers might expose HIPO types)
  if(TARGET hipo::hipo)
    target_link_libraries(chanser_core PUBLIC hipo::hipo)
  endif()

  # Link ROOT
  if(TARGET chanser::root_deps)
    target_link_libraries(chanser_core PRIVATE chanser::root_deps)
  endif()

  # Link CLAS12 (Both Banks AND Root)
  target_link_libraries(chanser_core PUBLIC ${CLAS12_BANKS_LIB} ${CLAS12_ROOT_LIB})


  # --- Installation ---
  install(TARGETS chanser_core
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  FILE(GLOB ROOT_CHANZER_PCM ${CMAKE_BINARY_DIR}/*pcm ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)
  if(ROOT_CHANZER_PCM)
    install(FILES ${ROOT_CHANZER_PCM} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
  endif()
endif()

# Build binaries in core/src
add_subdirectory(src)
