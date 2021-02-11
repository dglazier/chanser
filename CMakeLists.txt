cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(chanser)

	
set(CMAKE_INSTALL_BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_CXX_FLAGS "-fPIC -O3")


list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}) ############USEROOTSYS

find_package(ROOT REQUIRED COMPONENTS Proof)  ###########USEROOTSYS


#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE}) ##########USEROOTSYS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

IF (DEFINED ENV{RCDB_HOME})
  include_directories($ENV{RCDB_HOME}/cpp/include)
    #find path to mysql include directory
  FIND_PATH(MYSQL_INCLUDE_DIR mysql.h
    /usr/local/include/mysql
    /usr/include/mysql
    )
  include_directories(${MYSQL_INCLUDE_DIR})
ENDIF (DEFINED ENV{RCDB_HOME})

IF (DEFINED ENV{CCDB_HOME})
  # CCDB related
  include_directories($ENV{CCDB_HOME}/include)
ENDIF (DEFINED ENV{CCDB_HOME})
IF (DEFINED ENV{CLASQADB_HOME})
ENDIF (DEFINED ENV{CLASQADB_HOME})
IF (DEFINED ENV{CLASQADB_HOME})
  include_directories($ENV{CLASQADB_HOME}/srcC/include)
  #clasqaDB header contains function definitions which are not inlined
  #including this header causes multiple definitions of these functions
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -Wl,--allow-multiple-definition")
ENDIF (DEFINED ENV{CLASQADB_HOME})

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

