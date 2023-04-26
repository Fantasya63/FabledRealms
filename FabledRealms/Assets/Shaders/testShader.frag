#version 440 core

out vec4 FragColor;

in vec2 v_UV;
in vec3 v_Pos;
uniform sampler2D blockTex;
uniform float u_Time;


void main()
{
	vec2 UV = vec2(v_UV.x + sin(u_Time * 0.5), v_UV.y);
	FragColor = texture(blockTex, UV);//vec4(color, 0.0);
	//FragColor = vec4(v_UV, 0.0, 1.0);
}