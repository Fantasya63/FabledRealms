#version 440 core

out vec4 FragColor;

in vec3 v_UV;
uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, v_UV);

    //Gamma Correction
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}