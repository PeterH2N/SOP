
#pragma once

#include "projscene.hpp"

#include <fstream>

inline int mainLoopProj()
{
	///Basic vertex shader that transforms the vertex position based on a projection view matrix and passes the texture coordinate to the fragment shader.
	const std::string defaultVertexShader = "res/shader/3dproj_shader.vert";

	///Basic fragment shader that returns the colour of a pixel based on the input texture and its coordinate.
	const std::string defaultFragShader = "res/shader/3dproj_shader.frag";

	// Request a 24-bits depth buffer when creating the window
	sf::ContextSettings contextSettings;
	contextSettings.depthBits = 24;
	contextSettings.sRgbCapable = false;

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Peters SOP projekt - projektion", sf::Style::Default, contextSettings);
	//window.setVerticalSyncEnabled(true);

	// Initialise GLEW for the extended functions.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return EXIT_FAILURE;

	// Create some text to draw on top of our OpenGL object
	sf::Font font;
	if (!font.loadFromFile("res/fonts/arial.ttf"))
		return EXIT_FAILURE;
	sf::Text text(" Press C to enter camera mode. WASD to move, LShift/Space for up and down", font);
	text.setCharacterSize(20);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setFillColor(sf::Color(255, 255, 255, 170));
	text.setPosition(window.getSize().x / 2, window.getSize().y - 50);

	// Make the window the active window for OpenGL calls
	window.setActive(true);

	// Enable Z-buffer read and write and culling.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	sf::Shader mainShader;


	mainShader.loadFromFile(defaultVertexShader, defaultFragShader);

	ProjScene scene(&window, &mainShader);

	scene.addMeshOBJ("res/scenes/testscene.obj", sf::Color::White);
	scene.addMeshOBJ("res/scenes/teapot.obj", sf::Color::Cyan);
	scene.addMeshOBJ("res/scenes/spaceship.obj", sf::Color(100, 100, 100));
	scene.addMeshOBJ("res/scenes/cansatobj.obj", sf::Color::Yellow);

	scene.moveMesh(1, { 3, 1, 10 });
	scene.moveMesh(2, { -10, 6, 12 });
	scene.moveMesh(3, { -3, 1, 5 });
	scene.scaleMesh(3, 0.05);
	

	window.setActive(false);

	// Make the window no longer the active window for OpenGL calls


	// Create a clock for measuring the time elapsed
	sf::Clock clock;
	sf::Clock deltaClock;

	// is the camera active
	bool camIsActive = false;

	sf::Text FPSText;
	if (!font.loadFromFile("res/fonts/arial.ttf"))
		std::cout << "Font did not load!";

	FPSText.setFont(font);
	FPSText.setFillColor(sf::Color::Red);
	FPSText.setCharacterSize(40);
	FPSText.setPosition({ 10,10 });
	FPSText.setStyle(sf::Text::Bold);

	int EXITCODE = 0;

	// Start game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
			{
				EXITCODE = 1;
				window.close();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				EXITCODE = 0;
				window.close();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
			{
				if (!camIsActive)
				{
					camIsActive = true;
					sf::Mouse::setPosition({ (int)(window.getSize().x / 2), (int)(window.getSize().y / 2) }, window);
					window.setMouseCursorGrabbed(true);
					window.setMouseCursorVisible(false);
				}
				else
				{
					camIsActive = false;
					window.setMouseCursorVisible(true);
					window.setMouseCursorGrabbed(false);
				}


			}

			// Adjust the viewport when the window is resized
			if (event.type == sf::Event::Resized)
			{
				// Make the window the active window for OpenGL calls
				window.setActive(true);

				glViewport(0, 0, event.size.width, event.size.height);

				// Make the window no longer the active window for OpenGL calls
				window.setActive(false);
			}
		}

		// Clear the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (camIsActive)
			scene.cam.doMovement();

		float time = clock.getElapsedTime().asSeconds();

		mainShader.setUniform("time", time);

		scene.rotateMesh(2, {deltaClock.getElapsedTime().asSeconds() * 20, 0, deltaClock.getElapsedTime().asSeconds() * 20 });

		// Make the window the active window for OpenGL calls
		window.setActive(true);


		///////////////////////////////
		scene.draw();
		///////////////////////////////
		// Make the window no longer the active window for OpenGL calls
		window.setActive(false);

		// Draw some text on top of our OpenGL object
		window.pushGLStates();
		// FPS
		int fps = 1.0f / (float)deltaClock.restart().asSeconds();
		FPSText.setString(std::to_string(fps));
		// FPS
		window.draw(text);
		window.draw(FPSText);
		window.popGLStates();

		// Finally, display the rendered frame on screen
		window.display();
	}

	return EXITCODE;
}