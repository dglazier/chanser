set(SKEL skeleton)
set(DIRNAME skeleton)


ROOT_GENERATE_DICTIONARY(G__${SKEL}  Skeleton.h FSSkeleton.h SkeletonClass.h LINKDEF SkeletonLinkDef.h)

add_library(${SKEL} SHARED Skeleton.cpp FSSkeleton.cpp SkeletonClass.cpp G__${SKEL}.cxx)


install(TARGETS ${SKEL} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install (FILES  ${CMAKE_CURRENT_BINARY_DIR}/libskeleton_rdict.pcm 
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")
