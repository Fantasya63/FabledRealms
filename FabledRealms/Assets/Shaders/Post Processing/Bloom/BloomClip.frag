#version 330 core

uniform sampler2D srcTexture;
//uniform vec2 srcResolution;

in vec2 texCoord;
layout (location = 0) out vec4 FragColor;

void main()
{
    vec3 outCol = texture(srcTexture, texCoord).rgb;
    float brightness = dot(outCol, vec3(0.2126, 0.7152, 0.0722));
    
    if (brightness > 1.0)
        FragColor = vec4(outCol, 1.0);
    else
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}