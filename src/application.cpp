#include "ui.hpp"
#include "rmscene.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(620, 480), "ImGui + SFML = <3");

    window.setFramerateLimit(60);
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

    scene.addSphere({vec4(1, 1, 10, 1.5), sf::Color::White});
    scene.addSphere({ vec4(-1, 1, 10, 1.5), sf::Color::Yellow });
    scene.addCapsule({ vec3(-3, 2, 10), vec3(3, 2, 10), 1 , sf::Color::Red});
    scene.addPlane({ vec3(0,0,0), vec3(0,1,0) , sf::Color::White});
    scene.sendToShader(&shader);

    RMUI ui(&shader);

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

        ImGui::SFML::Update(window, deltaClock.restart());

        ui.draw();

        window.clear();


        window.draw(screen, 4, sf::Quads, &shader);
        ImGui::SFML::Render(window);


        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}