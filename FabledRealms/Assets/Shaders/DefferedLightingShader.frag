#version 330 core

out vec4 FragColor;

in vec2 v_UV;

uniform sampler2D PositionEmissionTex;
uniform sampler2D ColorMetallicTex;
uniform sampler2D NormalRoughnessTex;
uniform sampler2DArray shadowMap;

uniform float farPlane;

layout (std140) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1

//IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilteredMap;
uniform sampler2D brdfLUT;

const float PI = 3.14159265359;

uniform vec3 LightDir;
uniform vec3 camPos;
uniform mat4 view;

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

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

float ShadowCalculation(vec3 fragPosWorldSpace, vec3 normal, out int oLayer)
{
    // select cascade layer
    vec4 fragPosViewSpace = view * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope);
    float bias = 0.0005;//max(0.05 * (1.0 - dot(normal, LightDir)), 0.005);
    const float biasModifier = 0.05f;
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
        
        oLayer = layer;
    return shadow;
}


void main()
{
    //Sample G-Buffer
    vec4 PosEmission = texture(PositionEmissionTex, v_UV);
    vec4 ColMetallic = texture(ColorMetallicTex, v_UV);
    vec4 NorRoughness = texture(NormalRoughnessTex, v_UV);
    float roughness = NorRoughness.a;
    //roughness = 0.049;

    //vec4 PosEmission = texture(PositionEmissionTex, v_UV);
    //vec4 ColMetallic = vec4(1.0, 0.0, 0.0, 0.0);
    //vec4 NorRoughness = vec4(texture(NormalRoughnessTex, v_UV).rgb, 0.0);
    //float roughness = NorRoughness.a;

    
    //Light Color

   
    //Surface Properties
    vec3 N = NorRoughness.xyz * 2.0 - 1.0;
    vec3 V = normalize(camPos - PosEmission.xyz);
    vec3 R = reflect(-V, N);


    vec3 radiance = vec3(0.99, 0.98, 0.82);//vec3(23.47, 21.31, 20.79);
    //radiance = vec3(1.0, 0.387, 0.0) * 10;
    vec3 L = normalize(LightDir);
    vec3 H = normalize(V + L);

    //vec4 lightSpacePos = lightSpaceMatrix * vec4(PosEmission.xyz, 1.0);
    int layer;
    float shadow = 1.0 - ShadowCalculation(PosEmission.xyz, N, layer);


    vec3 albedo = ColMetallic.rgb;

    //TODO Change to use the LabPBR's standard
    float metallic = 0.0;
    if (ColMetallic.a > 229.0 / 255.0)
    {
        metallic = 1.0;
    }


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

    FragColor = vec4((kD * albedo / PI + specular) * radiance * NdotL * shadow, 1.0);


    //Ambient lighting
    F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    kS = F;
    kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilteredMap, R, roughness *  MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    specular = prefilteredColor * (F * brdf.x + brdf.y);


    //AO
    vec3 ambient = (kD * diffuse + specular) * PosEmission.a * 0.5;

    FragColor += vec4(ambient, 1.0);

    

   

    //FOG
	vec3 fogColor = vec3(0.8, 0.8, 1.0);
    fogColor = vec3(1.0, 0.37, 0.23);
    //fogColor = vec3(1.0, 1.0, 0.33);
    float fogStart = 48.0;
	float fogEnd = 256.0;

	float depth = length(camPos - PosEmission.xyz);
	float density = 0.01;

	float fog = clamp(0.0, 1.0, (fogEnd - depth) / (fogEnd - fogStart));

	FragColor.rgb = mix(fogColor, FragColor.rgb, fog);


    //if (layer == 1)
    //    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    //
    //else if (layer == 2)
    //    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //
    // else if (layer == 3)
    //    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    // else if (layer == 4)
    //    FragColor = vec4(0.0, 0.0, 1.0, 1.0);



    //FragColor = vec4(vec3(shadow), 1.0);
    //FragColor = vec4(N, 1.0);
    //FragColor = vec4(vec3(max(dot(R, L), 0.0)), 1.0);
    //FragColor = vec4(vec3(PosEmission.a), 1.0);
}