#version 460 core

out vec4 FragColor;

in vec2 v_UV;

uniform float u_Time;
uniform vec2 u_ScreenRes;

uniform sampler2D DiffuseTex;

void main()
{
	float aspect = u_ScreenRes.x / u_ScreenRes.y;
	vec2 uv = v_UV;
	uv -= vec2(0.5);
	uv.x *= aspect;
	//Scale the texture
	uv *= 10;

	uv += vec2(0.5);

	vec4 diff = texture(DiffuseTex, uv);
	if (diff.a < 0.5)
		discard;

	FragColor = diff;
}