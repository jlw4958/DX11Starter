#include "ShaderStructsInclude.hlsli"

cbuffer ExternalData : register(b0)
{
	// matrices
	float4x4 world;
	float4x4 worldInvTranspose;
	float4x4 view;
	float4x4 projection;
}

VertexToPixel_Normal main(VertexShaderInput input) // add all the other stuff
{
	// Set up output struct
    VertexToPixel_Normal output;

    matrix wvp = mul(projection, mul(view, world));
    output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));
    output.worldPosition = mul(world, float4(input.localPosition, 1)).xyz; // local pos x world matrix (only first 3 components of the vector)
    output.normal = mul((float3x3)worldInvTranspose, input.normal); // inverse transpose of world matrix cast to 3x3 x input normal
    output.uv = input.uv;

	// tangent
	output.tangent = mul((float3x3)world, input.tangent);

	return output;
}