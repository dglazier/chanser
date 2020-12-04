cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(chanser)

	
set(CMAKE_INSTALL_BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_CXX_FLAGS "-fPIC -O3")


list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}) ############USEROOTSYS

find_package(ROOT REQUIRED COMPONENTS Proof)  ###########USEROOTSYS

#give clas12root include dir
set(CLAS12ROOT_DIR $ENV{CLAS12ROOT}/Clas12Root)
include_directories(${CLAS12ROOT_DIR})

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE}) ##########USEROOTSYS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tmva)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/tmva)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/actions)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/actions)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rga_actions)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/rga_actions)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/skeleton)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/skeleton)

