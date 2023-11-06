#include "ShaderStructsInclude.hlsli"
#include "ShaderFunctionsInclude.hlsli"

#define TOTAL_LIGHTS 5

cbuffer ExternalData : register(b0)
{
    float3 colorTint;
    float roughness;
    float3 cameraPosition;
    float3 ambientColor;

    // lights array!
    Light lights[TOTAL_LIGHTS];
}

SamplerState BasicSampler : register(s0);

Texture2D NormalMap : register(t0);

float4 main(VertexToPixel_Normal input) : SV_TARGET
{
    float3 unpackedNormal = NormalMap.Sample(BasicSampler, input.uv).rgb * 2 - 1;
    unpackedNormal = normalize(unpackedNormal); // Don’t forget to normalize!



    return float4(1,1,1,1);
}