add_executable (chanser_proof chanser_proof.cpp)
target_link_libraries(chanser_proof ${ROOT_LIBRARIES})
add_executable (chanser_root chanser.cpp)
target_link_libraries(chanser_root ${ROOT_LIBRARIES})

install (TARGETS chanser_root chanser_proof DESTINATION ${CMAKE_INSTALL_BINDIR})
