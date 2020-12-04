#include "ui.hpp"

RMUI::RMUI(sf::Shader* _shader)
	: shadow(true), shader(_shader)
{
    shader->setUniform("shadow", shadow);
}

void RMUI::draw()
{
    ImGui::Begin("Options");
    if (ImGui::Checkbox("Shadows", &shadow))
        shader->setUniform("shadow", shadow);
    ImGui::End();
}
