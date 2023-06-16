#version 460 core

out vec4 FragColor;

in vec3 v_UV;
uniform samplerCube CubemapTex;

void main()
{    
    FragColor = texture(CubemapTex, v_UV);

    //Gamma Correction
	//float gamma = 2.2;
    //FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}