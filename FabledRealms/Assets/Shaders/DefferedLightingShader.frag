#version 440 core

out vec4 FragColor;

in vec2 v_UV;

uniform sampler2D PositionEmissionTex;
uniform sampler2D ColorMetallicTex;
uniform sampler2D NormalRoughnessTex;

void main()
{
    vec3 color = texture(ColorMetallicTex, v_UV).rgb;

    FragColor = vec4(color, 1.0);
}