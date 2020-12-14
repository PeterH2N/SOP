#include "projscene.hpp"

ProjScene::ProjScene(sf::RenderWindow* _window, sf::Shader* _shader)
	: window(_window), shader(_shader), cam(_window, { 0, 2, 0 })
{
}

ProjScene::~ProjScene()
{
	for (UINT i = 0; i < meshes.size(); i++)
		delete meshes[i];
}

void ProjScene::addMesh(Mesh* m)
{
	m->init();
	meshes.push_back(m);
}

void ProjScene::addMeshOBJ(std::string path, sf::Color color)
{
	Mesh* m = new Mesh(shader,color);

	m->loadFromOBJ(path);

	m->init();

	meshes.push_back(m);

}

void ProjScene::removeMesh(UINT i)
{
	delete meshes[i];
	meshes.erase(meshes.begin() + i);
}

void ProjScene::moveMesh(UINT i, glm::fvec3 move)
{
	meshes[i]->move(move);
}

void ProjScene::rotateMesh(UINT i, glm::fvec3 rot)
{
	meshes[i]->rotate(rot);
}

void ProjScene::scaleMesh(UINT i, float s)
{
	meshes[i]->scale(s);
}

int ProjScene::numMeshes()
{
	return meshes.size();
}

void ProjScene::draw()
{
	// Få viewMatrix fra kameraet
	glm::mat4 view = cam.getView();

	// Lav projektionsmatrix med glm
	GLfloat ratio = static_cast<float>(window->getSize().x) / window->getSize().y;
	glm::mat4 projection = glm::perspective(45.0f, ratio, 0.1f, 500.0f);

	// Laver sf matricer, så de nemmere kan sendes til shaderen
	sf::Glsl::Mat4 v(&view[0][0]);
	sf::Glsl::Mat4 proj(&projection[0][0]);

	// sender til shaderen
	shader->setUniform("view", v);
	shader->setUniform("proj", proj);

	// binder shader
	sf::Shader::bind(shader);
	// tegner alle meshes
	for (Mesh* mesh : meshes)
		mesh->draw();
	// unbinder shader
	sf::Shader::bind(NULL);
}
