#version 440 core

out vec4 FragColor;

uniform float u_Time;

void main()
{
	vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), abs(sin(u_Time)));
	FragColor = vec4(color, 0.0);
}