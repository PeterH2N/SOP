#include "ui.hpp"
#include <iostream>
#include "misc/cpp/imgui_stdlib.h"

RMUI::RMUI(sf::Shader* _shader, RMscene* _scene)
    : shadow(true), color(true), shader(_shader), scene(_scene)
{
    shader->setUniform("shadow", shadow);
    shader->setUniform("color", color);
}

void RMUI::addSphereMenu()
{
    if (ImGui::BeginPopupModal("Add Sphere"))
    {
        static float x, y, z;
        static float radius;
        static float color[3];
        static std::string name;
        ImGui::Text("Center");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("x", &x); ImGui::SameLine();
        ImGui::InputFloat("y", &y); ImGui::SameLine();
        ImGui::InputFloat("z", &z);

        ImGui::Text("Radius"); ImGui::SameLine();
        ImGui::InputFloat("", &radius);
        ImGui::PushItemWidth(100);
        ImGui::ColorEdit3("Color", color);
        ImGui::PopItemWidth();
        ImGui::InputText("Name", &name);

        if (ImGui::Button("Add") && radius != 0)
        {
            int r = color[0] * 255.0f;
            int g = color[1] * 255.0f;
            int b = color[2] * 255.0f;
            if (name == "")
            {
                std::string hi = std::to_string(scene->spheres.size() + 1);
                name = "Sphere " + hi;
            }

            scene->addSphere(RMscene::sphere(vec4(x, y, z, radius), sf::Color(r, g, b), name));
            scene->sendToShader(shader);

            ImGui::CloseCurrentPopup();
            x = 0; y = 0; z = 0; radius = 0, color[0] = 0; color[1] = 0; color[2] = 0;
            name = "";
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            x = 0; y = 0; z = 0; radius = 0, color[0] = 0; color[1] = 0; color[2] = 0;
            name = "";
        }
        ImGui::EndPopup();
    }
}

void RMUI::editSphereMenu(UINT index)
{
    if (ImGui::BeginPopupModal("Edit Sphere"))
    {
        static float x, y, z;
        static float radius;
        static float color[3];
        static std::string name;
        static bool isEditing = false;

        if (!isEditing)
        {
            x = scene->spheres[index].s.x, y = scene->spheres[index].s.y, z = scene->spheres[index].s.z;
            radius = scene->spheres[index].s.w;
            name = scene->spheres[index].name;
            color[0] = (float)scene->spheres[index].color.r / 255.0f;
            color[1] = (float)scene->spheres[index].color.g / 255.0f;
            color[2] = (float)scene->spheres[index].color.b / 255.0f;
            isEditing = true;
        }

        ImGui::Text("Center");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("x", &x); ImGui::SameLine();
        ImGui::InputFloat("y", &y); ImGui::SameLine();
        ImGui::InputFloat("z", &z);

        ImGui::Text("Radius"); ImGui::SameLine();
        ImGui::InputFloat("", &radius);
        ImGui::PushItemWidth(100);
        ImGui::ColorEdit3("Color", color);
        ImGui::PopItemWidth();
        ImGui::InputText("Name", &name);

        if (ImGui::Button("Apply") && radius != 0)
        {
            int r = color[0] * 255.0f;
            int g = color[1] * 255.0f;
            int b = color[2] * 255.0f;
            if (name == "")
                name = "Sphere " + std::to_string(scene->spheres.size() + 1);

            scene->changeSphere(index, RMscene::sphere(vec4(x, y, z, radius), sf::Color(r, g, b), name));
            scene->sendToShader(shader);
            
            //x = 0; y = 0; z = 0; radius = 0, color[0] = 0; color[1] = 0; color[2] = 0;
            name = "";
            isEditing = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            //x = 0; y = 0; z = 0; radius = 0, color[0] = 0; color[1] = 0; color[2] = 0;
            name = "";
            isEditing = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void RMUI::sphereList()
{
    if (ImGui::Button("+", { 18,18 }))
        ImGui::OpenPopup("Add Sphere");
    addSphereMenu();
    ImGui::Separator();
    for (UINT i = 0; i < scene->spheres.size(); i++)
    {
        ImGui::PushID(i);
        ImGui::Text(scene->spheres[i].name.c_str()); ImGui::SameLine(ImGui::GetWindowWidth() - 110);
        
        if (ImGui::Button("Edit"))
            ImGui::OpenPopup("Edit Sphere");
        editSphereMenu(i);

        ImGui::SameLine();
        if (ImGui::Button("Remove"))
        {
            scene->removeSphere(i);
            scene->sendToShader(shader);
        }
        ImGui::Separator();
        ImGui::PopID();

    }
}

void RMUI::addCapsuleMenu()
{
    if (ImGui::BeginPopupModal("Add Capsule"))
    {
        static float ax, ay, az, bx, by, bz, radius;
        static float color[3];
        static std::string name;
        ImGui::Text("Point A");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("x##1", &ax); ImGui::SameLine();
        ImGui::InputFloat("y##1", &ay); ImGui::SameLine();
        ImGui::InputFloat("z##1", &az);

        ImGui::Text("Point B");
        ImGui::InputFloat("x", &bx); ImGui::SameLine();
        ImGui::InputFloat("y", &by); ImGui::SameLine();
        ImGui::InputFloat("z", &bz);

        ImGui::Text("Radius"); ImGui::SameLine();
        ImGui::InputFloat("", &radius);

        ImGui::PushItemWidth(100);
        ImGui::ColorEdit3("Color", color);
        ImGui::PopItemWidth();
        ImGui::InputText("Name", &name);

        if (ImGui::Button("Add"))
        {
            int r = color[0] * 255.0f;
            int g = color[1] * 255.0f;
            int b = color[2] * 255.0f;
            if (name == "")
                name = "Capsule " + std::to_string(scene->capsules.size() + 1);

            scene->addCapsule(RMscene::capsule(vec3(ax, ay, az), vec3(bx, by, bz), radius, sf::Color(r, g, b), name));
            scene->sendToShader(shader);

            ImGui::CloseCurrentPopup();
            ax = 0; ay = 0; az = 0; bx = 0; by = 0; bz = 0; radius = 0; color[0] = 0; color[1] = 0; color[2] = 0; name = "";
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            ax = 0; ay = 0; az = 0; bx = 0; by = 0; bz = 0; radius = 0; color[0] = 0; color[1] = 0; color[2] = 0; name = "";
        }
        ImGui::EndPopup();
    }
}

void RMUI::editCapsuleMenu(UINT index)
{


    if (ImGui::BeginPopupModal("Edit Capsule"))
    {
        static float ax, ay, az, bx, by, bz, radius;
        static float color[3];
        static std::string name;
        static bool isEditingCaps = false;

        if (!isEditingCaps)
        {
            ax = scene->capsules[index].a.x, ay = scene->capsules[index].a.y, az = scene->capsules[index].a.z,
                bx = scene->capsules[index].b.x, by = scene->capsules[index].b.y, bz = scene->capsules[index].b.z,
                radius = scene->capsules[index].r;
            color[0] = scene->capsules[index].color.r / 255.0f;
            color[1] = scene->capsules[index].color.g / 255.0f;
            color[2] = scene->capsules[index].color.b / 255.0f;
            name = scene->capsules[index].name;
            isEditingCaps = true;
        }

        ImGui::Text("Point A");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("x##1", &ax); ImGui::SameLine();
        ImGui::InputFloat("y##1", &ay); ImGui::SameLine();
        ImGui::InputFloat("z##1", &az);

        ImGui::Text("Point B");
        ImGui::InputFloat("x", &bx); ImGui::SameLine();
        ImGui::InputFloat("y", &by); ImGui::SameLine();
        ImGui::InputFloat("z", &bz);

        ImGui::Text("Radius"); ImGui::SameLine();
        ImGui::InputFloat("", &radius);

        ImGui::PushItemWidth(100);
        ImGui::ColorEdit3("Color", color);
        ImGui::PushItemWidth(100);
        ImGui::InputText("Name", &name);

        if (ImGui::Button("Apply"))
        {
            int r = color[0] * 255.0f;
            int g = color[1] * 255.0f;
            int b = color[2] * 255.0f;
            if (name == "")
                name = "Capsule " + std::to_string(scene->capsules.size() + 1);

            scene->changeCapsule(index, RMscene::capsule(vec3(ax, ay, az), vec3(bx, by, bz), radius, sf::Color(r, g, b), name));
            scene->sendToShader(shader);

            isEditingCaps = false;
            ax = 0; ay = 0; az = 0; bx = 0; by = 0; bz = 0; radius = 0; color[0] = 0; color[1] = 0; color[2] = 0; name = "";
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            isEditingCaps = false;
            ax = 0; ay = 0; az = 0; bx = 0; by = 0; bz = 0; radius = 0; color[0] = 0; color[1] = 0; color[2] = 0; name = "";
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    
}

void RMUI::capsuleList()
{
    if (ImGui::Button("+", { 18,18 }))
        ImGui::OpenPopup("Add Capsule");
    addCapsuleMenu();
    ImGui::Separator();
    for (UINT i = 0; i < scene->capsules.size(); i++)
    {
        ImGui::PushID(i);
        ImGui::Text(scene->capsules[i].name.c_str()); ImGui::SameLine(ImGui::GetWindowWidth() - 110);

        if (ImGui::Button("Edit"))
            ImGui::OpenPopup("Edit Capsule");
        editCapsuleMenu(i);

        ImGui::SameLine();
        if (ImGui::Button("Remove"))
        {
            scene->removeCapsule(i);
            scene->sendToShader(shader);
        }
        ImGui::Separator();
        ImGui::PopID();

    }
}

void RMUI::addCubeMenu()
{
    if (ImGui::BeginPopupModal("Add Cube"))
    {
        static vec3 cubeP;
        static vec3 cubeS;
        static vec3 cubeR;
        static std::string name;
        static float color[3];

        ImGui::Text("Center Point");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("x##1", &cubeP.x); ImGui::SameLine();
        ImGui::InputFloat("y##1", &cubeP.y); ImGui::SameLine();
        ImGui::InputFloat("z##1", &cubeP.z);

        ImGui::Text("Size");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("W", &cubeS.x); ImGui::SameLine();
        ImGui::InputFloat("B", &cubeS.z); ImGui::SameLine();
        ImGui::InputFloat("H", &cubeS.y);

        ImGui::Text("Rotation (degrees)");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("x", &cubeR.x); ImGui::SameLine();
        ImGui::InputFloat("y", &cubeR.y); ImGui::SameLine();
        ImGui::InputFloat("z", &cubeR.z);

        ImGui::PushItemWidth(100);
        ImGui::ColorEdit3("Color", color);
        ImGui::PushItemWidth(100);
        ImGui::InputText("Name", &name);
        ImGui::PopItemWidth();

        if (ImGui::Button("Add"))
        {
            int r = color[0] * 255.0f;
            int g = color[1] * 255.0f;
            int b = color[2] * 255.0f;
            if (name == "")
                name = "Cube " + std::to_string(scene->capsules.size() + 1);

            scene->addCube({ cubeP, cubeS, cubeR, sf::Color(r, g, b), name });
            scene->sendToShader(shader);
            cubeP = vec3(); cubeS = vec3(); cubeR = vec3(); name = ""; color[0] = 0; color[1] = 0; color[2] = 0;
            ImGui::CloseCurrentPopup();

        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            cubeP = vec3(); cubeS = vec3(); cubeR = vec3(); name = ""; color[0] = 0; color[1] = 0; color[2] = 0;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void RMUI::editCubeMenu(UINT index)
{
    if (ImGui::BeginPopupModal("Edit Cube"))
    {
        static vec3 cubeP;
        static vec3 cubeS;
        static vec3 cubeR;
        static std::string name;
        static float cubecolor[3];
        static bool isEditingCube = false;

        if (!isEditingCube)
        {
            cubeP = scene->cubes[index].p;
            cubeS = scene->cubes[index].s;
            cubeR = scene->cubes[index].r;
            name = scene->cubes[index].name;
            cubecolor[0] = scene->cubes[index].color.r / 225.0f;
            cubecolor[1] = scene->cubes[index].color.g / 225.0f;
            cubecolor[2] = scene->cubes[index].color.b / 225.0f;
            isEditingCube = true;
        }

        ImGui::Text("Center Point");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("x##1", &cubeP.x); ImGui::SameLine();
        ImGui::InputFloat("y##1", &cubeP.y); ImGui::SameLine();
        ImGui::InputFloat("z##1", &cubeP.z);

        ImGui::Text("Size");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("W", &cubeS.x); ImGui::SameLine();
        ImGui::InputFloat("B", &cubeS.z); ImGui::SameLine();
        ImGui::InputFloat("H", &cubeS.y);

        ImGui::Text("Rotation (degrees)");
        ImGui::PushItemWidth(50);
        ImGui::InputFloat("x", &cubeR.x); ImGui::SameLine();
        ImGui::InputFloat("y", &cubeR.y); ImGui::SameLine();
        ImGui::InputFloat("z", &cubeR.z);

        ImGui::PushItemWidth(100);
        ImGui::ColorEdit3("Color", cubecolor);
        ImGui::PushItemWidth(100);
        ImGui::InputText("Name", &name);
        ImGui::PopItemWidth();

        if (ImGui::Button("Apply"))
        {
            int r = cubecolor[0] * 255.0f;
            int g = cubecolor[1] * 255.0f;
            int b = cubecolor[2] * 255.0f;
            if (name == "")
                name = "Cube " + std::to_string(scene->capsules.size() + 1);

            scene->changeCube(index, { cubeP, cubeS, cubeR, sf::Color(r, g, b), name });
            scene->sendToShader(shader);
            //cubeP = vec3(); cubeS = vec3(); name = ""; color[0] = 0; color[1] = 0; color[2] = 0;
            isEditingCube = false;
            ImGui::CloseCurrentPopup();

        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            //cubeP = vec3(); cubeS = vec3(); name = ""; color[0] = 0; color[1] = 0; color[2] = 0;
            isEditingCube = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void RMUI::cubeList()
{
    if (ImGui::Button("+", { 18,18 }))
        ImGui::OpenPopup("Add Cube");
    addCubeMenu();
    ImGui::Separator();
    for (UINT i = 0; i < scene->cubes.size(); i++)
    {
        ImGui::PushID(i);
        ImGui::Text(scene->cubes[i].name.c_str()); ImGui::SameLine(ImGui::GetWindowWidth() - 110);

        if (ImGui::Button("Edit"))
            ImGui::OpenPopup("Edit Cube");
        editCubeMenu(i);

        ImGui::SameLine();
        if (ImGui::Button("Remove"))
        {
            scene->removeCube(i);
            scene->sendToShader(shader);
        }
        ImGui::Separator();
        ImGui::PopID();

    }
}

void RMUI::menu()
{
    std::string menuAction = "";

    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Save"))
            menuAction = "save";
        if (ImGui::MenuItem("Open"))
            menuAction = "open";
        ImGui::EndMenu();
    }
    if (menuAction == "save")   ImGui::OpenPopup("Save Scene");
    else if (menuAction == "open") ImGui::OpenPopup("Open Scene");

    saveScene();
    openScene();

}

void RMUI::shaderOptions()
{
    if (ImGui::TreeNode("Shader Options"))
    {
        if (ImGui::Checkbox("Shadows", &shadow))
            shader->setUniform("shadow", shadow);
        if (ImGui::Checkbox("Colors", &color))
            shader->setUniform("color", color);

        ImGui::TreePop();
    }
}

void RMUI::objects()
{
    if (ImGui::TreeNode("Objects"))
    {
        if (ImGui::TreeNode("Spheres"))
        {
            sphereList();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Capsules"))
        {
            capsuleList();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Cubes"))
        {
            cubeList();
            ImGui::TreePop();
        }
    }
}

void RMUI::saveScene()
{
    if (ImGui::BeginPopupModal("Save Scene"))
    {
        static std::string path;
        static std::string name;
        ImGui::Text("Specify path for saved file");
        ImGui::InputText("##1", &path);
        ImGui::Text("Name");
        ImGui::InputText("##2", &name);
        if (ImGui::Button("Save"))
        {
            scene->writeToFile(name, path);
            path = ""; name = "";
            ImGui::CloseCurrentPopup();
            return;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            path = ""; name = "";
            ImGui::CloseCurrentPopup();
            return;
        }
        ImGui::EndPopup();
    }
}

void RMUI::openScene()
{
    if (ImGui::BeginPopupModal("Open Scene"))
    {
        static std::string path;
        ImGui::Text("Specify path for file");
        ImGui::InputText("##1", &path);
        if (ImGui::Button("Open"))
        {
            scene->readFromFile(path);
            scene->sendToShader(shader);
            path = "";
            ImGui::CloseCurrentPopup();
            return;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            path = "";
            ImGui::CloseCurrentPopup();
            return;
        }
        ImGui::EndPopup();
    }
}

void RMUI::draw()
{

    if (!ImGui::Begin("Options"))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    menu();

    shaderOptions();

    objects();

    ImGui::End();
}
