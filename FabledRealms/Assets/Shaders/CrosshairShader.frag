#version 440 core

out vec4 FragColor;

in vec2 v_UV;

uniform float u_Time;
uniform vec2 u_ScreenRes;

uniform sampler2D DiffuseTex;

void main()
{
	vec4 diff = texture(DiffuseTex, v_UV);
	//if (diff.a < 0.5)
		//discard;

	FragColor = diff;
}