#ifndef _OBJMODEL_H_
#define _OBJMODEL_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <SFML/Graphics/Image.hpp>

class ObjModel
{
public:

	struct ObjMtl
	{
		// the most relevant material values for basic lighting
		glm::vec3 Ka;
		glm::vec3 Kd;
		glm::vec3 Ks;
		float Ns; // specular exponent in [0,1000]
		
		// indexes into the textures array; -1 for no texture
		int map_Kd;
		int map_Ka;

		ObjMtl() : Ka( glm::vec3( 0.0f, 0.0f, 0.0f ) ),
				   Kd( glm::vec3( 0.0f, 0.0f, 0.0f ) ),
				   Ks( glm::vec3( 0.0f, 0.0f, 0.0f ) ),
				   Ns( 0.0f ),
				   map_Kd( -1 ),
				   map_Ka( -1 )
		{
		}
	};

	// you can extend this to support quads or arbitrary polygons
	// alternatively, break quads into triangles immediately during file read
	struct Triangle
	{
		// generally, all faces in a group will use the same data, but this isn't guaranteed
		// you will want to form meshes from only faces with the same per-vertex data for building vertex buffers
		enum VertexType { POSITION_ONLY, POSITION_TEXCOORD, POSITION_NORMAL, POSITION_TEXCOORD_NORMAL } vertexType;

		int materialID; // groups can contain polygons with different materials - you may want to split these
		                // into separate meshes for rendering

		bool smooth_shading;
		int smoothing_group; // smoothing group, for computing normals

		// these are indexes into the master lists of position, texcoord, and normal data
		// note 1: the starter code adjusts these to index from 0, not from 1 as in the .obj text
		// note 2: a major issue with .obj is that each vertex in a face can have unique indexes for its position,
		//         texcoord, and normal. how can you consolidate these with minimal duplication?
		int vertices[3];
		int texcoords[3];
		int normals[3];
	};

	struct TriangleGroup
	{
		std::string name;
		std::vector<Triangle> triangles;
	};

	bool loadFromFile( std::string path, std::string filename );

private:
	std::string name;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;

	// you may want to hoist materials out of models into a global scene list
	// that way multiple .obj's can inherit the same .mtl without duplication
	std::vector<ObjMtl> materials;
	std::unordered_map<std::string, int> materialIDs;
	std::vector<sf::Image> textures; // also take a look at sf::Texture - what is different about them?
	std::unordered_map<std::string, int> textureIDs;

	std::vector<TriangleGroup> groups;

	bool loadMTL( std::string path, std::string filename );
};

#endif // _OBJMODEL_H_