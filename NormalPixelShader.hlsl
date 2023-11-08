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
    // Feel free to adjust/simplify this code to fit with your existing shader(s)
    // Simplifications include not re-normalizing the same vector more than once!
    float3 N = input.normal = normalize(input.normal); // Must be normalized here or before
    float3 T = normalize(input.tangent); // Must be normalized here or before
    
    T = normalize(T - N * dot(T, N)); // Gram-Schmidt assumes T&N are normalized!
    float3 B = cross(T, N);
    float3x3 TBN = float3x3(T, B, N);
    
    // Adjust the variables below as necessary to work with your own code
    float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, input.uv).rgb;
    float specScale = SurfaceSpecular.Sample(BasicSampler, input.uv).r;
    //specScale *= any(diffuse);
    
    // unpack and sample normal
    float3 unpackedNormal = SurfaceNormal.Sample(BasicSampler, input.uv).rgb * 2 - 1;
    unpackedNormal = normalize(unpackedNormal); // Don’t forget to normalize!
    
    float3 finalColor = ambientColor * surfaceColor;
    
    // Assumes that input.normal is the normal later in the shader
    input.normal = mul(unpackedNormal, TBN); // Note multiplication order!
    
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