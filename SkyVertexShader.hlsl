#include "ShaderStructsInclude.hlsli"

cbuffer ExternalData : register(b0)
{
	// matrices
	float4x4 view;
	float4x4 projection;
}

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
    output.position = mul(float4(input.localPosition, 1), mul(projection, viewCpy));

	// set output depth to 1.0
	output.position.z = output.position.w;

	// output sample direction
	output.sampleDir = input.localPosition;

	return output;
}