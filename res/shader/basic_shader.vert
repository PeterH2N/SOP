#version 130 

void main()
{
    // oversætter bare koordinater til at ligge mellem -1 og 1
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

};