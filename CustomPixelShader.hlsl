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
	
	//float x = input.uv.x;
	//float y = sin(x);
	//output.uv = float2(x, y);
	//if () {
	//	output.color = float4(1, 0, 0, 1);
	//}
	//else {
	//	output.color = float4(0, 0, 1, 1);
	//}

	//output.screenPosition = float4(sin(input.screenPosition.x), input.screenPosition.y, input.screenPosition.z, input.screenPosition.w);
	//output.color = float4(1, 0, sqrt(sin(input.uv)));
	return output.color;
}