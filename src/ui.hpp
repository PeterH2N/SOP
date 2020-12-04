#ifndef UI_H
#define UI_H

#include "rmscene.hpp"

class RMUI
{
public: 
	RMUI(sf::Shader*);
private:
	bool shadow;
	sf::Shader* shader;
public:
	void draw();
};


#endif UI_H
