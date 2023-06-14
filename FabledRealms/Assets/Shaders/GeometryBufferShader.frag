#version 440 core

layout (location = 0) out vec4 g_PositionEmission;
layout (location = 1) out vec4 g_ColorMetallic;
layout (location = 2) out vec4 g_NormalRoughness;

in vec2 v_UV;
in vec3 v_Pos;
in vec3 v_Normal;
in mat3 v_TBN;

uniform sampler2D DiffuseTex;
uniform sampler2D RoughnessTex;
uniform sampler2D NormalTex;
uniform sampler2D MetallicTex;


void main()
{
	// LabPBR Standard: https://wiki.shaderlabs.org/wiki/LabPBR_Material_Standard
	// Specular
	// Red - Perceptual Smoothness
	// Green - F0 [0-229], Metal IDS [230-255]
	// Blue - Porousity
	// Alpha - Emissive

	// Normal - (DirectX)
	// Red - Normal x
	// Green - Normal y
	// Blue - AO


	vec4 albedo = texture(DiffuseTex, v_UV);
	vec4 specular = texture(RoughnessTex, v_UV);
	
	float roughness = 1.0 - specular.r;
	
	vec4 normalTex = texture(NormalTex, v_UV);
	normalTex.y = 1.0 - normalTex.y;

	vec3 normal = vec3(normalTex.x, normalTex.y, sqrt(1.0 - dot(normalTex.xy, normalTex.xy)));
	normal = normal * vec3(2.0) - vec3(1.0);
	//normal = normalize(v_TBN * normal);


	if (albedo.a < 0.5)
		discard;

	g_PositionEmission = vec4(v_Pos.rgb, normalTex.a);
	g_ColorMetallic = vec4(albedo.rgb, specular.g);
	g_NormalRoughness = vec4(v_Normal * 0.5 + 0.5, roughness);
} 