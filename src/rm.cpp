#include "rm.hpp"
#include <iostream>

RayMarcher::RayMarcher(std::string vert, std::string frag, sf::RenderWindow* _window)
	: scene(), render(true), ui(RMUI(&shader, &scene, &render)), window(_window)
{
	shader.loadFromFile(vert, frag);

	shader.setUniform("shadow", true);
	shader.setUniform("color", true);

	scene.addPlane({ vec3(0,0,0), vec3(0,1,0) , sf::Color(255,255,255), "Ground Plane" });

	scene.sendToShader(&shader);

	screen.setSize({ (float)window->getSize().x, (float)window->getSize().y});
	screen.setPosition(0, 0);

	shader.setUniform("screenSize", screen.getSize());
	shader.setUniform("time", 0.0f);

	if (!font.loadFromFile("res/fonts/arial.ttf"))
		std::cout << "Font did not load!";

	FPSText.setFont(font);
	FPSText.setFillColor(sf::Color::Red);
	FPSText.setCharacterSize(40);
	FPSText.setPosition({ 10,10 });
	FPSText.setStyle(sf::Text::Bold);
}

void RayMarcher::draw()
{

		// FPS
		int fps = 1.0f / (float)deltaClock.restart().asSeconds();
		FPSText.setString(std::to_string(fps));
		// FPS

		if (render)
		{
			shader.setUniform("time", (float)time.getElapsedTime().asSeconds());
			window->draw(screen, &shader);
		}

		ui.draw();
		window->draw(FPSText);

}