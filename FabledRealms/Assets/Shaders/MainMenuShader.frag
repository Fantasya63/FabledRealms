#version 440 core

out vec4 FragColor;

in vec2 v_UV;

uniform float u_Time;
uniform vec2 u_ScreenRes;
uniform sampler2D MenuTex;

void main()
{
	float aspect = u_ScreenRes.x / u_ScreenRes.y;
	vec2 uv = v_UV;
	uv -= vec2(0.5);
	uv.x *= aspect;
	uv += vec2(0.5);

	vec3 diff = texture(MenuTex, uv).rgb;
	vec3 blue = vec3(0.0, 0.75, 1.0) * diff;
	vec3 red = vec3(1.0, 0.0, 0.0) * diff;

	float blueFactor = sin(u_Time + v_UV.x + v_UV.y);
	//Scale it to 0-1 range
	blueFactor = blueFactor * 0.5 + 0.5;

	vec3 color = mix(red, blue, blueFactor);

	FragColor = vec4(color, 1.0);
	
	//Gamma Correction
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}