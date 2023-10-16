cbuffer ExternalData : register(b0)
{
	float4 colorTint;
}

struct VertexToPixel
{

	float4 screenPosition : SV_POSITION;
	float2 uv : TEXTCOORD; 

};

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1, 0, 1, 1);

}