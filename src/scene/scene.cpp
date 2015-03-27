#include "scene.hpp"
#include <SFML/System/Err.hpp>
#include <fstream>
#include <limits>

/* Using a macro to avoid repeating excessively long, templated statement for a simple effect.
 * This takes a std::ifstream and a char and advances the ifstream until it passes the next
 * occurence of that character, or reaches the end of the file.
 *
 * Ignore the warnings about max() on Windows. They're lying, it compiles.
 */
#define SKIP_THRU_CHAR( s , x ) if ( s.good() ) s.ignore( std::numeric_limits<std::streamsize>::max(), x )

Scene::Scene()
{
}

bool Scene::loadFromFile( std::string filename )
{
	std::string path;
	size_t pathlen = filename.find_last_of( "\\/", filename.npos );
	if ( pathlen < filename.npos )
		path = filename.substr( 0, pathlen + 1 );
	else
		path = "./";

	std::string token;
	std::ifstream istream( filename );
	if ( !istream.good() )
	{
		sf::err( ) << std::string( "Error opening file." ) << filename << std::endl;
		return false;
	}

	while ( istream.good() && (istream.peek() != EOF) )
	{
		istream >> token;

		// skip comment lines
		if ( token == "#" )
		{
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "sunlight" )
		{
			SKIP_THRU_CHAR( istream, '{' );
			SKIP_THRU_CHAR( istream, '\n' );

			// read in the sunlight parameters - override any previously seen values
			while ( istream.good() && istream.peek() != '}' )
			{
				istream >> token;

				if ( token == "direction" )
				{
					float x, y, z;
					istream >> x;
					istream >> y;
					istream >> z;
					sunlight.direction = glm::normalize( glm::vec3( x, y, z ) );
				}
				else if ( token == "color" )
				{
					float r, g, b;
					istream >> r;
					istream >> g;
					istream >> b;
					sunlight.color = glm::vec3( glm::clamp( r, 0.0f, 1.0f ),
												glm::clamp( g, 0.0f, 1.0f ),
												glm::clamp( b, 0.0f, 1.0f ) );
				}
				else if ( token == "ambient" )
				{
					float a;
					istream >> a;
					sunlight.ambient = glm::clamp( a, 0.0f, 1.0f );
				}
				SKIP_THRU_CHAR( istream, '\n' );
			}
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "spotlight" )
		{
			SKIP_THRU_CHAR( istream, '{' );
			SKIP_THRU_CHAR( istream, '\n' );

			SpotLight spotlight;
			while ( istream.good( ) && istream.peek( ) != '}' )
			{
				istream >> token;

				if ( token == "position" )
				{
					float x, y, z;
					istream >> x;
					istream >> y;
					istream >> z;
					spotlight.position = glm::vec3( x, y, z );
				}
				else if ( token == "direction" )
				{
					float x, y, z;
					istream >> x;
					istream >> y;
					istream >> z;
					spotlight.direction = glm::normalize( glm::vec3( x, y, z ) );
				}
				else if ( token == "color" )
				{
					float r, g, b;
					istream >> r;
					istream >> g;
					istream >> b;
					spotlight.color = glm::vec3( glm::clamp( r, 0.0f, 1.0f ),
												 glm::clamp( g, 0.0f, 1.0f ),
												 glm::clamp( b, 0.0f, 1.0f ) );
				}
				else if ( token == "exponent" )
				{
					istream >> spotlight.exponent;				
				}
				else if ( token == "angle" )
				{
					float a;
					istream >> a;
					spotlight.angle = glm::clamp( a, 0.0f, 180.0f );
				}
				else if ( token == "length" )
				{
					float l;
					istream >> l;
					spotlight.length = glm::max( l, 0.0f );
				}
				else if ( token == "attenuation" )
				{
					istream >> spotlight.Kc;
					istream >> spotlight.Kl;
					istream >> spotlight.Kq;
				}
				SKIP_THRU_CHAR( istream, '\n' );
			}
			spotlights.push_back( spotlight );
			SKIP_THRU_CHAR( istream, '\n' );			
		}
		else if ( token == "pointlight" )
		{
			SKIP_THRU_CHAR( istream, '{' );
			SKIP_THRU_CHAR( istream, '\n' );

			PointLight pointlight;
			while ( istream.good( ) && istream.peek( ) != '}' )
			{
				istream >> token;

				if ( token == "position" )
				{
					float x, y, z;
					istream >> x;
					istream >> y;
					istream >> z;
					pointlight.position = glm::vec3( x, y, z );
				}
				else if ( token == "color" )
				{
					float r, g, b;
					istream >> r;
					istream >> g;
					istream >> b;
					pointlight.color = glm::vec3( glm::clamp( r, 0.0f, 1.0f ),
												  glm::clamp( g, 0.0f, 1.0f ),
												  glm::clamp( b, 0.0f, 1.0f ) );
				}
				else if ( token == "velocity" )
				{
					float v;
					istream >> v;
					pointlight.velocity = glm::clamp( v, 0.0f, 1.0f );
				}
				else if ( token == "attenuation" )
				{
					istream >> pointlight.Kc;
					istream >> pointlight.Kl;
					istream >> pointlight.Kq;
				}
				SKIP_THRU_CHAR( istream, '\n' );
			}
			pointlights.push_back( pointlight );
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "model" )
		{
			SKIP_THRU_CHAR( istream, '{' );
			SKIP_THRU_CHAR( istream, '\n' );

			StaticModel model;
			while ( istream.good( ) && istream.peek( ) != '}' )
			{
				istream >> token;

				if ( token == "position" )
				{
					float x, y, z;
					istream >> x;
					istream >> y;
					istream >> z;
					model.position = glm::vec3( x, y, z );
				}
				else if ( token == "orientation" )
				{
					float roll, pitch, yaw;
					istream >> roll;
					istream >> pitch;
					istream >> yaw;
					model.orientation = glm::vec3( roll, pitch, yaw );
				}
				else if ( token == "scale" )
				{
					float x, y, z;
					istream >> x;
					istream >> y;
					istream >> z;
					model.scale = glm::vec3( x, y, z );
				}
				else if ( token == "file" )
				{
					SKIP_THRU_CHAR( istream, '\"' );
					std::getline( istream, token, '\"' );

					// strip duplicate objects - only one copy of the model data in memory
					if ( objmodels.count( token ) == 0 && !objmodels[token].loadFromFile( path, token ) )
					{
						sf::err() << "Error reading .obj file: " << token << std::endl;
						return false;
					}
					model.model = &objmodels[token];
				}
				SKIP_THRU_CHAR( istream, '\n' );
			}
			models.push_back( model );
			SKIP_THRU_CHAR( istream, '\n' );
		}
	}

	if ( istream.fail() )
	{
		sf::err() << "An error occured while reading scene file; last token was: " << token << std::endl;
		return false;
	}

	return true;
}

Scene::~Scene()
{
}