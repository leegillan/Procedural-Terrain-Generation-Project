// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

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
    float2 padding;
	//    
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
	float texBound : PSIZE;
};

OutputType main(InputType input)
{
    OutputType output;
    
	output.texBound = mul(input.position, worldMatrix).y;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    
    output.multiLightPos1 = mul(input.position, worldMatrix).xyz;
    output.multiLightPos2 = mul(input.position, worldMatrix).xyz;
    output.multiLightPos3 = mul(input.position, worldMatrix).xyz;
    output.multiLightPos4 = mul(input.position, worldMatrix).xyz;
   
    return output;
}