#version 460 core

out vec4 FragColor;

in vec2 v_UV;

uniform sampler2D PositionEmissionTex;
uniform sampler2D ColorMetallicTex;
uniform sampler2D NormalRoughnessTex;


 const float PI = 3.14159265359;


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

void main()
{
    //Sample G-Buffer
    vec4 PosEmission = texture(PositionEmissionTex, v_UV);
    vec4 ColMetallic = texture(ColorMetallicTex, v_UV);
    vec4 NorRoughness = texture(NormalRoughnessTex, v_UV);


    //Surface Properties
    vec3 N = NorRoughness.xyz * 2.0 - 1.0;
    vec3 V = normalize(-PosEmission.xyz);
    vec3 albedo = ColMetallic.rgb;

    float metallic = 0.0;
    if (ColMetallic.a > 0.5)
    {
        metallic = 1.0;
    }

    vec3 lightColor = vec3(1.0, 1.0, 1.0);//vec3(23.47, 21.31, 20.79);
    vec3 L = normalize(vec3(0.8, 1.0, 1.0));
    vec3 H = normalize(V + L);

    //TODO - Use F0 input from textures
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(V, H), 0.0), F0);

    float NDF = DistributionGGX(N, H, NorRoughness.a);
    float G = GeometrySmith(N, V, L, NorRoughness.a);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;

    vec3 specular = numerator / denominator;


    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;


    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);

    vec3 ambient = vec3(0.03) * albedo;

    FragColor = vec4((kD * albedo / PI + specular) * lightColor * NdotL + ambient, 1.0);
    //FragColor = vec4(N, 1.0);
}