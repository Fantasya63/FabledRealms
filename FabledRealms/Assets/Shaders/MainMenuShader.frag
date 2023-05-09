#version 440 core

out vec4 FragColor;

in vec2 v_UV;

uniform float u_Time;
uniform vec2 u_ScreenRes;
uniform sampler2D MenuTex;

void main()
{
	vec3 color = vec3(0.0, 0.75, 1.0) * (sin(u_Time + v_UV.x + v_UV.y) * 0.5 + 0.5) * 0.5;
	color = max(color, vec3(0.3, 0.1, 0.1));
	color += texture(MenuTex, v_UV).rgb;

	FragColor = vec4(color, 1.0);
	
	//Gamma Correction
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}