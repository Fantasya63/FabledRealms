#version 460 core

uniform sampler2D srcTexture;
//uniform vec2 srcResolution;

in vec2 texCoord;
layout (location = 0) out vec4 FragColor;

void main()
{
    vec3 outCol = texture(srcTexture, texCoord).rgb;
    float brightness = dot(outCol, vec3(0.2126, 0.7152, 0.0722));
    
    float threshold = smoothstep(0.0, 1.0, brightness);
    FragColor = vec4(outCol * threshold, 1.0);
}