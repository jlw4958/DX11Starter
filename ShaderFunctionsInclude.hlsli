#ifndef __GGP_SHADER_INCLUDES_FUNCTIONS__ // Each .hlsli file needs a unique identifier!
#define __GGP_SHADER_INCLUDES_FUNCTIONS__

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

#endif