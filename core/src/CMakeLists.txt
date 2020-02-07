add_executable (chanzer_proof chanzer_proof.cpp)
target_link_libraries(chanzer_proof ${ROOT_LIBRARIES})

install (TARGETS chanzer_proof DESTINATION ${CMAKE_INSTALL_BINDIR})
