#ifndef SCENE_H
#define SCENE_H

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <string>

#define M_PI 3.14159265359f

typedef sf::Glsl::Vec4 vec4;
typedef sf::Glsl::Vec3 vec3;
typedef sf::Glsl::Vec2 vec2;
typedef unsigned int UINT;


class RMscene
{
public:
	RMscene();
	// struct for hver type af geomtri
	struct sphere
	{
		sphere(){}
		sphere(vec4 _s, sf::Color _color, std::string _name)
			: s(_s), color(_color), name(_name) {}

		vec4 s;
		sf::Color color;
		std::string name;
	};
	
	struct capsule
	{
		capsule(){}
		capsule(vec3 _a, vec3 _b, float _r, sf::Color _color, std::string _name)
			: a(_a), b(_b), r(_r), color(_color), name(_name) {}
		vec3 a;
		vec3 b;
		float r;
		sf::Color color;
		std::string name;
	};

	struct plane
	{
		plane(){}
		plane(vec3 _p, vec3 _n, sf::Color _color, std::string _name)
			: p(_p), n(_n), color(_color), name(_name) {}
		vec3 p;
		vec3 n;
		sf::Color color;
		std::string name;
	};

	struct cube
	{
		cube() {};
		cube(vec3 _p, vec3 _s, vec3 _r, sf::Color _color, std::string _name)
			: p(_p), s(_s), r(_r), color(_color), name(_name) {}
		vec3 p;
		vec3 s;
		vec3 r;
		sf::Color color;
		std::string name;
	};
//private:
	//vektor til hver af disse. Jeg ville have brugt nedarvning hvis det ikke var for GLSLs begrænsninger

	std::vector<sphere> spheres;
	std::vector<capsule> capsules;
	std::vector<plane> planes;
	std::vector<cube> cubes;

public:
	void sendToShader(sf::Shader*);

	// en masse funktioner der ændrer på objekterne
	void addSphere(RMscene::sphere);
	void addCapsule(RMscene::capsule);
	void addPlane(RMscene::plane);
	void addCube(RMscene::cube);

	void changeSphere(UINT, RMscene::sphere);
	void changeCapsule(UINT, RMscene::capsule);
	void changePlane(UINT, RMscene::plane);
	void changeCube(UINT, RMscene::cube);

	void removeSphere(UINT);
	void removeCapsule(UINT);
	void removePlane(UINT);
	void removeCube(UINT);

	// til at oprette filer og indlæse dem igen
	bool writeToFile(std::string);
	bool readFromFile(std::string);
};

#endif SCENE_H
