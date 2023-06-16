#version 460 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_UV;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec3 a_Tangent;

out vec2 v_UV;
out vec3 v_Pos;
out vec3 v_Normal;
out mat3 v_TBN;

uniform mat4 a_ProjMatrix;
uniform mat4 a_ViewMatrix;
uniform mat4 a_ModelMatrix;


void main()
{
	vec4 pos =  a_ProjMatrix * a_ViewMatrix * a_ModelMatrix * vec4(a_Pos, 1.0);
	gl_Position = pos;

	v_Pos = (a_ViewMatrix * a_ModelMatrix * vec4(a_Pos, 1.0)).xyz;
	
	v_UV = a_UV;

	vec3 T = normalize(vec3(a_ModelMatrix * vec4(a_Tangent, 0.0)));
	vec3 N = normalize(vec3(a_ModelMatrix * vec4(a_Normal, 0.0)));
	vec3 B = normalize(cross(N, T)); // Maybe the order is wrong?

	mat3 TBN = mat3(T, B, N);
	v_TBN = TBN;

	v_Normal = a_Normal;
}