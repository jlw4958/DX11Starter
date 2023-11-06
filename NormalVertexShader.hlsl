#include "ShaderStructsInclude.hlsli"

cbuffer ExternalData : register(b0)
{
	// matrices
	float4x4 world;
	float4x4 worldInvTranspose;
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
	float3 tangent : NORMAL;
	float2 uv : TEXTCOORD;
};

VertexToPixel_Normal main(VertexShaderInput input)
{
	VertexToPixel_Normal output;

	output.tangent = mul((float3x3)world, input.normal);

	// mul((float3x3)worldInvTranspose, input.normal);
	return output;
}