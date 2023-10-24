#include "ShaderStructsInclude.hlsli"
#include "ShaderFunctionsInclude.hlsli"

cbuffer ExternalData : register(b0)
{
    float4 colorTint;
    float roughness;
    float3 cameraPosition;
    float3 ambientColor;
    Light directionalLight1;
    Light directionalLight2;
    Light directionalLight3;
}

// smapler for textures!
SamplerState BasicSampelr : register(s0);

// you may end up with multiple of these; will be used for this is ngle draw
Texture2D SurfaceTexture : register(t0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{

    //return SurfaceTexture.Sample(BasicSampler, input.uv);

    float spec;
    
    // normalize incoming normal (input.normal)
    input.normal = normalize(input.normal);
    
    //// directional light
    //float3 lightNormalDir = normalize(-directionalLight1.Direction);
    //float3 diffusion = Diffuse(input.normal, lightNormalDir);
    
    float3 finalColor = (diffusion * directionalLight1.Color * (float3) colorTint) + (ambientColor * (float3) colorTint);
    //// specular
    //float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
    
    //if (specExponent > 0.05f)
    //{
    //    spec = pow(saturate(dot(Reflection(lightNormalDir, input.normal), ViewVector(cameraPosition, input.worldPosition))), specExponent);
    //}
    //else
    //{
    //    spec = 0;
    //}
    
    //float3 light = colorTint * (diffusion + spec); // Tint specular?
    
    // after making 3 lights, add together with ambientColor to make finalColor; return final color

    return float4(finalColor, 1.0);
    //return float4((diffusion * colorTint + spec) * directionalLight1.Color, 1.0f);
}