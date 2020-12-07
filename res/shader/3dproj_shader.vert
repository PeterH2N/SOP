#version 130 

void main()
{
    gl_Position = gl_Vertex;

    //forward the color
    gl_FrontColor = gl_Color;
};