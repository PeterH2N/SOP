#include "camera.hpp"

Camera::Camera(sf::RenderWindow* _window, glm::fvec3 _position, glm::fvec3 _up, glm::vec3 _front, float _yaw, float _pitch, float _speed, float _sensitivity, float _zoom)
	:window(_window), yaw(_yaw), pitch(_pitch), speed(_speed), sensitivity(_sensitivity), zoom(_zoom), position(_position), worldUp(_up), front(_front)
{
	updateVectors();
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::doMovement()
{
	// Denne del af koden s�rger for at musen bliver i midten af vinduet, s� l�nge kamera mode er sl�et til
	sf::Vector2i middle = { (int)(window->getSize().x / 2), (int)(window->getSize().y / 2) };

	sf::Vector2i currentMousePos = sf::Mouse::getPosition(*window);

	float xOffset = currentMousePos.x - middle.x;
	float yOffset = currentMousePos.y - middle.y;

	// opdaterer kameraet baseret p� musens bev�gelse. -yOffset, fordi ellers ville der kigges ned n�r musen gik op
	processMouse(xOffset, -yOffset);

	sf::Mouse::setPosition(middle, *window);

	// timeren genstartes her, s� den er ens for alle inputs. Bev�gelse gennem rum er l�st til tid, s� framerate ikke �ndre p� hastigheden.
	float deltaTime = deltaClock.restart().asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		processKeyboard(camMovement::forward, deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		processKeyboard(camMovement::backward, deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		processKeyboard(camMovement::left, deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		processKeyboard(camMovement::right, deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		processKeyboard(camMovement::down, deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		processKeyboard(camMovement::up, deltaTime);
}

void Camera::updateVectors()
{
	glm::fvec3 tempFront;

	// vektoren der peger fremad udregnes fra yaw og pitch - taget fra linket i toppen af headerfilen
	tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	tempFront.y = sin(glm::radians(pitch));
	tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(tempFront);
	// vektoren der peger til h�jre udregnes ud fra den statiske op-vektor.
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	// worldfront er ikke statisk, men betegner den normaliserede front vektor, spejlet i xz-planet. Den bruges til at bev�ge sig lige fremad, selvom kameret peger en anden vej.
	worldFront = glm::normalize(glm::cross(worldUp, right));
}

void Camera::processKeyboard(camMovement mvmnt, float deltaTime)
{
	// hastighed beregnes ud fra til, for at garantere en konsistent hastighed, selvom framerate �ndres.
	float velocity = speed * deltaTime;

	switch (mvmnt)
	{
	case Camera::camMovement::forward:
		position += worldFront * velocity;
		break;
	case Camera::camMovement::backward:
		position -= worldFront * velocity;
		break;
	case Camera::camMovement::left:
		position -= right * velocity;
		break;
	case Camera::camMovement::right:
		position += right * velocity;
		break;
	case Camera::camMovement::up:
		position += worldUp * velocity;
		break;
	case Camera::camMovement::down:
		position -= worldUp * velocity;
	default:
		break;
	}
}

void Camera::processMouse(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// s�rger for at man ikke kan dreje s� langt op, at man vender p� hovedet.
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateVectors();
}

// bruges ikke, da jeg ikke f�lte at zoom var n�dvendigt.
void Camera::processScroll(float yOffset)
{
	if (zoom >= 1.0f && zoom <= ZOOM)
		zoom -= yOffset;
	else if (zoom < 1.0f)
		zoom = 1.0f;
	else if (zoom > ZOOM)
		zoom = ZOOM;
}
