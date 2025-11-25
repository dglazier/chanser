cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(chanser VERSION 0.1 LANGUAGES CXX)

# Modern CMake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ==============================================================================
# 1. Install Path Configuration (Must be before GNUInstallDirs)
# ==============================================================================
# Set default install path to the Project Root (Source Directory)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE PATH "Default install prefix" FORCE)
endif()

include(GNUInstallDirs)

# Force relative paths for lib/bin so they install into ${CMAKE_INSTALL_PREFIX}/lib
#set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Install lib dir" FORCE)
#set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "Install bin dir" FORCE)
#set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Install include dir" FORCE)

# ==============================================================================
# 2. Library Extension Detection
# ==============================================================================
if(DEFINED LIBRARY_EXT)
  set(LIBRARY_EXT "${LIBRARY_EXT}")
elseif(DEFINED ENV{LIB_EXT})
  set(LIBRARY_EXT "$ENV{LIB_EXT}")
elseif(DEFINED CMAKE_SHARED_LIBRARY_SUFFIX)
  set(LIBRARY_EXT "${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
  if(APPLE)
    set(LIBRARY_EXT ".dylib")
  else()
    set(LIBRARY_EXT ".so")
  endif()
endif()
message(STATUS "Using shared library extension: ${LIBRARY_EXT}")

# ==============================================================================
# 3. Dependencies
# ==============================================================================

# --- ROOT ---
if(DEFINED ENV{ROOTSYS})
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
endif()

find_package(ROOT REQUIRED COMPONENTS Proof Rint Core Hist Tree)

add_library(chanser_root_deps INTERFACE)
if(TARGET ROOT::Proof)
  target_link_libraries(chanser_root_deps INTERFACE 
    ROOT::Proof 
    ROOT::Rint 
    ROOT::Core 
    ROOT::Hist 
    ROOT::Tree
    ROOT::EG      # Added
    ROOT::Physics # Added
  )
else()
  if(DEFINED ROOT_LIBRARIES)
    target_link_libraries(chanser_root_deps INTERFACE ${ROOT_LIBRARIES})
  endif()
  if(DEFINED ROOT_INCLUDE_DIRS)
    target_include_directories(chanser_root_deps INTERFACE ${ROOT_INCLUDE_DIRS})
  endif()
endif()
add_library(chanser::root_deps ALIAS chanser_root_deps)

# --- HIPO4 (Moved up so subdirectories can find it) ---
message(STATUS "Checking for HIPO4 library...")
if(DEFINED ENV{HIPO})
  set(HIPO_INSTALL_DIR $ENV{HIPO})
  message(STATUS "Found HIPO4 at: ${HIPO_INSTALL_DIR}")

  add_library(hipo::hipo INTERFACE IMPORTED)
  target_include_directories(hipo::hipo INTERFACE ${HIPO_INSTALL_DIR}/include)
   
  set(HIPO_LIB "${HIPO_INSTALL_DIR}/lib/libhipo4${LIBRARY_EXT}")
  if(NOT EXISTS "${HIPO_LIB}")
      message(FATAL_ERROR "Could not find hipo4 library at ${HIPO_INSTALL_DIR}/lib")
  endif()

  target_link_libraries(hipo::hipo INTERFACE ${HIPO_LIB})
else()
  message(FATAL_ERROR "HIPO environment variable not set.")
endif()

# --- Optional Deps (RCDB, CCDB, etc) ---
if(DEFINED ENV{RCDB_HOME})
  add_library(chanser_rcdb_deps INTERFACE)
  target_include_directories(chanser_rcdb_deps INTERFACE $ENV{RCDB_HOME}/cpp/include)
  find_path(MYSQL_INCLUDE_DIR mysql.h /usr/local/include/mysql /usr/include/mysql)
  if(MYSQL_INCLUDE_DIR)
    target_include_directories(chanser_rcdb_deps INTERFACE ${MYSQL_INCLUDE_DIR})
  endif()
  add_library(chanser::rcdb_deps ALIAS chanser_rcdb_deps)
endif()

if(DEFINED ENV{CCDB_HOME})
  add_library(chanser_ccdb_deps INTERFACE)
  target_include_directories(chanser_ccdb_deps INTERFACE $ENV{CCDB_HOME}/include)
  add_library(chanser::ccdb_deps ALIAS chanser_ccdb_deps)
endif()

if(DEFINED ENV{CLASQADB_HOME})
  add_library(chanser_clasqadb_deps INTERFACE)
  target_include_directories(chanser_clasqadb_deps INTERFACE $ENV{CLASQADB_HOME}/srcC/include)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--allow-multiple-definition")
  add_library(chanser::clasqadb_deps ALIAS chanser_clasqadb_deps)
endif()

# ==============================================================================
# 4. Add Subdirectories
# ==============================================================================
# These contain their own CMakeLists.txt. 
# core/src/CMakeLists.txt handles the executables.
add_subdirectory(core)
add_subdirectory(tmva)
add_subdirectory(actions)
add_subdirectory(rga_actions)
add_subdirectory(skeleton)
