
static const uint MAX_SHADOWMAP_AMOUNT = 4;


cbuffer cbufferPerFrame  : register(b0)
{

	matrix view;
	matrix projection;
	matrix invViewProjMatrix;
	float4 camPos;
	float4 mousePos;
	int numPointLights;
	int numSpotLights;
	int numDirLights;
	float pad;

};
cbuffer worldConstantBuffer	: register(b1)
{
	matrix world;
};

cbuffer lightBuffer			: register(b2)
{
	float4 lightPosition;
	matrix lightView;
	matrix lightProjection;
	float4 lightDir;
	float4 lightDiffuse;
	float intensity;
	float lightRange;
	float attenuation;
	float lPad;
	bool castShadow;
};
cbuffer textureSampleBuffer : register(b3)
{	
	bool diffuseMap;
	bool normalMap;
	bool specularMap;
	bool glowMap;
};


struct PointLight
{
	float4 lightPosition;
	matrix lightView;
	matrix lightProjection;
	float4 lightLookAt;
	float4 lightDiffuse;
	float intensity;
	float3 padI;
	float lightRange;
	float3 padR;
	float attenuation;
	float3 padA;
	bool castShadow;
};


struct DirectionalLight
{
	float4 lightPosition;
	matrix lightView;
	matrix lightProjection;
	float4 lightLookAt;
	float4 lightDiffuse;

	float intensity;
	float3 padI;

	bool castShadow;
	float3 padShadow;

};
struct Material
{
	float4 diffuseSample	;
	float4 normal			;
	float4 specular			;
	float4 shadow			;

};


struct VS_IN
{
	float3 Pos			 : POSITION;
	float2 Uv			 : TEXCOORD0;
};

struct VS_OUT
{

	float4 Pos			 : SV_POSITION;
	float2 Uv			 : TEXCOORD0;

};

//Vertex shader
VS_OUT VS_main( VS_IN input)
{
	VS_OUT output;
	output.Pos				 = float4(input.Pos, 1.0f);
	output.Uv				 = input.Uv;
	

	return output;
}


SamplerState	 linearSampler					 : register(s0);
SamplerState	 pointSampler					 : register(s1);

Texture2D		diffuseTexture					 : register(t0);
Texture2D		specularTexture					 : register(t1);
Texture2D		normalTexture					 : register(t2);
Texture2D		depthTexture					 : register(t3);
Texture2D		positionTexture					 : register(t4);
Texture2D		glowTexture						 : register(t5);
Texture2DArray  shadowTex						 : register(t6);

StructuredBuffer<PointLight>		pointlights  : register(t8);
StructuredBuffer<DirectionalLight>  dirLights    : register(t9);

static const float SSAO_RAD = 0.005f;

float sampleShadowStencils(float4 worldPos, matrix lightView, matrix lightProj,int shadowMapIndex)
{

	//shadowmap stuff
	float shadowSamples = 0.0f;

	float4 shadowSample = float4(1, 1, 1, 1);
	float tempCooef = 0;
	float SMAP_SIZE = 512.0f;
	
	float bias;
	float dx = 1.0f / SMAP_SIZE;

		float2 projectTexCoord;
		float depthValue;
		float lightDepthValue;
		float lightIntensity;
		float4 lightPos;
		//worldPos.xyz = worldPos.xyz / worldPos.w;


		//lightPos = mul(worldPos, lightView);
		//lightPos = mul(lightPos, lightProj);

		//projectTexCoord.x = lightPos.x / lightPos.w;
		//projectTexCoord.y = lightPos.y / lightPos.w;

		//lightDepthValue = lightPos.z / lightPos.w;

		//projectTexCoord.x = projectTexCoord.x * 0.5f + 0.5f;
		//projectTexCoord.y = projectTexCoord.y * -0.5f + 0.5f;
		//

		//depthValue = shadowTex.Sample(linearSampler, float3(projectTexCoord.xy, shadowMapIndex)).r + bias;

		////float tempSample = shadowTex.Sample(samplerTypeState, float3(projectTexCoord, i)).r

		//float s0 = (shadowTex.Sample(linearSampler, float3(projectTexCoord, shadowMapIndex)).r + bias < lightDepthValue) ? 0.0f : 1.0f;
		//float s1 = (shadowTex.Sample(linearSampler, float3(projectTexCoord, shadowMapIndex) + float3(dx, 0.0f, 0.0f)).r + bias < lightDepthValue) ? 0.0f : 1.0f;
		//float s2 = (shadowTex.Sample(linearSampler, float3(projectTexCoord, shadowMapIndex) + float3(0.0f, dx, 0.0f)).r + bias < lightDepthValue) ? 0.0f : 1.0f;
		//float s3 = (shadowTex.Sample(linearSampler, float3(projectTexCoord, shadowMapIndex) + float3(dx, dx, 0.0f)).r + bias < lightDepthValue) ? 0.0f : 1.0f;

		//float2 texelpos = projectTexCoord * SMAP_SIZE;
		//float2 lerps = frac(texelpos);
		//float shadowcooef = lerp(lerp(s0, s1, lerps.x), lerp(s2, s3, lerps.x), lerps.y);





		float4 posLightH = mul(float4(worldPos.xyz, 1.0f), lightView);
		posLightH = mul(posLightH, lightProj);
		posLightH.xy /= posLightH.w;

		float2 smTex = float2(posLightH.x, -posLightH.y) * 0.5f + 0.5f;

		float depth = posLightH.z / posLightH.w;

		smTex -= float2(dx, dx) * 0.5f;

		////////////////BIAS IS HERE
		bias = 0.0001f;

		//16 samples == -2 till 2
		//8 samples == -1 till 1
		[unroll]
		for (int k = -1; k < 1; k++) 
			[unroll]
			for (int l = -1; l < 1; l++)
				shadowSamples += shadowTex.Sample( linearSampler , float3(smTex + float2(dx * k, dx * l),0) ).r + bias < depth ? 0.0f : 1.0f;

			float shadowFactor = shadowSamples  *0.125f;//divisin by 8      // * 0.0625f; // division by 16.0f;


		//tempCooef += shadowcooef;
	
	//shadowSample = shadowSample * tempCooef;
	//shadowSample = saturate(shadowSample);
	return shadowFactor;



}




void ComputePointLight(PointLight light, Material mat, float3 wPos,float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	//Initialize output
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec	= float4(0.0f, 0.0f, 0.0f, 0.0f);


	//Vector from the surface to the light
	float3 lightVec = light.lightPosition.xyz - wPos;

	//Distance of lightVec
	float d = length(lightVec);
	
	//Range test
	if (d > light.lightRange)
		return;

	//Normalize the light vector
	lightVec /= d;

	//ambient = mat.ambient * light.ambient; <-- none of which exists

	//Add diffuse and specular term if the surface is in line of sight of the light

	float fDot = dot(lightVec, mat.normal.xyz);
	
	[flatten]
	if (fDot > 0.0f)
	{
		float3 v = reflect(-lightVec, mat.normal.xyz);
		float specFactor = pow(max(dot(v, toEye),0.0f),20.0f) ; //20.0f SHOULD BE SHINYPOWER, which will be in mat.specular.w

		diffuse = fDot * mat.diffuseSample * light.lightDiffuse;
		spec	= specFactor * mat.specular * light.lightDiffuse; //3rd multiplier SHOULD BE LIGHTS SPECULAR COLOR, BUT WE HAVE NONE, WE USE LIGHT COLOR

	}
	//Attenuate
	float att = 1.0f / dot(light.attenuation, float3(1.0f, d, d*d));

	float denom = d / light.lightRange + 1.0f;
	float attenuation = min(lerp(1.0f, 0.0f, d / (light.lightRange * 2.0f)), 1.0f / (denom * denom));

	diffuse   = diffuse * att* light.intensity;
	spec	  = spec    * att* light.intensity;

}



void ComputeDirectionalLight(DirectionalLight light, Material mat, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	//Initialize output
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec	= float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -normalize(light.lightLookAt.xyz - light.lightPosition.xyz);

	//ambient = mat.ambient * light.ambient; <-- none of which exists

	//Add diffuse and specular term if the surface is in line of sight of the light

	float fDot = dot(lightVec, mat.normal.xyz);

	[flatten]
	if (fDot > 0.0f)
	{
		float3 v = reflect(-lightVec, mat.normal.xyz);
		float specFactor = pow(max(dot(v, toEye), 0.0f), 20.0f); //20.0f SHOULD BE SHINYPOWER, which will be in mat.specular.w
		diffuse = fDot * mat.diffuseSample * light.lightDiffuse;
		spec = specFactor * mat.specular * light.lightDiffuse; //3rd multiplier SHOULD BE LIGHTS SPECULAR COLOR, BUT WE HAVE NONE, WE USE LIGHT COLOR
	}

}
float4 PS_main(VS_OUT input) : SV_TARGET
{

	int shadowMapsProcessed = 0;

	float depthSample				= depthTexture.Sample(pointSampler, input.Uv).x;

	float4 depthPrepare				= mul(float4(input.Uv.x *2.0f - 1.0f, input.Uv.y * -2.0f + 1.0f, depthSample, 1.0f), invViewProjMatrix);
	
	float3 worldPos					= positionTexture.Sample(pointSampler, input.Uv).xyz; //depthPrepare / depthPrepare.w;


	float4 diffuseSamp			= diffuseTexture.Sample(pointSampler,input.Uv);
	float4 normal					= normalTexture.Sample(pointSampler, input.Uv);
	float4 specularSample				= specularTexture.Sample(pointSampler, input.Uv);
	float shadow = 1.0;// = sampleShadowStencils(float4(worldPos, 1.0f));

	Material pixelMat;
	pixelMat.diffuseSample	    = diffuseSamp	;
	pixelMat.normal				= normal		;
	pixelMat.specular			= specularSample;
	pixelMat.shadow				= shadow		;
	//The light ray from the vert position to the light
	//normalized to be used as a direction vector
	
	float4 ambient		= float4(0.0f, 0.0f, 0.0f, 0.0f); // Hardcoded ambient. 
	float4 diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f); 
	float4 specular		= float4(0.0f, 0.0f, 0.0f, 0.0f); 

	float4 A, D, S;

			
	float3 v				= normalize(camPos.xyz - worldPos);				 //create a ray from the vert pos to the camera.
	
	for (int i = 0; i < numPointLights; i++)
	{

			ComputePointLight(pointlights[i], pixelMat, worldPos, v,
				A, D, S);
				if (pointlights[i].castShadow != 0) //if the light casts shadows
				{
					if (shadowMapsProcessed < (int)MAX_SHADOWMAP_AMOUNT)
					{

						shadow += sampleShadowStencils(float4(worldPos, 1.0f), pointlights[i].lightView, pointlights[i].lightProjection, shadowMapsProcessed);
						shadowMapsProcessed += 1;
					}

					ambient		+= A;
					diffuse		+= shadow * D;
					specular	+= shadow * S;
				}
				else
				{
					ambient += A;
					diffuse += D;
					specular += S;
				}

	}

	for (int d = 0; d < numDirLights; d++)
	{
			ComputeDirectionalLight(dirLights[d], pixelMat, v, A, D, S);
		if (dirLights[d].castShadow != 0) //if the light casts shadows
		{
			if (shadowMapsProcessed < (int)MAX_SHADOWMAP_AMOUNT)
			{
				
				shadow += sampleShadowStencils(float4(worldPos, 1.0f), dirLights[d].lightView, dirLights[d].lightProjection, shadowMapsProcessed);
				shadowMapsProcessed += 1;
			ambient += A;
			diffuse += shadow  *D;
			specular+= shadow  *S;
			}
		}
		else
		{
			ambient += A;
			diffuse += D;
			specular+= S;
		}

	}

	//saturate(diffuse);
	//saturate(specular);
	//float4 combinedLightDiffuse = { 0.0f,0.0f,0.0f,1.0f };

	//for (int i = 0; i < numPointLights; i++)
	//{

	//	float3 vRay				= (float3)(pointlights[i].lightPosition - worldPos);
	//	float rayLength			= length(vRay);
	//	//if (rayLength > pointlights[i].lightRange)
	//		//continue;
	//	vRay					/= rayLength;

	//						
	//	float3 r				= reflect(-vRay, normal.xyz);						 //Reflect is used in the specular shading
	//						
	//	float fDot				= saturate(dot(vRay, normal.xyz));					 //Calculate how much of the pixel is to be lit "intensity"


	//	float4 lightColor		= mul(pointlights[i].lightDiffuse, pointlights[0].intensity);
	//	float shinyPower		= 20.0f;//specularTexture.Sample(SampleType,input.Uv).r; //How much light is to be reflected
	//						
	//	float3 KS				= specular.xyz;									 //This is the color of the specularity. For now it is set as the light color
	//						
	//	float3 specularLight	= { KS * pow(max(dot(r,v),0.0f),shinyPower) };
	//			
	//	float4 lightDiffuse		= saturate(lightColor * fDot ) + float4(specularLight, 1.0f);
	//

	//
	//	float3 spotDir			= normalize(pointlights[i].lightLookAt - lightPosition);

	//	//lightDiffuse			*= pow(max(dot(-vRay, spotDir), 0.0f), 30.0f);

	//	float radio			= 30.0f;
	//	float lightLength	= length(pointlights[i].lightLookAt - worldPos);
	//	if (lightLength > radio)
	//		lightDiffuse *= 0.0;
	//	else
	//		lightDiffuse *= ((radio - lightLength) / radio);
	//	combinedLightDiffuse += lightDiffuse;
	//}
	


	//========== Screen Space Ambient Occlusion =============
	 //quite fake. Use AO maps if possible. dis should be for objects without AO maps
	/*float ssao = 0.0f;

	float2 vec[8] = { 
		float2(1.0f, 0.0f),		 float2(-1.0f, 0.0f),
		float2(0.0f, 1.0f),		 float2(0.0f, -1.0f),
		float2(0.707f, 0.707f),  float2(-0.707f, -0.707f),
		float2(-0.707f, 0.707f), float2(0.707f, -0.707f), };

	[unroll]
	for (int j = 0; j < 8; j++)
	{
		for (float k = 0.0f; k < 1.01f; k += 0.5f)
		{
			float2  sampleVec = input.Uv + vec[j] * SSAO_RAD * k;
			float	ssaoDepthSample = depthTexture.Sample(pointSampler, sampleVec).x;
			float4  ssaoDepthPrepare = mul(float4(sampleVec.x * 2.0f - 1.0f, sampleVec.y * -2.0f + 1.0f, ssaoDepthSample, 1.0f), invViewProjMatrix);
			float3 dif = ssaoDepthPrepare.xyz / ssaoDepthPrepare.w - worldPos;
			float l = length(dif) * 0.05f;
			dif = normalize(dif);

			ssao += max(0.0f, dot(normal.xyz, dif) - 0.01f) * 1.0f / (10.0f + l);
		}
	}

	ssao = 1.0f - ssao / 16.0f;*/



	//float4 finalCol = diffuseSample * ( ambient * ssao + (combinedLightDiffuse * shadow) );
	//finalCol.r += diffuseSample.a; //Laser point color
	//finalCol = saturate(finalCol);
	float4 glow = glowTexture.Sample(linearSampler, input.Uv);
	glow *= glow.a;
	float4 finalCol = saturate(ambient /** ssao*/ + (diffuse + specular)+glow);//* shadow));
	finalCol.r += diffuseSamp.a; //Laser point color
	finalCol.w = 1.0f;

	//justglow
	//float4 specularSample = specularTexture.Sample(pointSampler, input.Uv);

	return glow;
	//return glow;
}





