#version 440 core

layout (location = 0) in vec3 a_Pos;

out vec3 v_UV;

uniform mat4 a_ProjMatrix;
uniform mat4 a_ViewMatrix;

void main()
{
	v_UV = a_Pos;
	vec4 pos = a_ProjMatrix * a_ViewMatrix * vec4(a_Pos, 1.0);
	
	//Optimization to cancel Perspective Division
	//From https://learnopengl.com/Advanced-OpenGL/Cubemaps
	gl_Position = pos.xyww;
}