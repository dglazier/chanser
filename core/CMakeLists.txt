# core/CMakeLists.txt - modernized with unique target names

set(CHANZER chanser)
set(DIRNAME core)

set(CLAS12ROOT $ENV{CLAS12ROOT})
set(HIPO $ENV{HIPO})
set(CHANSERBASE $ENV{CHANSER})

if(DEFINED ROOT_USE_FILE)
  include(${ROOT_USE_FILE})
endif()

# Gather core root-level sources (exclude src/ subdir)
file(GLOB CORE_ROOT_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.C"
)

# Generate ROOT dictionary (adjust header list / LINKDEF as needed)
if(DEFINED ROOT_GENERATE_DICTIONARY)
  ROOT_GENERATE_DICTIONARY(G__${CHANZER}
    Archive.h AnaDB.h Loader.h FiledTree.h BaseOutEvent.h BaseRunInfo.h RunInfo.h
    BaseEventInfo.h EventInfo.h BaseParticle.h CLAS12Particle.h DataManager.h
    HipoData.h HipoTruth.h ParticleConfig.h
    LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/ChanserLinkDef.h
  )
  list(APPEND CORE_ROOT_SRCS "${CMAKE_CURRENT_BINARY_DIR}/G__${CHANZER}.cxx")
endif()

if(NOT CORE_ROOT_SRCS)
  message(WARNING "core: no root-level source files found; skipping chanser_core target")
else()
  # Use a unique CMake target name (chanser_core) to avoid colliding with executables named 'chanser'.
  add_library(chanser_core SHARED ${CORE_ROOT_SRCS})

  # Make the produced library file keep the old base name 'chanser' (libchanser.so)
  set_target_properties(chanser_core PROPERTIES OUTPUT_NAME ${CHANZER})

  if(TARGET hipo::hipo)
    target_link_libraries(chanser_core PUBLIC hipo::hipo)
  endif()

  if(CLAS12ROOT)
	# We use PUBLIC because CLAS12Particle.h (likely a public header) includes region_particle.h
  	target_include_directories(chanser_core PUBLIC ${CLAS12ROOT}/Clas12Banks ${CLAS12ROOT}/Clas12Root ${CLAS12ROOT}/hipo4 )
  endif()

target_include_directories(chanser_core
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/actions
      ${CMAKE_CURRENT_SOURCE_DIR}/tmva
  )

  if(TARGET chanser::root_deps)
    target_link_libraries(chanser_core PRIVATE chanser::root_deps)
  else()
    if(DEFINED ROOT_LIBRARIES)
      target_link_libraries(chanser_core PRIVATE ${ROOT_LIBRARIES})
    endif()
  endif()

  # 1. Find the library dynamically
  #    PATH_SUFFIXES tells CMake to look in lib, lib64, and lib32 inside CLAS12ROOT
  find_library(CLAS12_LIB NAMES Clas12Banks
   			  HINTS ${CLAS12ROOT}
   			  PATH_SUFFIXES lib lib64 lib32
   			  )

   if(CLAS12_LIB)
	message(STATUS "Found Clas12Banks: ${CLAS12_LIB}")
   else()
	message(FATAL_ERROR "Could not find Clas12Banks library in ${CLAS12ROOT} (checked lib, lib64, lib32).")
   endif()

   # ... setup target ...

   # 2. Link using the found variable
   if(CLAS12ROOT)
	target_link_libraries(chanser_core PRIVATE ${CLAS12_LIB})
   else()
   # Fallback for system install
     target_link_libraries(chanser_core PRIVATE -lClas12Banks)
   endif()

  install(TARGETS chanser_core
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  )

  FILE(GLOB ROOT_CHANZER_PCM ${CMAKE_BINARY_DIR}/*pcm ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)
  if(ROOT_CHANZER_PCM)
    install(FILES ${ROOT_CHANZER_PCM} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
  endif()
endif()

# Build binaries in core/src which link to chanser_core
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
