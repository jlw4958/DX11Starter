#include "ShaderStructsInclude.hlsli"

TextureCube SkyTexture : register(t0);

SamplerState SkySampler : register(s0);

float4 main(VertexToPixel_Sky input) : SV_TARGET
{
	float3 skySample = SkyTexture.Sample(SkySampler, input.sampleDir);

	return float4(skySample, 1.0f);
}