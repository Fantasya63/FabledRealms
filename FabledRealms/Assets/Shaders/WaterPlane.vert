#version 460 core

layout (location = 0) in vec3 a_Pos;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec3 a_Tangent;

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ProjMatrix;
uniform mat3 u_ViewNormalMatrix;

out mat3 v_TBN;
out vec3 v_Normal;
out vec3 v_Position;

void main()
{
    v_Normal = u_ViewNormalMatrix * a_Normal;
    v_Position = (u_ModelViewMatrix * vec4(a_Pos, 1.0)).xyz;

    vec3 T = normalize(u_ViewNormalMatrix * a_Tangent);
	vec3 N = normalize(u_ViewNormalMatrix * a_Normal);
	vec3 B = normalize(cross(N, T)); // Maybe the order is wrong?

	mat3 TBN = mat3(T, B, N);
    v_TBN = TBN;

    gl_Position = u_ProjMatrix * vec4(v_Position, 1.0);
}  