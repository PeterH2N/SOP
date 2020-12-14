#ifndef PROJSCENE_H
#define PROJSCENE_H

#include "camera.hpp"

class ProjScene
{
public:
	ProjScene(sf::RenderWindow* _window, sf::Shader* _shader);
	~ProjScene();
private:
	// vindue pointer så vi kender størrelsen mm.
	sf::RenderWindow* window;
	// shder som meshes skal initialiseres med
	sf::Shader* shader;

	// meshes. De kan ikke kopieres, derfor er de pointers
	std::vector<Mesh*> meshes;

public:
	// kamera
	Camera cam;

	// diverse funktioner til at manipulere meshes inde i scenen.
	void addMesh(Mesh*);

	void addMeshOBJ(std::string, sf::Color);

	void removeMesh(UINT);

	void moveMesh(UINT, glm::fvec3);

	void rotateMesh(UINT, glm::fvec3);

	void scaleMesh(UINT, float);

	// returnerer blot meshes.size()
	int numMeshes();

	// binder shaderen, og tegner alle meshes på skærmen
	void draw();
};


#endif PROJSCENE_H
