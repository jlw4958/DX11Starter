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

float Attenuate(Light light, float3 worldPos)
{
    float dist = distance(light.Position, worldPos);
    float att = saturate(1.0f - (dist * dist / (light.Range * light.Range)));
    return att * att;
}

float SpecCalc(float rough, float3 dir, float3 normal, float3 camPos,float3 worldPos)
{
    float spec = 0;
    float specExponent = (1.0f - rough) * MAX_SPECULAR_EXPONENT;
    if (specExponent > 0.05f)
    {
        spec = pow(saturate(dot(Reflection(dir, normal), ViewVector(camPos, worldPos))), specExponent);
    }
    
    return spec;
}

float3 DirLight(Light light, float3 normal, float3 colorTint, float roughness, float3 cameraPos, float3 worldPos, float specScale)
{
    // normalize direction
    float3 lightNormalDir = normalize(-light.Direction);
    
    // diffusion
    float3 diffusion = Diffuse(normal, lightNormalDir);
    
    // specular
    float spec = SpecCalc(roughness, lightNormalDir, normal, cameraPos, worldPos) * specScale;
    
    return (diffusion * colorTint + spec) * light.Color;
}

float3 PointLight(Light light, float3 normal, float3 colorTint, float roughness, float3 cameraPos, float3 worldPos, float specScale)
{
    // direction
    float3 pointDir = normalize(light.Position - worldPos);
    
    // normalize direction
    float3 lightNormalDir = normalize(-pointDir);
    
    // diffusion
    float3 diffusion = Diffuse(normal, lightNormalDir);
    
    // specular
    float spec = SpecCalc(roughness, lightNormalDir, normal, cameraPos, worldPos) * specScale;
    
    // attenuation
    float attenuate = Attenuate(light, worldPos);

    return (diffusion * colorTint + spec) * attenuate * light.Color;
};

#endif