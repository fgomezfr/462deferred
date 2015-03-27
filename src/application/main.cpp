/*
 * This is the main file for your executable. It's best not to put much code here -
 * keep the overall program flow obvious, and let your Renderer class do the work.
 * You can add new command line arguments and event handling here though.
 */

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <string>
#include "../renderer/camera.hpp"
#include "../renderer/renderer.hpp"
#include "../scene/scene.hpp"

int main( int argc, char ** argv )
{
	// this defines the opengl context to be created for your window
	sf::ContextSettings contextSettings;

	// the depth + stencil buffer is 32 bits per pixel, split between the two uses
	contextSettings.depthBits = 32;
	contextSettings.stencilBits = 0; // you (probably) won't need the stencil buffer

	contextSettings.antialiasingLevel = 0; // DO NOT TURN ON AA WITH DEFERRED RENDERING
	
	// you can change the requested OpenGL version support here
	contextSettings.majorVersion = 3;
	contextSettings.minorVersion = 0;

	// create the window - you can change resolution, title, etc. here
	sf::Window window(sf::VideoMode(1280, 720), "P4 Deferred Renderer", sf::Style::Default, contextSettings);
	window.setVerticalSyncEnabled(true);


	// load the scene data - this may take a while for large scenes
	std::string filename;
	if ( argc > 1 )
	{
		filename = std::string( argv[argc - 1] );
	}
	else
	{
		sf::err() << "Error: Missing argument string (must provide a scene file)." << std::endl;
		window.close();
		return EXIT_FAILURE;
	}

	// setup the renderer
	Scene scene;
	if ( !scene.loadFromFile( filename ) )
	{
		sf::err() << "FATAL ERROR: Failed to load scene file" << std::endl;
		window.close();
		return EXIT_FAILURE;
	}

	Camera camera;
	Renderer renderer;
	if ( !renderer.initialize( camera, scene ) )
	{
		sf::err() << "FATAL ERROR: Failed to initialize renderer" << std::endl;
		window.close();
		return EXIT_FAILURE;
	}

	sf::Clock clock;

	// main loop - handle user input
	bool running = true;
	while ( running )
	{
		sf::Event event;
		while ( window.pollEvent(event) )
		{
			switch ( event.type )
			{
				case sf::Event::Closed:
					running = false;
					break;

				case sf::Event::KeyPressed:
					/* An important limitation of SFML is that events must be processed
					 * in the thread that created the window. So if you add any multithreading
					 * to your renderer, you will want to handle events here. For the camera,
					 * you can just use sf::Keyboard::isKeyPressed( ... ) from anywhere
					 * to check if a key is currently pressed. But if you have an important event,
					 * like saving a screenshot, that you want to trigger immediately and not poll
					 * for every frame, you should put that here.
					 */
					 break;

				case sf::Event::Resized:
					glViewport(0, 0, event.size.width, event.size.height);
					break;

				// If you want, you can pause rendering when the window is out of focus.
				// You can also do tricks when the mouse leaves the window.
				case sf::Event::LostFocus:
				case sf::Event::GainedFocus:
					break;

			}
		}

		// update the camera position and orientation
		camera.handleInput( clock.restart().asSeconds() );

		renderer.render( camera, scene );

		float frameTime = clock.getElapsedTime().asSeconds();

		// if you want to show a frame rate counter, you can use sfml's 2D graphics library
		// you will need to provide a font file in the location where your code will run
		// be careful though, as SFML/Graphics will override OpenGL settings!

		// sfml handles presenting the frame for you
		window.display();
	}

	renderer.release();

	window.close();
	return EXIT_SUCCESS;
}