

###Setup DOXYGEN

find_package(Doxygen REQUIRED)

# Find all the public headers
get_target_property(CHANSER_PUBLIC_HEADER_DIR chanser INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE CHANSER_PUBLIC_HEADERS ${CHANSER_PUBLIC_HEADER_DIR}/*.h)



set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/core)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)
set(DOXYGEN_OUTPUT_XML ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen/xml)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#Replace variables inside @@ with the current values from Doxyfile.in
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) #Doxygen won't create this for us
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${CHANSER_PUBLIC_HEADERS}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating docs")

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})


#This will be the main output of our command
#set(DOXYGEN_INDEX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/html/index.html)

#add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
#                   DEPENDS ${CHANSER_PUBLIC_HEADERS}
#                   COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                   MAIN_DEPENDENCY Doxyfile
#                   COMMENT "Generating docs")

#add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

###Done DOXYGEN

###Now SPHINX
find_package(Sphinx REQUIRED)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs/sphinx)

add_custom_target(Sphinx ALL
                  COMMAND ${SPHINX_EXECUTABLE} -b html
                  # Tell Breathe where to find the Doxygen output
                  -Dbreathe_projects.chanser=${DOXYGEN_OUTPUT_XML}
                  ${SPHINX_SOURCE} ${SPHINX_BUILD}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Generating documentation with Sphinx")
