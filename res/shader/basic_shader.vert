#version 130 core

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // forward the vertex color
    //gl_FrontColor = gl_Color;
};