
#pragma once

#include "projscene.hpp"

#include <fstream>

inline int mainLoopProj()
{
	// vertex shader som afbilder verteces og normalvektorer, og sender farven videre til fragment shaderen.
	const std::string defaultVertexShader = "res/shader/3dproj_shader.vert";

	// fragment shader som beregner lysniveau ud fra normalvektoren, og en lyskilde, og s�tter farven efter dette
	const std::string defaultFragShader = "res/shader/3dproj_shader.frag";

	// indstillinger til vinduet
	sf::ContextSettings contextSettings;
	contextSettings.depthBits = 24;
	contextSettings.sRgbCapable = false;

	// Vinduet, lavet gennem SFML
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Peters SOP projekt - projektion", sf::Style::Default, contextSettings);
	// vi undlader at l�se framerate, da meningen er at sammenligne 
	//window.setVerticalSyncEnabled(true);

	// Initialiser wrapper APIen GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return EXIT_FAILURE;

	// Laver noget tekst som forklarer kameraet
	sf::Font font;
	if (!font.loadFromFile("res/fonts/arial.ttf"))
		return EXIT_FAILURE;
	sf::Text text(" Press C to enter camera mode. WASD to move, LShift/Space for up and down", font);
	text.setCharacterSize(20);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setFillColor(sf::Color(255, 255, 255, 170));
	text.setPosition(window.getSize().x / 2, window.getSize().y - 50);

	// S�tter vinduet som det aktive vindue til openGL
	window.setActive(true);

	// S�rger for at tegne det forreste foran det bagerste. Det s�rger openGL for nu
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// laver en sf::Shader instans, nemmere end selv at loade shaders
	sf::Shader mainShader;

	// f�r sfml til at tage shaders fra filen
	mainShader.loadFromFile(defaultVertexShader, defaultFragShader);

	// laver en scene og giver den vinduet og shaderen
	ProjScene scene(&window, &mainShader);

	// tilf�jer en masse testobjekter til scenen
	scene.addMeshOBJ("res/scenes/testscene.obj", sf::Color::White);
	scene.addMeshOBJ("res/scenes/teapot.obj", sf::Color::Cyan);
	scene.addMeshOBJ("res/scenes/spaceship.obj", sf::Color(100, 100, 100));
	scene.addMeshOBJ("res/scenes/cansatobj.obj", sf::Color::Yellow);

	// ryjje objekterne lidt rundt s� de er pr�senteret p�nt
	scene.moveMesh(1, { 3, 1, 10 });
	scene.moveMesh(2, { -10, 6, 12 });
	scene.moveMesh(3, { -3, 1, 5 });
	scene.scaleMesh(3, 0.05);
	

	window.setActive(false);


	// Laver en timer til total tid, og en til tid mellem hver iteration
	sf::Clock clock;
	sf::Clock deltaClock;

	// Er kameraet aktivt
	bool camIsActive = false;

	// tekst til at vise FPS i �verste venstre hj�rne
	sf::Text FPSText;
	if (!font.loadFromFile("res/fonts/arial.ttf"))
		std::cout << "Font did not load!";

	FPSText.setFont(font);
	FPSText.setFillColor(sf::Color::Red);
	FPSText.setCharacterSize(40);
	FPSText.setPosition({ 10,10 });
	FPSText.setStyle(sf::Text::Bold);

	// returv�rdi, som bestemmer om programmet skal forts�tte eller ej
	int EXITCODE = 0;

	// Start loop
	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Hvis vinduet lukkes skal hele programmet termineres
			if (event.type == sf::Event::Closed)
			{
				EXITCODE = 1;
				window.close();
			}

			// hvis escape trykkes skal der skiftes til andet vindue
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				EXITCODE = 0;
				window.close();
			}

			// at trykke C g�r kameraet aktivt og l�ser musen. tryk C igen for at slippe ud
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

			// Juster opengl viewport n�r vinduet �ndrer st�rrelse
			if (event.type == sf::Event::Resized)
			{
				window.setActive(true);

				glViewport(0, 0, event.size.width, event.size.height);

				window.setActive(false);
			}
		}

		// clear opengl
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (camIsActive)
			scene.cam.doMovement();


		// sender tiden til fragment shaderen
		float time = clock.getElapsedTime().asSeconds();

		mainShader.setUniform("time", time);

		// roterer det ene objekt
		scene.rotateMesh(2, {deltaClock.getElapsedTime().asSeconds() * 20, 0, deltaClock.getElapsedTime().asSeconds() * 20 });



		window.setActive(true);
		// tegn al opengl her//////////
		scene.draw();
		///////////////////////////////
		window.setActive(false);

		// kald pushGLStates for at tegne normal sfml igen
		window.pushGLStates();
		// FPS //////////////
		int fps = 1.0f / (float)deltaClock.restart().asSeconds();
		FPSText.setString(std::to_string(fps));
		window.draw(text);
		window.draw(FPSText);
		// FPS //////////////
		window.popGLStates();

		// Vis alt det tegnede p� sk�rmen
		window.display();
	}

	return EXITCODE;
}