#ifndef RM_H
#define RM_H

#include "ui.hpp"

// wrapper klasse til det hele, for at g�re loopet mere clean
class RayMarcher
{
public:
	RayMarcher(std::string vert, std::string frag, sf::RenderWindow*);
//private:

	RMscene scene;

	sf::Shader shader;

	RMUI ui;

	sf::Clock time;

	sf::Clock deltaClock;

	sf::Text FPSText;

	sf::Font font;

	sf::RectangleShape screen;

	sf::RenderWindow* window;

	// boolean der fort�ller om vi overhovedet skal rendere til sk�rmen
	bool render;

public:

	void draw();

};


#endif RM_H