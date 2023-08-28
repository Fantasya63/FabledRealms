#version 460 core

uniform sampler2D u_SrcTexture;
uniform vec2 u_SrcResolution;
in vec2 texCoord;
out vec4 fragColor;
 
const float spread;


void main()
{
	//Blur the scene
	
	
	// a, b, c
	// d, e, f
	// g, h, i
	//Box blur where e is the current pixel
	
	
	vec3 a = texture(u_SrcTexture, texcoord + (vec2(-1.0,  1.0) * spread) / u_SrcResolution).rgb;
	vec3 b = texture(u_SrcTexture, texcoord + (vec2( 0.0,  1.0) * spread) / u_SrcResolution).rgb;
	vec3 c = texture(u_SrcTexture, texcoord + (vec2( 1.0,  1.0) * spread) / u_SrcResolution).rgb;
	vec3 d = texture(u_SrcTexture, texcoord + (vec2(-1.0,  0.0) * spread) / u_SrcResolution).rgb;
	vec3 e = texture(u_SrcTexture, texcoord + (vec2( 0.0,  0.0) * spread) / u_SrcResolution).rgb;
	vec3 f = texture(u_SrcTexture, texcoord + (vec2( 1.0,  0.0) * spread) / u_SrcResolution).rgb;
	vec3 g = texture(u_SrcTexture, texcoord + (vec2(-1.0, -1.0) * spread) / u_SrcResolution).rgb;
	vec3 h = texture(u_SrcTexture, texcoord + (vec2( 0.0, -1.0) * spread) / u_SrcResolution).rgb;
	vec3 i = texture(u_SrcTexture, texcoord + (vec2( 1.0, -1.0) * spread) / u_SrcResolution).rgb;
	
	fragColor = (a + b + c + d + e + f + g + h + i) / 9.0;
}