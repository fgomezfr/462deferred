set( SRCS "scene.cpp" "objmodel.cpp")
set( INCS "scene.hpp" "objmodel.hpp")

add_library(scene ${SRCS} ${INCS})
source_group(headers FILES ${INCS})