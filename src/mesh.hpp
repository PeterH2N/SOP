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

private:

	// vertex buffer objekter
	GLuint vertexVBO;
	GLuint indexVBO;

	std::vector<GLfloat> verts;
	std::vector<int> indices;

	std::vector<glm::fvec3> faceNormals;
	std::vector<GLfloat> flatVerts;
	std::vector<int> flatIndices;

	// pointer til den shader der bruges
	sf::Shader* shader;

	// farve på meshen
	sf::Color color;

	// Stride er antal bytes per element.
	const uint64_t stride = sizeof(GLfloat) * 9;
	// fortæller hvor mange bytes der skal skippes for at få information om normalvektorer.
	const uint64_t normalOffset = sizeof(GLfloat) * 3;
	// samme, men for farveinfo
	const uint64_t colorOffset = sizeof(GLfloat) * 6;
	// antal verteces
	unsigned int drawCount();

	// vertex array objekt
	GLuint vao;

	// udrenger normalvektorer for hver overflade/trekant og gemmer den i facenormals. Indexet i faceNormals passer med indeces
	glm::fvec3 calculateFaceNormal(glm::vec<3, glm::fvec3>);

	// postition i rum, så objektet kan flyttes
	glm::fvec3 position;

	// rotation i grader (omregnes til radianer senere)
	float rotX, rotY, rotZ;
	float scalar;

	// returnerer tranlaterings og rotations matrix for objektet, som bruges når objektet tegnes på skærmen
	sf::Glsl::Mat4 model();

public:

	// sætter data ind i vertexayyaybuffer og index vertexarraybufferen. Må IKKE kaldes mere end 1 gang.
	void init();

	// vertex og index data indlæses fra en obj fil.
	void loadFromOBJ(const std::string& path);

	// tegner objektet. Kaldes i loop, eller bare i forbindelse med scene draw funktionen
	void draw();

	// translaterer objektet
	void move(glm::fvec3);

	// roterer ved at tilføje rotation
	void rotate(glm::fvec3);

	// skalerer objektet
	void scale(float);

};






#endif MESH_H