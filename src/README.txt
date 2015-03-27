CMU 15462 Deferred Rendering Starter Code
@author Felipe Gomez-Frittelli (fgomezfr@andrew.cmu.edu)

This code is provided as-is for students starting 3D rendering projects in OpenGL.
You are not required to use this code for 15-462, but it may save you some time
if you are not familiar with window management, linking against OpenGL, and .obj
file parsing.

The code is divided into four projects, as a hint for organizing your renderer:

application/
	main.cpp - the main program; opens a window and starts rendering
	           add any necessary user input and event handling here

scene/
	scene.cpp - the scene representation, including lights and .obj models
	objmodel.cpp - a raw memory dump of selected data from .obj and .mtl files

	Very basic parsing of .scene, .obj, and .mtl files is provided in these classes.
	You can replace or augment this to handle extensions to the scene format or
	other .obj / .mtl parameters not targeted by the basic project requirements.

	We recommend that you keep all scene data separate from rendering code; a scene
	is just information about a renderable environment, without the tools or techniques
	to render itself. Keep OpenGL code in renderer - this will make it easier to
	iterate on rendering without changing scene code, and allows you to expand to
	handle other scene types, multiple scenes in one application, different renderers
	applied to the same scene, etc.


renderer/
	renderer.cpp - a skeleton file for your renderer

	No real code here, just some stubs for suggested organization. It's a good
	technique to build a 'renderer' class that encapsulates the code for rendering
	a given scene, and then apply that code to your scene.

glm/
	The GLM math libraries: http://glm.g-truc.net/0.9.6/index.html

	You will want to explore the online documentation; there are many tutorials for
	common graphics operations, and a full code reference. If you want to push
	performance in your renderer, you can configure glm to compile to SIMD intrinsics.

cmake/
	FindSFML.cmake - a cmake module used to find the installed SFML libraries

This code relies on SFML for window management, keyboard input, and image I/O.
Before you can build and run the code, you need to download and install the package
for your OS:

	http://www.sfml-dev.org/download.php

Download the latest stable version (2.2 as of 3/26/2015); Windows users should install
the 32-bit version (just extract the files to where you want them installed), unless
you have a reason to build a 64-bit renderer exe. Linux users may be able to get SFML
through your package manager; if not (or if you are working on a GHC machine) you may
need to download and build SFML from source. You can install to your ~/public space on
AFS, or store files in /tmp on most GHC machines.

Once SFML is installed, change the value of CMAKE_PREFIX_PATH in cmake/CMakeLists.txt
to the directory where you installed SFML. You can then run CMake on the top-level /src
directory to generate build files for your platform. Windows users should use the
cmake-gui application, and change the value of CMAKE_INSTALL_PATH to where you want
your exe to be output (a folder from which you know the path to the scene files).

If you add source files to your project, don't forget to edit the CMakeLists.txt file
in the corresponding directory; CMake will automatically re-generate the build files.

A note on linking: The cmake settings provided here will try to link against the static
SFML libraries. If you encounter problems with this, you can set SFML_STATIC_LIBRARIES
to FALSE in the cmake/CMakeLists.txt file to link against dynamic libraries. This will
require you to copy the .dll's to wherever your exe runs from (especially if you are
running inside the Visual Studio debugger).

Ideally, your code should build on any platform using CMake and SFML. If you choose not
to use this code, please stick with SFML if at all possible. If you really really want
to use a different window library, it should build using only the files you submit. If
you encounter any other issues with the starter code, contact course staff via Piazza
and we will try to push a solution to Github and the course website as soon as possible.