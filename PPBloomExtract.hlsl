#include "ShaderStructsInclude.hlsli"
#include "ShaderFunctionsInclude.hlsli"

cbuffer externalData : register(b0)
{
    float bloomThreshold;
}

Texture2D BrightTexture : register(t0);
SamplerState samples : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	// grab pixel color
    float4 finalColor = BrightTexture.Sample(samples, input.uv);
	
	// if color is higher than bloom threshold, return	
    return (finalColor - bloomThreshold, 1);
}