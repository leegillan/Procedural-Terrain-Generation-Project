/* See the file "LICENSE" for the full license governing this code. */

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBuffer : register(b1)
{
    float time;
    float fps;
    float padding;
    //
}

cbuffer WaveBuffer : register(b2)
{
    float amplitude;
    float speed;
    float steepness;
    bool wavePad;
    //
    float3 freq;
    float waveHeight;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 multiLightPos1 : TEXCOORD1;
	float3 multiLightPos2 : TEXCOORD2;
	float3 multiLightPos3 : TEXCOORD3;
	float3 multiLightPos4 : TEXCOORD4;
};



OutputType main(InputType input)
{
    OutputType output;
    input.position.w = 1.0f;

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	float3 direction = float3(1, 1, 1) * freq;
 
	input.position.y = waveHeight;

	//Moves the wave with modifiable values
	input.position.y += (float)(((steepness / direction * amplitude * 4) * amplitude) * direction * cos(dot(direction, input.position.xyz) + speed * time));
    input.normal.y += (float) (((steepness / direction * amplitude * 4) * amplitude) * direction * cos(dot(direction, input.position.xyz) + speed * time));

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) worldMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);

	output.multiLightPos1 = mul(input.position, worldMatrix).xyz;
	output.multiLightPos2 = mul(input.position, worldMatrix).xyz;
	output.multiLightPos3 = mul(input.position, worldMatrix).xyz;
	output.multiLightPos4 = mul(input.position, worldMatrix).xyz;

    return output;

 

}
 