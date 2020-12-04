#ifndef SCENE_H
#define SCENE_H

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

typedef sf::Glsl::Vec4 vec4;
typedef sf::Glsl::Vec3 vec3;
typedef sf::Glsl::Vec2 vec2;


class RMscene
{
public:
	RMscene();

	struct sphere
	{
		vec4 sphere;
		sf::Color color;
	};
	
	struct capsule
	{
		vec3 a;
		vec3 b;
		float r;
		sf::Color color;
	};

	struct plane
	{
		vec3 p;
		vec3 n;
		sf::Color color;
	};
private:

	std::vector<sphere> spheres;
	std::vector<capsule> capsules;
	std::vector<plane> planes;

public:
	void sendToShader(sf::Shader*);

	void addSphere(RMscene::sphere);
	void addCapsule(RMscene::capsule);

	void changeSphere(UINT, RMscene::sphere);
	void changeCapsule(UINT, RMscene::capsule);

	void removeSphere(UINT);
	void removeCapsule(UINT);
};

#endif SCENE_H
