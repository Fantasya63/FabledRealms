#version 440 core

out vec4 FragColor;

in vec2 v_UV;
in vec3 v_Pos;
in vec3 v_Normal;

uniform sampler2D blockTex;
uniform float u_Time;


void main()
{
	vec4 albedo = texture(blockTex, v_UV);
	if (albedo.a < 0.5)
		discard;

	float light = max(0.0, dot(v_Normal, normalize(vec3(1.0, 1.0, 2.0))));
	vec3 diff = light * albedo.rgb;
	vec3 ambient = vec3(0.1, 0.1, 0.1);
	
	vec3 color = diff + ambient * albedo.rgb;

	//FOG
	float fogStart = 48.0;
	float fogEnd = 256.0;

	float depth = length(v_Pos);
	float density = 0.01;

	float fog = clamp(0.0, 1.0, (fogEnd - depth) / (fogEnd - fogStart));

	color = mix(vec3(0.8, 0.8, 1.0), color, fog);
	

	FragColor = vec4(color, 0.0);

	//Gamma Correction
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}