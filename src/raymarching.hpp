#pragma once
#include "ui.hpp"
#include "rm.hpp"
#include <iostream>

int mainLoopRM()
{
    // laver vindue
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Peters SOP projekt - raymarching");

    // låser ikke framerate
    window.setVerticalSyncEnabled(false);
    ImGui::SFML::Init(window);

    // stier til shaders
    const std::string vert = "res/shader/basic_shader.vert";
    const std::string frag = "res/shader/raymarching_shader.frag";

    // initializerer raymarcher med stier og vindue
    RayMarcher RM(vert, frag, &window);

    // fortæller om programmet skal termineres eller fortsættes
    int EXITCODE;

    sf::Event event;
    // to timere til forskel i tid, og til total tid
    sf::Clock deltaClock;
    sf::Clock time;
    // starter loop
    while (window.isOpen()) {
        while (window.pollEvent(event)) {

            // hvis vinduet bliver lukket skal programmet termineres
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

            ImGui::SFML::ProcessEvent(event);
        }

        // updaterer IMGUI
        ImGui::SFML::Update(window, deltaClock.restart());

        // fjern alt fra vinduet
        window.clear();

        // tegn shader og UI
        RM.draw();
        ImGui::SFML::Render(window);;

        window.display();
    }

    // stop IMGUI
    ImGui::SFML::Shutdown();
    return EXITCODE;
}