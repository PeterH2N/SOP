#ifndef UI_H
#define UI_H

#include "rmscene.hpp"

class RMUI
{
public: 
	RMUI(sf::Shader*, RMscene*, bool*);
private:
	bool shadow;
	bool color;

	bool* render;

	// pointers til den shader og den scene som UIen manipulerer
	sf::Shader* shader;
	RMscene* scene;

	// menuer til alle de forskellige objekter. Kunne nok generaliseres med templates
	void addSphereMenu();
	void editSphereMenu(UINT);
	void sphereList();

	void addCapsuleMenu();
	void editCapsuleMenu(UINT);
	void capsuleList();

	void addCubeMenu();
	void editCubeMenu(UINT);
	void cubeList();

	void menu();
	void shaderOptions();
	void objects();

	void saveScene();
	void openScene();
public:
	void draw();
};


#endif UI_H
