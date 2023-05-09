#version 440 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_UV;

out vec2 v_UV;
out vec3 v_Pos;
out vec3 v_Normal;

uniform mat4 a_ProjMatrix;
uniform mat4 a_ViewMatrix;
uniform mat4 a_ModelMatrix;


void main()
{
	v_Normal = a_Normal;
	vec4 pos =  a_ProjMatrix * a_ViewMatrix * a_ModelMatrix * vec4(a_Pos, 1.0);
	v_Pos = (a_ModelMatrix * vec4(a_Pos, 1.0)).xyz;
	v_UV = a_UV;
	gl_Position = pos;
}