# this adds the current directory the list of paths to search for cmake modules
# this way cmake uses "FindSFML.cmake" to pick up the SFML libraries
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

# windows users - set the path to your sfml install here
list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\SFML-2.2")

# this will make your code link against static sfml .lib's
# the build time can be a little slower, but you won't have to copy dll's around to run the exe
set( SFML_STATIC_LIBRARIES TRUE )

# if you want to use any other sfml modules (e.g. audio) add them here
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

find_package(OpenGL REQUIRED)