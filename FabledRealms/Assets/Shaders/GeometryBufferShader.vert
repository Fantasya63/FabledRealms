#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_UV;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in float a_AO;

out vec2 v_UV;
out vec3 v_Pos;
out vec3 v_Normal;
out mat3 v_TBN;
out float v_AO;

uniform mat4 a_ProjMatrix;
uniform mat4 a_ViewMatrix;
uniform mat4 a_ModelMatrix;
uniform mat4 a_ModelViewMatrix;

uniform mat3 a_ViewNormalMatrix;

void main()
{
	gl_Position =  a_ProjMatrix * a_ModelViewMatrix * vec4(a_Pos, 1.0);

	v_Pos = (a_ModelViewMatrix * vec4(a_Pos, 1.0)).xyz;
	
	v_UV = a_UV;

	vec3 T = normalize(a_ViewNormalMatrix * a_Tangent);
	vec3 N = normalize(a_ViewNormalMatrix * a_Normal);
	vec3 B = normalize(cross(N, T)); // Maybe the order is wrong?

	mat3 TBN = mat3(T, B, N);
	v_TBN = TBN;
	v_AO = a_AO;
	v_Normal = N;
	//vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
}