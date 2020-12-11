#pragma once
#include "ui.hpp"
#include "rm.hpp"
#include <iostream>

int mainLoopRM()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Peters SOP projekt - raymarching");

    window.setFramerateLimit(240);
    ImGui::SFML::Init(window);

    std::string vert = "res/shader/basic_shader.vert";
    std::string frag = "res/shader/raymarching_shader.frag";

    RayMarcher RM(vert, frag, &window);

    int EXITCODE;

    sf::Event event;
    sf::Clock deltaClock;
    sf::Clock time;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {

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

            ImGui::SFML::ProcessEvent(event);
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();

        RM.draw();
        ImGui::SFML::Render(window);;

        window.display();
    }

    ImGui::SFML::Shutdown();
    return EXITCODE;
}