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
	vec3 color = diff * (max(0.0, dot(v_Normal, normalize(vec3(-5.0, 1.0, 1.0)))) + vec3(0.1, 0.1, 0.4));

	

	FragColor = vec4(diff, 0.0);

	//Gamma Correction
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));


	//FragColor = vec4(v_UV, 0.0, 1.0);
	//FragColor = vec4(v_Pos.xyz, 1.0);
}