#version 440 core

out vec4 FragColor;

in vec3 v_UV;
uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, v_UV);
}