#version 440 core

layout (location = 0) out vec4 g_PositionEmission;
layout (location = 1) out vec4 g_ColorMetallic;
layout (location = 2) out vec4 g_NormalRoughness;

in vec2 v_UV;
in vec3 v_Pos;
in vec3 v_Normal;

uniform sampler2D texColor;
uniform sampler2D texNormal;
uniform sampler2D texARM; //AO, Roughness, Metallic


void main()
{
	vec4 albedo = texture(texColor, v_UV);
	if (albedo.a < 0.5)
		discard;

	g_PositionEmission = vec4(v_Pos.rgb, 0.0);
	g_ColorMet
} 