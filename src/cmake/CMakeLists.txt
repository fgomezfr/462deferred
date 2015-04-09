# this adds the current directory the list of paths to search for cmake modules
# this way cmake uses "FindSFML.cmake" to pick up the SFML libraries
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

# windows users - set the path to your install here if necessary
if ( WIN32 )
list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\SFML")
endif()

# this will make your code link against static sfml .lib's
# the build time can be a little slower, but you won't have to copy dll's around to run the exe
set( SFML_STATIC_LIBRARIES TRUE )

# if you want to use any other sfml modules (e.g. audio) add them here
find_package(SFML 2.2 COMPONENTS graphics window system REQUIRED)

# here we link to the latest (as of 4/9/2015) glew32s.lib on Windows systems
# this is included only to expose OpenGL1.2+ functionality, which is not included in gl.h
# if you want to make use of glew in your renderer, you should add a full path search for
# glew libs on other platforms, and make sure it is always included from the code
set( GLEW_LIBRARIES "" )
if ( WIN32 )

	list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/GLEW )
	list(APPEND CMAKE_LIBRARY_PATH ${CMAKE_CURRENT_LIST_DIR}/GLEW/lib )
	find_package( GLEW REQUIRED )

	if ( GLEW_FOUND )
		include_directories(${GLEW_INCLUDE_DIRS})
		add_definitions( -DGLEW_STATIC )
	endif()

endif()

if ( SFML_FOUND )
	include_directories(${SFML_INCLUDE_DIR})
endif()

find_package(OpenGL REQUIRED)
find_package(Threads)