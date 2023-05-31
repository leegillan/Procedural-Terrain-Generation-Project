// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

//Textures
Texture2D grassTexture : register(t0);
Texture2D snowTexture : register(t1);
Texture2D rockTexture : register(t2);
Texture2D sandTexture : register(t3);

//Sampler
SamplerState sampler0 : register(s0);

//Constants
cbuffer LightBuffer : register(b0)
{
    float4 multiLightPosition[4];
    float4 ambientColour;
    float4 diffuseColour;
    //
    float3 lightDirection;
    float waterHeight;
    //
    float4 multiLightDiffuse[4];
    //
};

//Structs
struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 multiLightPos1 : TEXCOORD1;
    float3 multiLightPos2 : TEXCOORD2;
    float3 multiLightPos3 : TEXCOORD3;
    float3 multiLightPos4 : TEXCOORD4;
	float texBound : PSIZE;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse, float intensityMul)
{
    float intensity = saturate(dot(normal, lightDirection) * intensityMul);
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float4 grassColour;
    float4 snowColour;
    float4 rockColour;
	float4 sandColour;

    float4 textureColour;
    float4 lightColour;
   
    float texBlend;
    float slope;

	// Sample the textures. Calculate light intensity and colour, return light*texture for final pixel colour.
    grassColour = grassTexture.Sample(sampler0, input.tex);
    snowColour = snowTexture.Sample(sampler0, input.tex);
    rockColour = rockTexture.Sample(sampler0, input.tex);
	sandColour = sandTexture.Sample(sampler0, input.tex);
    
    // Calculate the slope of this point.
    slope = 1.0f - input.normal.y;
    
    // Determine which texture to use based on slope.
    if (slope < 0.2f)
    {
        texBlend = slope / 0.2f;
        textureColour = lerp(grassColour, rockColour, texBlend);
    }

    if ((slope < 0.7f) && (slope >= 0.2f))
    {
        texBlend = (slope - 0.2f) * (1.0f / (0.7f - 0.2f));
        textureColour = lerp(rockColour, grassColour, texBlend);
    }
	
    if (slope >= 0.6f)
    {
		texBlend = slope / 0.6f;
        textureColour = lerp(rockColour, grassColour, texBlend);
    }
	
	// Determine which texture to use based on slope.
	if (input.texBound < waterHeight - 5)
	{
		texBlend = slope / 0.8f;
		textureColour = lerp(sandColour, grassColour, 0.4f);
	}

	if (input.texBound > 35.0f)
	{
		texBlend = slope / 0.8f;
		textureColour = lerp(snowColour, rockColour, 0.3f);
	}

    float3 multiLightVec1 = normalize(multiLightPosition[0].xyz - input.multiLightPos1);
    float3 multiLightVec2 = normalize(multiLightPosition[1].xyz - input.multiLightPos2);
    float3 multiLightVec3 = normalize(multiLightPosition[2].xyz - input.multiLightPos2);
    float3 multiLightVec4 = normalize(multiLightPosition[3].xyz - input.multiLightPos3);
    
    // Multiply the texture pixel by the combination of all four light colors to get the final result.

    lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour, 1.0f);
    float4 multiLightColour1 = calculateLighting(-multiLightVec1, input.normal, multiLightDiffuse[0], 1.0f);
    float4 multiLightColour2 = calculateLighting(-multiLightVec2, input.normal, multiLightDiffuse[1], 1.0f);
    float4 multiLightColour3 = calculateLighting(-multiLightVec3, input.normal, multiLightDiffuse[2], 1.0f);
    float4 multiLightColour4 = calculateLighting(-multiLightVec4, input.normal, multiLightDiffuse[3], 1.0f);
    
    lightColour = ambientColour + calculateLighting(-lightDirection, input.normal, diffuseColour, 1.0f);
    
    lightColour += saturate(multiLightColour1 + multiLightColour2 + multiLightColour3 + multiLightColour4);
    
    return lightColour * textureColour;
}

