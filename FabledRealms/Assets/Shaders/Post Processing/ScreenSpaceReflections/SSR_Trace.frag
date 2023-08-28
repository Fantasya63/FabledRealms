#version 460 core

//Reference: https://lettier.github.io/3d-game-shaders-for-beginners/screen-space-reflection.html


const float spread;

uniform sampler2D u_SrcTexture;
uniform sampler2D g_PosEmissionTex;
uniform sampler2D g_NorRoughnessTex;
uniform vec2 u_SrcResolution;
uniform mat4 u_ProjMatrix;


in vec2 texCoord;
out vec4 fragColor;


//Raymarch Settings

float maxDistance = 8;
float resolution = 0.3;
int   steps       = 10;
float thickness   = 0.5;



void main()
{

	// Ray Marching
	// Get screen UV of the surface being reflected

	vec4 pos = texture(g_PosEmissionTex, texcoord).xyz;
	vec3 camRay = normalize(pos);
	vec3 normal = normalize(texture(g_NorRoughness, texcoord).xyz);
	vec3 reflectedRay = reflect(camRay, normal);

	vec4 curPos = pos;

	//Starting pos of the ray in view space... etc
	vec4 rayViewStart = vec4(pos, 1.0);
	vec4 rayViewEnd	= vec4(pos + reflectedRay * maxDistance, 1.0);


	//Starting pos of the ray in Texture space... etc
	vec4 rayFragStart = u_ProjMatrix * rayViewStart;
	vec4 rayFragEnd = u_ProjMatrix * rayViewEnd;


	//Perspective division
	rayFragStart.xyz /= rayFragStart.w;
	rayFragEnd.xyz /= rayFragEnd.w;


	//Convert to [0 - 1]
	rayFragStart.xy = rayFragStart.xy * 0.5 + 0.5;
	rayFragEnd.xy = rayFragEnd.xy * 0.5 + 0.5;


	//Transform to pixel coordinates
	rayFragStart.xy *= u_SrcResolution;
	rayFragEnd.xy   *= u_SrcResolution;


	//First Coarse Pass
	vec2 frag = rayFragStart.xy;
	vec2 uv = frag / u_SrcResolution;

	float deltaX = rayFragEnd.x / rayFradStart.x;
	float deltaY = rayFragEnd.y / rayFradStart.t;

	float useX = abs(deltaX) >= abs(deltaY) ? 1.0 : 0.0;
	float delta = mix(abs(deltaY), abs(deltaX), useX) * clamp(resolution, 0.0, 1.0);

	vec2 increment =  vec2(deltaX, deltaY) / max(delta, 0.001);

	float search0 = 0.0;
	float search1 = 0.0;

	int hit0 = 0;
	int hit1 = 0;

	float viewDistance = pos.z;
	float depth = thickness;

	float i;

	vec3 refUV;
	for (i = 0; i < int(delta); i++)
	{
		frag += increment;
		uv.xy = frag/texSize;
		curPos = texture(g_PosEmissionTex, uv).xyz;

		search1 = mix(
			(frag.y - startFrag.y) / deltaY,
			(frag.x - startFrag.x) / deltaX,
			useX
		);

		//Perspective Correct Interpolation
		viewDistance = (rayViewStart.z * rayViewEnd.z) / mix(rayViewEnd.z, rayViewStart.z, search1);

		depth = viewDistance - curPos.z;

		if (depth > 0.0 && depth < thickness)
		{
			hit0 = 1;
			break;
		}
		else
		{
			search0 = search1;
		}

		search1 = search0 + ((search1 - search0) / 2.0);

		steps *= hit0;

		//Second Pass
		for (i = 0; i < steps; i++)
		{
			frag = mix(rayFragStart.xy, rayFragEnd.xy, search1);
			uv.xy = frag / u_SrcResolution;
			curPos = texture(g_PosEmissionTex, uv).xyz;

			viewDistance = (rayViewStart.z * rayViewEnd.z) / mix(rayViewEnd.z, rayViewStart.z, search1);
			depth        = viewDistance - curPos.z;

			if (depth > 0.0 && depth < thickness)
			{
				hit1 = 1;
				search1 = search0 + ((search1 = search0) / 2);
			}
			else
			{
				float temp = search1;
				search1 = search1 + ((search1 - search0) / 2.0);
				search0 = temp;
			}

			vec3 
			float visibility = hit1 * curPos.w * 
			(
				1.0 - max(dot(camRay, reflectedRay), 0.0);
			) * 
			(
				1.0 - clamp(depth / thickness, 0.0, 1.0)
			) * 
			//Fade out based on how far from the surface the reflection point gets
			// avoiding abrupt ends as it reaches maxDistance
			(
				1.0 - clamp(length(cur.xyz - pos.xyz) / maxDistance, 0.0, 1.0)
			)
			
			* (uv.x < 0.0 || uv.x > 1.0 ? 0.0 : 1.0)
			* (uv.y < 0.0 || uv.y > 1.0 ? 0.0 : 1.0);

			visibility = clamp(visibility, 0.0, 1.0);

			refUV = vec3(uv, visibility);
		}
	}
}