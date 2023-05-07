#version 440 core

layout (location = 0) in vec3 a_Pos;
layout (location = 2) in vec2 a_UV;

out vec2 v_UV;
out vec3 v_Pos;

uniform mat4 a_ProjMatrix;
uniform mat4 a_ViewMatrix;

void main()
{
	v_Pos = a_Pos;
	v_UV = a_UV;
	gl_Position = a_ProjMatrix * a_ViewMatrix * vec4(a_Pos, 1.0);
}