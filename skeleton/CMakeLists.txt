set(SKEL skeleton)
set(DIRNAME skeleton)


ROOT_GENERATE_DICTIONARY(G__${SKEL}  Skeleton.h FSSkeleton.h SkeletonClass.h LINKDEF SkeletonLinkDef.h)

add_library(${SKEL} SHARED Skeleton.cpp FSSkeleton.cpp SkeletonClass.cpp G__${SKEL}.cxx)


install(TARGETS ${SKEL} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_SKEL_PCM ${CMAKE_BINARY_DIR}/${DIRNAME}/*pcm)
#FILE(GLOB ROOT_SKEL_PCM ${CMAKE_BINARY_DIR}/*pcm)

install (FILES  ${ROOT_SKEL_PCM}    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")
