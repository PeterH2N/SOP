#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <SFML/OpenGL.hpp>

int main()
{

    sf::RenderWindow window(sf::VideoMode(960, 450), "ImGui + SFML = <3");

    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Event event;

    sf::Vector2f windowSize = { (float)window.getSize().x, (float)window.getSize().y };

    float aspectRatio = (float)window.getSize().x / (float)window.getSize().y;

    sf::Shader shader;

    shader.loadFromFile("res/shader/basic_shader.vert", "res/shader/basic_shader.frag");

    sf::Vertex screen[] = {
    sf::Vertex({-1.0f, -1.0f}, sf::Color::Red),
    sf::Vertex({ 1.0f, -1.0f}, sf::Color::Red),
    sf::Vertex({ 1.0f,  1.0f}, sf::Color::Red),
    sf::Vertex({-1.0f,  1.0f}, sf::Color::Red),
    };

    

    shader.setUniform("screenSize", windowSize);
    shader.setUniform("time", 0.0f);

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
                aspectRatio = (float)windowSize.x / (float)windowSize.y;
            }

            ImGui::SFML::ProcessEvent(event);
        }

        shader.setUniform("time", (float)time.getElapsedTime().asSeconds());

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("imgui");
        ImGui::Button("look");
        ImGui::End();

        window.clear();


        window.draw(screen, 4, sf::Quads, &shader);
        ImGui::SFML::Render(window);


        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}