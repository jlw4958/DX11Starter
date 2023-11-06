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

// smapler for textures!
SamplerState BasicSampler : register(s0);

Texture2D SurfaceTexture : register(t0);
Texture2D SurfaceSpecular : register(t1);
Texture2D SurfaceNormal : register(t2);

float4 main(VertexToPixel_Normal input) : SV_TARGET
{
    // normalize lights
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent);

    // normal mapping
    float3 unpackedNormal = SurfaceNormal.Sample(BasicSampler, input.uv).rgb * 2 - 1;
    unpackedNormal = normalize(unpackedNormal); // Don’t forget to normalize!
    
    T = normalize(T - N * dot(T, N)); // Gram-Schmidt assumes T&N are normalized!
    float3 B = cross(T, N);
    float3x3 TBN = float3x3(T, B, N);
    
    // Assumes that input.normal is the normal later in the shader
    input.normal = mul(unpackedNormal, TBN); // Note multiplication order!
    
    // Adjust the variables below as necessary to work with your own code
    float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, input.uv).rgb;
    float specScale = SurfaceSpecular.Sample(BasicSampler, input.uv).r;
    
    // after making 3 lights, add together with ambientColor to make finalColor; return final color
    float3 finalColor = ambientColor * surfaceColor;
    
    float3 finalLight;
    for (int i = 0; i < TOTAL_LIGHTS; i++)
    {
        
        if (lights[i].Type == LIGHT_TYPE_DIRECTIONAL)
        {
            finalLight = DirLight(lights[i], input.normal, surfaceColor, roughness, cameraPosition, input.worldPosition, specScale);
        }
        if (lights[i].Type == LIGHT_TYPE_POINT)
        {
            finalLight = PointLight(lights[i], input.normal, surfaceColor, roughness, cameraPosition, input.worldPosition, specScale);
        }
        finalColor += finalLight;
    }
    
    return float4(finalColor, 1.0);
}