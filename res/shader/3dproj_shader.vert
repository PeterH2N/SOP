#version 330
attribute vec3 position;
attribute vec3 normal;
attribute vec3 color;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

varying vec3 norm;
varying vec3 fragPos;
varying vec3 col;

void main() 
{
	gl_Position = proj * view * model * vec4(position, 1.0);
	fragPos = vec3(model * vec4(position, 1.0f));
	norm = mat3(transpose(inverse(model))) * normal;
	col = color;
}