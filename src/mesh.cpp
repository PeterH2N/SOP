#include "mesh.hpp"

#include <fstream>
#define M_PI 3.14159265359f


Mesh::Mesh(sf::Shader* _shader, sf::Color _color)
	: shader(_shader), vao(0), color(_color), position(0), rotX(0), rotY(0), rotZ(0), scalar(1)
{
	// Generate Vertex Buffer
	glGenBuffers(1, &vertexVBO);
	// Generate Index Buffer
	glGenBuffers(1, &indexVBO);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexVBO);
	glDeleteBuffers(1, &indexVBO);
	vertexVBO = 0;
	indexVBO = 0;
	vao = 0;
}

unsigned int Mesh::drawCount()
{
	return indices.size();
}

glm::fvec3 Mesh::calculateFaceNormal(glm::vec<3, glm::fvec3> tri)
{

	// getting the vectors between them
	glm::fvec3 v1v2 = tri[1] - tri[0];
	glm::fvec3 v1v3 = tri[2] - tri[0];

	//std::cout << "\n\n";
	//
	//std::cout << v1v2.x << " " << v1v2.y << " " << v1v2.z << std::endl;

	// calculating face normal
	glm::fvec3 n = glm::normalize(glm::cross(v1v2, v1v3));

	//std::cout << n.x << " " << n.y << " " << n.z << std::endl;

	return n;

}

sf::Glsl::Mat4 Mesh::model()
{
	glm::mat4 matrix_pos = glm::translate(position);
	glm::mat4 matrix_rotX = glm::rotate(rotX * (M_PI / 180.0f), glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 matrix_rotY = glm::rotate(rotY * (M_PI / 180.0f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 matrix_rotZ = glm::rotate(rotZ * (M_PI / 180.0f), glm::vec3(0.f, 0.f, 1.f));

	matrix_pos = glm::scale(matrix_pos, glm::fvec3(scalar));

	glm::mat4 matrix_model = matrix_pos * matrix_rotZ * matrix_rotY * matrix_rotX;

	return sf::Glsl::Mat4(&matrix_model[0][0]);
}

void Mesh::loadFromOBJ(const std::string& path)
{
	bool normals = false;
	bool textures = false;

	verts.clear();
	indices.clear();

	std::ifstream file;

	file.open(path, std::ios::in);

	if (!file.good())
		std::cout << "Could not open file: " << path << std::endl;

	std::string line;

	while (std::getline(file, line))
	{
		// kommetarer
		if (line[0] == '#')
		{

		}
		// verteces
		else if (line[0] == 'v' && line[1] == ' ')
		{
			int i1, i2;
			i1 = line.find(" ");
			if (line[i1 + 1] == ' ')
				i1++;

			for (UINT i = 0; i < 3; i++)
			{
				i2 = line.find(" ", i1 + 1);
				verts.push_back(stof(line.substr(i1, i2)));
				i1 = i2;
			}
			for (UINT i = 0; i < 6; i++)
			{
				verts.push_back(0);
			}

		}
		// tjekker om der er information om normalvektorer. Disse bruges ikke, da vi selv udregner dem.
		else if (!normals && line[0] == 'v' && line[1] == 'n')
		{
			normals = true;
		}
		// tjekker om der er textureinformation.
		else if (!textures && line[0] == 'v' && line[1] == 't')
		{
			textures = true;
		}
		// hvis hverken normalvektor eller texture information er der, skal indekserne læses lidt anderledes
		else if (line[0] == 'f' && !normals && !textures)
		{
			int i1, i2;
			i1 = line.find(" ");
			for (UINT i = 0; i < 3; i++)
			{
				i2 = line.find(" ", i1 + 1);
				indices.push_back(stoi(line.substr(i1, i2)) - 1);
				i1 = i2;
			}
		}
		// når der er info om enten normal eller textures står index lidt anderleses i filen.
		else if ((line[0] == 'f' && normals) || (line[0] == 'f' && textures))
		{
			int space = 0;
			int slash = 0;
			for (UINT i = 0; i < 3; i++)
			{
				space = line.find(' ', space + 1);
				slash = line.find('/', space);
				indices.push_back(stoi(line.substr(space, slash)) - 1);
			}
		}
	}
}

void Mesh::draw()
{
	// sætter model matricen i shaderen
	shader->setUniform("model", model());
	// binder vores vertex array objekt
	glBindVertexArray(vao);
	// tegner alle trekanterne
	glDrawElements(GL_TRIANGLES, drawCount(), GL_UNSIGNED_INT, 0);

	// binder vertexarrayobjekt til intet, så der kan tegnes andet.
	glBindVertexArray(0);
}

void Mesh::move(glm::fvec3 m)
{
	position += m;
}

void Mesh::rotate(glm::fvec3 r)
{
	rotX += r.x;
	rotY += r.y;
	rotZ += r.z;
}

void Mesh::scale(float s)
{
	scalar = s;
}

void Mesh::init()
{
	// Pakke alle trekanter ind så de er nemmere at tilg¨å
	std::vector<glm::vec<3, glm::fvec3>> tris;

	for (UINT i = 0; i < indices.size(); i += 3)
	{
		glm::vec< 3, glm::fvec3> tri;

		tri[0] = glm::fvec3(verts[indices[i + 0] * 9 + 0], verts[indices[i + 0] * 9 + 1], verts[indices[i + 0] * 9 + 2]);
		tri[1] = glm::fvec3(verts[indices[i + 1] * 9 + 0], verts[indices[i + 1] * 9 + 1], verts[indices[i + 1] * 9 + 2]);
		tri[2] = glm::fvec3(verts[indices[i + 2] * 9 + 0], verts[indices[i + 2] * 9 + 1], verts[indices[i + 2] * 9 + 2]);

		tris.push_back(tri);
	}

	// Sætter dem alle ind i en ny vektor
	for (UINT i = 0; i < tris.size(); i++)
	{
		glm::fvec3 normal = calculateFaceNormal(tris[i]);

		// verteces
		flatVerts.push_back(tris[i][0][0]);
		flatVerts.push_back(tris[i][0][1]);
		flatVerts.push_back(tris[i][0][2]);

		//normals - TODO
		flatVerts.push_back(normal.x);
		flatVerts.push_back(normal.y);
		flatVerts.push_back(normal.z);

		// color
		flatVerts.push_back((float)color.r / 255.0f);
		flatVerts.push_back((float)color.g / 255.0f);
		flatVerts.push_back((float)color.b / 255.0f);

		// verteces
		flatVerts.push_back(tris[i][1][0]);
		flatVerts.push_back(tris[i][1][1]);
		flatVerts.push_back(tris[i][1][2]);

		//normals - TODO
		flatVerts.push_back(normal.x);
		flatVerts.push_back(normal.y);
		flatVerts.push_back(normal.z);

		// color
		flatVerts.push_back((float)color.r / 255.0f);
		flatVerts.push_back((float)color.g / 255.0f);
		flatVerts.push_back((float)color.b / 255.0f);

		// verteces
		flatVerts.push_back(tris[i][2][0]);
		flatVerts.push_back(tris[i][2][1]);
		flatVerts.push_back(tris[i][2][2]);

		//normals - TODO
		flatVerts.push_back(normal.x);
		flatVerts.push_back(normal.y);
		flatVerts.push_back(normal.z);

		// color
		flatVerts.push_back((float)color.r / 255.0f);
		flatVerts.push_back((float)color.g / 255.0f);
		flatVerts.push_back((float)color.b / 255.0f);
	}


	for (UINT i = 0; i < flatVerts.size() / 9; i++)
	{
		flatIndices.push_back(i);
	}

	// sætter vertex array objects og vertex buffer objects op, og sender geometridata til grafikhukommelsen
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, drawCount() * stride, flatVerts.data(), GL_STATIC_DRAW);

	GLint posAttrib = glGetAttribLocation(shader->getNativeHandle(), "position");

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(posAttrib);

	GLint normAttrib = glGetAttribLocation(shader->getNativeHandle(), "normal");

	glEnableVertexAttribArray(normAttrib);
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)normalOffset);

	GLint colAttrib = glGetAttribLocation(shader->getNativeHandle(), "color");

	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawCount() * sizeof(indices[0]), flatIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}
