set( SRCS "renderer.cpp" "camera.cpp")
set( INCS "renderer.hpp" "camera.hpp")

add_library(renderer ${SRCS} ${INCS})
source_group(headers FILES ${INCS})