#ifndef CAMERA_H
#define CAMERA_H

// camera class is implemented based on this tutorial: https://www.youtube.com/watch?v=ns9eVfHCYdg

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

	glm::fvec3 position;
	glm::fvec3 worldUp;
	glm::fvec3 worldFront;
	glm::fvec3 up;
	glm::fvec3 front;
	glm::fvec3 right;

	sf::Clock deltaClock;

	void updateVectors();

	void processKeyboard(camMovement, float deltaTime);

	void processMouse(float xOffset, float yOffset, bool constrainPitch = true);

	void processScroll(float yOffset);
public:
	glm::mat4 getView();

	void doMovement();

};



#endif CAMERA_H
