#ifndef MESH_H
#define MESH_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

/// GLEW is needed to provide OpenGL extensions.
#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

/// GLM is needed to provide 3D math properties, particularly matrices for 3D transformations.
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp" 
#include "glm/gtx/transform.hpp"

#include <iostream>

#ifndef GL_SRGB8_ALPHA8
#define GL_SRGB8_ALPHA8 0x8C43
#endif

class Mesh
{
public:

	Mesh(sf::Shader* _shader, sf::Color _color);
	~Mesh();

//private:

	GLuint vertexVBO;
	GLuint indexVBO;

	std::vector<GLfloat> verts;
	std::vector<int> indices;

	std::vector<glm::fvec3> faceNormals;
	std::vector<GLfloat> flatVerts;
	std::vector<int> flatIndices;
private:
	sf::Shader* shader;

	sf::Color color;

	// Stride is the number of bytes per array element.
	const uint64_t stride = sizeof(GLfloat) * 9;
	// data offset for the normal information
	const uint64_t normalOffset = sizeof(GLfloat) * 3;
	// data offset for color information
	const uint64_t colorOffset = sizeof(GLfloat) * 6;
	// Amount of index elements in total.
	unsigned int drawCount();

	GLuint vao;

	glm::fvec3 calculateFaceNormal(glm::vec<3, glm::fvec3>);

	glm::fvec3 position;

	// rotation in degrees
	float rotX, rotY, rotZ;
	float scalar;

	sf::Glsl::Mat4 model();

public:

	void init();

	void loadFromOBJ(const std::string& path);

	void draw();

	void move(glm::fvec3);

	void rotate(glm::fvec3);

	void scale(float);

};






#endif MESH_H