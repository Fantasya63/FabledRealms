#version 460 core
const float PI = 3.14159265359;

uniform sampler2D g_Depth;

//IBL

uniform samplerCube irradianceMap;
uniform samplerCube prefilteredMap;
uniform sampler2D brdfLUT;
uniform sampler2D waterNormal;

//Screen Information
uniform sampler2D sceneDepth;
uniform sampler2D screenTex;


uniform mat4 u_ViewMatrix;
uniform mat4 u_InvViewMatrix;
uniform mat4 u_ProjMatrix;
uniform mat4 u_InvProjMatrix;
uniform mat3 u_ViewWorldNormalMatrix;

uniform vec3 u_LightDir;

uniform float u_WaterWorldLevel;


in vec3 v_Normal;
in vec3 v_Position;
in mat3 v_TBN;


// Lighting Settings
const float ambientStrength =  0.5f;







//PBR

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

vec2 GetScreenUV(vec3 pos)
{
    vec4 proj = u_ProjMatrix * vec4(pos, 1.0);
    proj.xyz /= proj.w;

    return proj.xy * vec2(0.5) + vec2(0.5);
}

vec3 GetViewPosFromDepth(vec2 uv, float depth)
{
    // Clip space position
    vec4 pos = vec4(vec3(uv, depth) * 2.0 - 1.0, 1.0);
    pos = u_InvProjMatrix * pos;

    //Perspective division
    pos.xyz = pos.xyz / pos.w;

    return pos.xyz;
}

void main()
{

    //Surface Properties
    vec4 color = vec4(0.0, 0.0, 0.3, 0.01);
    vec4 deepColor = vec4(0.05, 0.05, 0.14, 0.9);
    deepColor.rgb = pow(deepColor.rgb, vec3(2.2));
    float roughness = 0.1f;
    float density = 0.1;

    vec3 normalTex = texture(waterNormal, (u_InvViewMatrix * vec4(v_Position, 1.0)).xz * 0.05).rgb;
    vec3 N = v_TBN * normalTex;


    vec3 V = normalize(-v_Position);
    vec3 R = reflect(-V, N);
    

    vec3 radiance = vec3(0.99, 0.98, 0.82);//vec3(23.47, 21.31, 20.79);
    vec3 L = u_LightDir;
    vec3 H = normalize(V + L);

    vec3 F0 = vec3(0.04);
    vec3 F = fresnelSchlick(max(dot(V, H), 0.0), F0);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 numerator = NDF * G * F;
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    vec3 specular = numerator / denominator;


    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;


    //IBL
    F = fresnelSchlickRoughness(NdotV, F0, roughness);
    kS = F;
    kD = 1.0 - kS;
    //kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, u_ViewWorldNormalMatrix * N).rgb;
    vec3 diffuse = irradiance * color.rgb;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilteredMap, u_ViewWorldNormalMatrix * R, roughness *  MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT, vec2(NdotV, roughness)).rg;
    specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec2 screenUV = GetScreenUV(v_Position);
    float depth = texture(sceneDepth, screenUV).r;
    vec3 bottomPos = GetViewPosFromDepth(screenUV, depth);
    float opticalDepth = length(v_Position - bottomPos);

    vec3 bottomWorldPos = (u_InvViewMatrix * vec4(bottomPos, 1.0)).xyz;

    //AO
    vec3 ambient = (kD * diffuse * opticalDepth + specular) * ambientStrength;
    //float factor = min(1.0, opticalDepth / 16.0);
    vec4 waterColor = mix(deepColor, color, exp(-(u_WaterWorldLevel - bottomWorldPos.y) * density));

    //gl_FragColor = vec4(specular * exp(-opticalDepth), F);
    //gl_FragColor = vec4(ambient, F);
    //gl_FragColor = vec4(specular, F);
    vec3 backbuffer = texture(screenTex, screenUV).rgb;
    //(kD * albedo / PI + specular) * radiance * NdotL * shadow * AO;

    float beer = exp(-density * opticalDepth);
    vec3 albedo = mix( waterColor.rgb, backbuffer.rgb, beer);
    //albedo = mix( color.rgb, backbuffer.rgb, beer);
    gl_FragColor = vec4(kD * albedo / PI + specular, 1.0);
    //gl_FragColor = vec4(mix(albedo, specular, F), 1.0);

   

    //DEBUG
    //gl_FragColor = vec4(GetScreenUV(v_Position), 0.0, 1.0);
    //gl_FragColor = vec4(vec3(depth), 1.0);
    //gl_FragColor = vec4(vec3(length(v_Position - bottomPos)), 1.0);
    //gl_FragColor = vec4(GetViewPosFromDepth(screenUV, depth), 1.0);
    //gl_FragColor = vec4(texture(screenTex, screenUV));
    //gl_FragColor = vec4(albedo, 1.0);
}  