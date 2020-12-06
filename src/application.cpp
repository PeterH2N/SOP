#include "ui.hpp"
#include "rmscene.hpp"
#include <iostream>



int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "ImGui + SFML = <3");

    window.setFramerateLimit(240);
    ImGui::SFML::Init(window);

    sf::Event event;

    sf::Vector2f windowSize = { (float)window.getSize().x, (float)window.getSize().y };
    sf::Vector2f viewPort = sf::Vector2f(windowSize.x, windowSize.y);

    sf::Shader shader;

    shader.loadFromFile("res/shader/basic_shader.vert", "res/shader/raymarching_shader.frag");

    sf::Vertex screen[] = {
    sf::Vertex({0, 0}),
    sf::Vertex({ viewPort.x, 0}),
    sf::Vertex({ viewPort.x,  viewPort.y}),
    sf::Vertex({0,  viewPort.y}),
    };

    shader.setUniform("screenSize", viewPort);
    shader.setUniform("time", 0.0f);

    RMscene scene;

    //scene.addSphere({vec4(1, 1, 10, 1.5), sf::Color::White, "sphere 1"});
    //scene.addSphere({ vec4(-1, 1, 10, 1.5), sf::Color::Yellow, "sphere 2" });
    //scene.addCapsule({ vec3(-3, 2, 10), vec3(3, 2, 10), 1 , sf::Color::Red, "capsule 1"});
    scene.addPlane({ vec3(0,0,0), vec3(0,1,0) , sf::Color(255,255,255), "Ground Plane"});
    //scene.addCube({ vec3(0, 0.5, 10), vec3(0.5, 0.5, 0.5), vec3(0, 45, 0), sf::Color::White, "cube 1" });

    scene.sendToShader(&shader);

    sf::Font font;
    if (!font.loadFromFile("res/fonts/arial.ttf"))
        std::cout << "Font did not load!";

    sf::Text FPSText;

    FPSText.setFont(font);
    FPSText.setFillColor(sf::Color::Red);
    FPSText.setCharacterSize(40);
    FPSText.setPosition({ 10,10 });
    FPSText.setStyle(sf::Text::Bold);


    RMUI ui(&shader, &scene);

    sf::Clock deltaClock;
    sf::Clock time;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                windowSize = { (float)event.size.width, (float)event.size.height };
            }

            ImGui::SFML::ProcessEvent(event);
        }

        shader.setUniform("time", (float)time.getElapsedTime().asSeconds());

        // FPS
        int fps = 1.0f / (float)deltaClock.getElapsedTime().asSeconds();
        FPSText.setString(std::to_string(fps));
        // FPS

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();

        ui.draw();
        window.draw(screen, 4, sf::Quads, &shader);
        ImGui::SFML::Render(window);;

        window.draw(FPSText);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}