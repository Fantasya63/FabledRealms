#version 460 core

out vec4 FragColor;

in vec2 v_UV;

uniform sampler2D scene;
uniform sampler2D bloom;

void main()
{
    vec3 hdr = texture(scene, v_UV).rgb;
    vec3 bloomCol = texture(bloom, v_UV).rgb + hdr;

    //hdr = mix(hdr, bloomCol, 0.5);
    
    //Tone Mapping
    // reinhard tone mapping
    vec3 mapped = hdr / (hdr + vec3(1.0));

    //Gamma Correction
	float gamma = 2.2;
    mapped = pow(mapped.rgb, vec3(1.0/gamma));

    FragColor = vec4(mapped, 1.0);
}