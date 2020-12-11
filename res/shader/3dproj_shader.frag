#version 330


varying vec3 norm;
varying vec3 fragPos;
varying vec3 col;

uniform float time;

vec3 lightPos = vec3(0, 10, 6);


float ambientLight = 0.2;

float getDiff(vec3 n)
{
	lightPos.xz += vec2(sin(time), cos(time)) * 2;
	vec3 lightDir = normalize(lightPos - fragPos);
	return clamp(dot(n, lightDir), ambientLight, 1.0f);
}

void main() 
{
	vec3 color = col;
	vec3 normal = normalize(norm);
	color *= getDiff(normal);
	gl_FragColor = vec4(color, 1);
			
}