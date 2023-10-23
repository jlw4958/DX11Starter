
cbuffer ExternalData : register(b0)
{
	float4 colorTint;
}

struct VertexToPixel
{

	float4 screenPosition : SV_POSITION;
	float2 uv : TEXTCOORD; 
	float4 color : COLOR;        // RGBA color

};

float4 main(VertexToPixel input) : SV_TARGET
{
	VertexToPixel output;
	
    output.uv.x = sin(input.uv.x);
    output.uv.y = cos(input.uv.y);
	
    output.color = colorTint * output.uv.x * output.uv.y;
	
    return output.color;
}