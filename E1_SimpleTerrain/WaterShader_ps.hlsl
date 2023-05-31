// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 multiLightPosition[4];
	float4 ambientColour;
	float4 diffuseColour;
	//
	float3 lightDirection;
	float blendVal;
	//
	float4 multiLightDiffuse[4];
	//
};
struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 multiLightPos1 : TEXCOORD1;
	float3 multiLightPos2 : TEXCOORD2;
	float3 multiLightPos3 : TEXCOORD3;
	float3 multiLightPos4 : TEXCOORD4;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse, float mul)
{
    float intensity = saturate(dot(normal, lightDirection) * mul);
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 lightColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = texture0.Sample(sampler0, input.tex);

    textureColour.a = blendVal;

	float3 multiLightVec1 = normalize(multiLightPosition[0].xyz - input.multiLightPos1);
	float3 multiLightVec2 = normalize(multiLightPosition[1].xyz - input.multiLightPos2);
	float3 multiLightVec3 = normalize(multiLightPosition[2].xyz - input.multiLightPos2);
	float3 multiLightVec4 = normalize(multiLightPosition[3].xyz - input.multiLightPos3);

	// Multiply the texture pixel by the combination of all four light colors to get the final result.
	//lightColour = ambientColour + calculateLighting(-lightDirection, input.normal, diffuseColour, 1.0f);
	float4 multiLightColour1 = calculateLighting(-multiLightVec1, input.normal, multiLightDiffuse[0], 1.0f);
	float4 multiLightColour2 = calculateLighting(-multiLightVec2, input.normal, multiLightDiffuse[1], 1.0f);
	float4 multiLightColour3 = calculateLighting(-multiLightVec3, input.normal, multiLightDiffuse[2], 1.0f);
	float4 multiLightColour4 = calculateLighting(-multiLightVec4, input.normal, multiLightDiffuse[3], 1.0f);

	lightColour = ambientColour + calculateLighting(-lightDirection, input.normal, diffuseColour, 1.0f);

	lightColour += saturate(multiLightColour1 + multiLightColour2 + multiLightColour3 + multiLightColour4);

	return lightColour * textureColour;
}



