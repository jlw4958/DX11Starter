#include "ShaderStructsInclude.hlsli"

#ifndef __GGP_SHADER_INCLUDES_FUNCTIONS__ // Each .hlsli file needs a unique identifier!
#define __GGP_SHADER_INCLUDES_FUNCTIONS__

// light functions
float3 Diffuse(float3 normal, float3 dirToLight)
{
    return saturate(dot(normal, dirToLight));
}

float3 ViewVector(float3 cameraPos, float3 pixelWorldPos)
{
    return float3(normalize(cameraPos - pixelWorldPos));
}

float3 Reflection(float3 lightDir, float3 normal)
{
    return reflect(lightDir, normal);
}

float3 DirLight(Light light, float3 normal, float4 surfaceColor, float3 ambient, float roughness, float3 cameraPos, float3 worldPos)
{
    float spec = 0;
    // directional light
    float3 lightNormalDir = normalize(-light.Direction);
    float3 diffusion = Diffuse(normal, lightNormalDir);

    float3 finalColor = (diffusion * light.Color * (float3) surfaceColor) + (ambient * (float3)surfaceColor);
    // specular
    float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;

    if (specExponent > 0.05f)
    {
        spec = pow(saturate(dot(Reflection(lightNormalDir, normal), ViewVector(cameraPos, worldPos))), specExponent);
    }
    
    return (diffusion * (float3)surfaceColor + spec) * light.Color;
}

#endif