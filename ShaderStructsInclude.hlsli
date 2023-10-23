#ifndef __GGP_SHADER_INCLUDES_STRUCTS__ // Each .hlsli file needs a unique identifier!
#define __GGP_SHADER_INCLUDES_STRUCTS__

struct VertexToPixel
{

    float4 screenPosition : SV_POSITION;
    float2 uv : TEXTCOORD;
    float4 color : COLOR; // RGBA color

};

#endif