#include "ui.hpp"
#include "rm.hpp"
#include <iostream>


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Peters SOP projekt");

    window.setFramerateLimit(240);
    ImGui::SFML::Init(window);

    std::string vert = "res/shader/basic_shader.vert";
    std::string frag = "res/shader/raymarching_shader.frag";

    RayMarcher RM(vert, frag, &window);

    sf::Event event;
    sf::Clock deltaClock;
    sf::Clock time;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized)
            {

            }

            ImGui::SFML::ProcessEvent(event);
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();
        RM.ui.draw();
        RM.draw();
        ImGui::SFML::Render(window);;
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}