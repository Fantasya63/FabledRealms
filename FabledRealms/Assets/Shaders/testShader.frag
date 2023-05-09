#version 440 core

out vec4 FragColor;

in vec2 v_UV;
in vec3 v_Pos;
in vec3 v_Normal;

uniform sampler2D blockTex;
uniform float u_Time;


void main()
{
	vec3 diff = texture(blockTex, v_UV).rgb;
	vec3 ambient = vec3(0.1, 0.1, 0.4);
	float light = max(0.0, dot(v_Normal, normalize(vec3(1.0, 1.0, 2.0))));
	vec3 color = diff * (light  + ambient);

	//FOG
	float distance = max(0.0, length(v_Pos) - 32.0);
	float beer = 1.0 - exp(-distance * 0.002);
	beer = clamp(beer, 0.0, 1.0);

	color = mix(color, vec3(0.8, 0.8, 1.0), beer);
	

	FragColor = vec4(color, 0.0);

	//Gamma Correction
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}