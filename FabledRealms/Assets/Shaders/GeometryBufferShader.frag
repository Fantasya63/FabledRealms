#version 440 core

layout (location = 0) out vec4 g_PositionEmission;
layout (location = 1) out vec4 g_ColorMetallic;
layout (location = 2) out vec4 g_NormalRoughness;

in vec2 v_UV;
in vec3 v_Pos;
in vec3 v_Normal;
in vec3 v_Tangent;
in mat3 v_TBN;

uniform sampler2D DiffuseTex;
uniform sampler2D RoughnessTex;
uniform sampler2D NormalTex;
uniform sampler2D MetallicTex;


void main()
{

	vec4 albedo = texture(DiffuseTex, v_UV);
	vec4 roughness = texture(RoughnessTex, v_UV);
	
	vec3 normal = texture(NormalTex, v_UV).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(v_TBN * normal);

	float metallic = texture(MetallicTex, v_UV).r;


	if (albedo.a < 0.5)
		discard;

	g_PositionEmission = vec4(v_Pos.rgb, 0.0);
	g_ColorMetallic = vec4(albedo.rgb, metallic);
	g_NormalRoughness = vec4(normal, roughness);
} 