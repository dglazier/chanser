add_executable (chanzer_proof chanzer_proof.cpp)
target_link_libraries(chanzer_proof ${ROOT_LIBRARIES})
add_executable (chanzer_root chanzer.cpp)
target_link_libraries(chanzer_root ${ROOT_LIBRARIES})

install (TARGETS chanzer_root chanzer_proof DESTINATION ${CMAKE_INSTALL_BINDIR})
