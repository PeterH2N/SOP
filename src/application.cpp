#include "ui.hpp"
#include "rm.hpp"
#include <iostream>

#include "GL/glew.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp" 
#include "glm/gtx/transform.hpp"


#ifndef GL_SRGB8_ALPHA8
#define GL_SRGB8_ALPHA8 0x8C43
#endif

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

        RM.draw();
        ImGui::SFML::Render(window);;

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}