add_executable(p4 main.cpp)
target_link_libraries(p4 scene renderer ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_LIBRARIES} pthread)
install(TARGETS p4 DESTINATION ${PROJECT_SOURCE_DIR}/..)
