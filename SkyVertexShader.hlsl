#include "ShaderStructsInclude.hlsli"

cbuffer ExternalData : register(b0)
{
	// matrices
	float4x4 view;
	float4x4 projection;
}

struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 localPosition : POSITION;     // XYZ position
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTCOORD;
};

VertexToPixel_Sky main(VertexShaderInput input)
{
	VertexToPixel_Sky output;

	// copy view matrix
	float4x4 viewCpy = view;

	// set translations to 0
	viewCpy._14 = 0;
	viewCpy._24 = 0;
	viewCpy._34 = 0;

	// apply projection & view to position
	output.position = mul(input.localPosition, mul(projection, viewCpy));

	// set output depth to 1.0
	output.position.z = output.position.w;

	// output sample direction
	output.sampleDir = input.localPosition;

	return output;
}