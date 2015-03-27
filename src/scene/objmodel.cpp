#include "objmodel.hpp"
#include <SFML/System/Err.hpp>
#include <fstream>
#include <limits>

#define SKIP_THRU_CHAR( s , x ) if ( s.good() ) s.ignore( std::numeric_limits<std::streamsize>::max(), x )

// private helper function - reads a .mtl file and adds to the material table
bool ObjModel::loadMTL( std::string path, std::string filename )
{
	std::string token;
	std::string mat_name;
	std::ifstream istream( path + filename );
	if ( !istream.good( ) )
	{
		sf::err( ) << std::string( "Error opening file: " ) << path + filename << std::endl;
		return false;
	}

	// find the first material
	while ( istream.good() && token != "newmtl" ) istream >> token;
	if ( istream.eof() ) return true; // a file with no materials??

	ObjMtl material;
	istream >> mat_name;
	SKIP_THRU_CHAR( istream, '\n' );

	while ( istream.good() && (istream.peek() != EOF) )
	{
		istream >> token;

		if ( token == "newmtl" )
		{
			// push the latest material, begin a new one
			materialIDs[mat_name] = materials.size( );
			materials.push_back( material );
			material = ObjMtl();
			istream >> mat_name;		
		}
		// these are most likely the relevent materials for your renderer
		// you can do more with .obj files, but you are not expected to for p4
		else if ( token == "Ka" )
		{
			float r, g, b;
			istream >> r;
			istream >> g;
			istream >> b;
			material.Ka = glm::vec3( glm::clamp( r, 0.0f, 1.0f ),
									 glm::clamp( g, 0.0f, 1.0f ),
									 glm::clamp( b, 0.0f, 1.0f ) );
		}
		else if ( token == "Kd" )
		{
			float r, g, b;
			istream >> r;
			istream >> g;
			istream >> b;
			material.Kd = glm::vec3( glm::clamp( r, 0.0f, 1.0f ),
									 glm::clamp( g, 0.0f, 1.0f ),
									 glm::clamp( b, 0.0f, 1.0f ) );
		}
		else if ( token == "Ks" )
		{
			float r, g, b;
			istream >> r;
			istream >> g;
			istream >> b;
			material.Ks = glm::vec3( glm::clamp( r, 0.0f, 1.0f ),
									 glm::clamp( g, 0.0f, 1.0f ),
									 glm::clamp( b, 0.0f, 1.0f ) );
		}
		else if ( token == "Ns" )
		{
			istream >> material.Ns;
			material.Ns = glm::clamp( material.Ns, 0.0f, 1000.0f );
		}
		else if ( token == "map_Kd" )
		{
			istream >> token;
			// load only one copy of each texture
			if ( textureIDs.count( token ) == 0 )
			{
				textures.push_back( sf::Image() );
				if ( !textures.back().loadFromFile( path + token ) )
				{
					sf::err() << "Error loading texture: " << token << std::endl;
					return false;
				}
				textureIDs[token] = textures.size();
			}
			material.map_Kd = textureIDs[token];
		}
		else if ( token == "map_Ka" )
		{
			// this is likely the same as map_Kd, but you may want to try lightmapping
			// or pre-computed radiance at some point
			istream >> token;
			if ( textureIDs.count( token ) == 0 )
			{
				textures.push_back( sf::Image( ) );
				if ( !textures.back( ).loadFromFile( path + token ) )
				{
					sf::err( ) << "Error loading texture: " << token << std::endl;
					return false;
				}
				textureIDs[token] = textures.size( );
			}
			material.map_Ka = textureIDs[token];
		}
		// ignore all other parameters, and move to next line after each property read
		SKIP_THRU_CHAR( istream, '\n' );
	}

	// don't forget to save the last material
	materialIDs[mat_name] = materials.size( );
	materials.push_back( material );
	return true;
}

/*
 * Parses an input .obj file, loading data into memory.
 * This does not cover the entire .obj spec, just the most common cases, namely v/t/n triangles.
 * You will need to perform additional processing to generate meshes from the vectors of raw data.
 */
bool ObjModel::loadFromFile( std::string path, std::string filename )
{
	name = filename;

	std::string token;
	std::ifstream istream( path + filename );
	if ( !istream.good( ) )
	{
		sf::err( ) << std::string( "Error opening file: " ) << path + filename << std::endl;
		return false;
	}

	// if the .obj is in a subdirectory, .mtl files will be relative to that directory
	size_t pathlen = filename.find_last_of( "\\/", filename.npos );
	if ( pathlen < filename.npos )
		path += filename.substr( 0, pathlen + 1 );

	std::string mtl;
	TriangleGroup group;
	Triangle triangle;
	triangle.materialID = -1;
	triangle.smoothing_group = 1;
	triangle.smooth_shading = false;

	while ( istream.good() && (istream.peek() != EOF) )
	{
		istream >> token;

		if ( token == "#" ) // comment line
		{
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "v" ) // vertex (position)
		{
			float x, y, z;
			istream >> x;
			istream >> y;
			istream >> z;
			vertices.push_back( glm::vec3( x, y, z ) );
			// note: .obj supports a 'w' component, we're ignoring it here (it's very uncommon)
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "vt" ) // tex coord
		{
			float u, v;
			istream >> u;
			istream >> v;
			texcoords.push_back( glm::vec2( u, v ) );
			// similarly, .obj supports 3D textures with a 'w' component
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "vn" ) // vertex normal
		{
			float x, y, z;
			istream >> x;
			istream >> y;
			istream >> z;
			normals.push_back( glm::normalize( glm::vec3( x, y, z ) ) );
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "vp" ) // parameter space vertices, not supported
		{
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "mtllib" )
		{
			istream >> token;
			if ( !loadMTL( path, token ) )
			{
				sf::err() << "Failed to load material lib: " << token << std::endl;
				return false;
			}
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "usemtl" )
		{
			istream >> mtl;
			if ( materialIDs.count( mtl ) == 0 )
			{
				sf::err() << "Error in .obj: material \"" << mtl << "\" not found." << std::endl;
				return false;
			}
			triangle.materialID = materialIDs[mtl];
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "g" ) // starts a new group of polygons
		{
			// push the old group into the list, if it wasn't empty
			if ( group.triangles.size() > 0 )
			{
				groups.push_back( group );
				group.triangles.clear();
			}
			// save the name of the group for debugging
			istream >> group.name;
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "s" ) // smoothing group index
		{
			istream >> token;
			if ( token == "off" )
				triangle.smooth_shading = false;
			else
				triangle.smoothing_group = std::stoi( token );

			// smooth shading groups is a feature of .obj used in some of the scenes
			// basically, you compute normals by averaging per-triangle normals, but only
			// for triangles in the same group. don't worry about this early on, but you may
			// need it for scenes like sponza where pre-computed normals are not provided
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else if ( token == "f" ) // a face, or polygon
		{
			int v, t, n;

			// read the first vertex and detect the type		
			// the first index is always position
			istream >> v;
			triangle.vertices[0] = v - 1;
			triangle.vertexType = Triangle::POSITION_ONLY;
			if ( istream.peek() == '/' )
			{
				istream.get(); // skip the slash

				// two slashes for vertex // normal
				if ( istream.peek() == '/' )
				{
					triangle.vertexType = Triangle::POSITION_NORMAL;
					istream.get();
					istream >> n;
					triangle.normals[0] = n - 1;

					// read the remaining vertices
					istream >> v; istream.get(); istream.get(); istream >> n;
					triangle.vertices[1] = v - 1; triangle.normals[1] = n - 1;
					istream >> v; istream.get(); istream.get(); istream >> n;
					triangle.vertices[2] = v - 1; triangle.normals[2] = n - 1;
				}
				else // single slash for texcoord
				{
					triangle.vertexType = Triangle::POSITION_TEXCOORD;
					istream >> t;
					triangle.texcoords[0] = t - 1;

					if ( istream.peek() != '/' )
					{
						// no normals, just read remaining vertices
						istream >> v; istream.get(); istream >> t;
						triangle.vertices[1] = v - 1; triangle.texcoords[1] = t - 1;
						istream >> v; istream.get(); istream >> t;
						triangle.vertices[2] = v - 1; triangle.texcoords[2] = t - 1;
					}
					else
					{
						triangle.vertexType = Triangle::POSITION_TEXCOORD_NORMAL;
						istream.get();
						istream >> n;
						triangle.normals[0] = n - 1;

						istream >> v; istream.get(); istream >> t; istream.get(); istream >> n;
						triangle.vertices[1] = v - 1; triangle.texcoords[1] = t - 1; triangle.normals[1] = n - 1;
						istream >> v; istream.get(); istream >> t; istream.get(); istream >> n;
						triangle.vertices[2] = v - 1; triangle.texcoords[2] = t - 1; triangle.normals[2] = n - 1;
					}
				}
			}
			group.triangles.push_back( triangle );
			SKIP_THRU_CHAR( istream, '\n' );
		}
		else
		{
			// ignore any other lines - invalid or unsupported obj content
			SKIP_THRU_CHAR( istream, '\n' );
		}
		token.clear();
	}

	// save the last group of polygons
	if ( group.triangles.size( ) > 0 )
	{
		groups.push_back( group );
		group.triangles.clear( );
	}

	if ( istream.fail( ) )
	{
		sf::err( ) << "An error occured while reading .obj file; last token was: " << token << std::endl;
		return false;
	}

	return true;
}