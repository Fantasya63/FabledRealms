#version 440 core

layout (location = 0) in vec3 a_Pos;
layout (location = 2) in vec2 a_UV;

out vec2 v_UV;

void main()
{
	v_UV = a_UV;
	gl_Position = vec4(a_Pos, 1.0);
}