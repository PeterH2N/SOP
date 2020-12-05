#ifndef UI_H
#define UI_H

#include "rmscene.hpp"

class RMUI
{
public: 
	RMUI(sf::Shader*, RMscene*);
private:
	bool shadow;


	sf::Shader* shader;
	RMscene* scene;

	void addSphereMenu();
	void editSphereMenu(UINT);
	void sphereList();

	void addCapsuleMenu();
	void editCapsuleMenu(UINT);
	void capsuleList();

	void saveScene();
	void openScene();
public:
	void draw();
};


#endif UI_H
