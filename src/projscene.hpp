#ifndef PROJSCENE_H
#define PROJSCENE_H

#include "camera.hpp"

class ProjScene
{
public:
	ProjScene(sf::RenderWindow* _window, sf::Shader* _shader);
	~ProjScene();
private:
	sf::RenderWindow* window;
	sf::Shader* shader;

	std::vector<Mesh*> meshes;

public:
	Camera cam;

	void addMesh(Mesh*);
	void addMeshOBJ(std::string, sf::Color);

	void removeMesh(UINT);

	void changeMesh(UINT, Mesh*);

	void moveMesh(UINT, glm::fvec3);

	void rotateMesh(UINT, glm::fvec3);

	void scaleMesh(UINT, float);

	int numMeshes();

	void draw();
};


#endif PROJSCENE_H
