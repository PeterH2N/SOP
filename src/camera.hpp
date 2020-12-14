#ifndef CAMERA_H
#define CAMERA_H

// camera klsee er implementeret på baggrund af denne video: https://www.youtube.com/watch?v=ns9eVfHCYdg

#include "mesh.hpp"

#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 6.0f
#define SENSITIVITY 0.1f
#define ZOOM 45.0f


class Camera
{
public:
	Camera(sf::RenderWindow* _window, glm::fvec3 _position = { 0,0,0 }, glm::fvec3 _up = { 0, 1, 0 }, glm::vec3 _front = { 0, 0, -1 },
		   float _yaw = YAW, float _pitch = PITCH, float _speed = SPEED, float _sensitivity = SENSITIVITY, float _zoom = ZOOM);

	enum class camMovement
	{
		forward,
		backward,
		left,
		right,
		up,
		down
	};
private:

	sf::RenderWindow* window;

	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	float zoom;

	// positionen af kameraet i world-space
	glm::fvec3 position;
	// vektor der betegner hvad op er i verden, uafhængig af hvordan kameraet er roteret.
	glm::fvec3 worldUp;
	// vektor der altid er parallel med xz planet, men peger samme retning som front.
	glm::fvec3 worldFront;
	// op i forhold til kameraets rotation
	glm::fvec3 up;
	// vektor der peger på det der kigges på
	glm::fvec3 front;
	// krydsprodukt af up og front.
	glm::fvec3 right;

	sf::Clock deltaClock;

	// funktion der opdaterer alle vektorer på baggrund af ændringer i andre værdier
	void updateVectors();

	// tager en bevægelse (forlæns højre osv) ind, og ændrer på positionen.
	void processKeyboard(camMovement, float deltaTime);

	// tager en ændring i x og y ind, og ændrer på yaw og pitch
	void processMouse(float xOffset, float yOffset, bool constrainPitch = true);

	// zoom, bruges ikke.
	void processScroll(float yOffset);
public:
	// giver view matricen, der skal bruges til at afbilde punkter
	glm::mat4 getView();

	// opsamling af al bevægelse, skal kaldes i loop.
	void doMovement();

};



#endif CAMERA_H
