
cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	float totalTime;
	//float greenValue;
}

struct VertexToPixel
{

	float4 screenPosition : SV_POSITION;
	float2 uv : TEXTCOORD; 

};

float4 main(VertexToPixel input) : SV_TARGET
{
	VertexToPixel output;
	
    //float x = input.uv.x;
    //float y = sin(x) * 2;
	
	//output.uv = float2(x, y);
	//if () {
	//	output.color = float4(1, 0, 0, 1);
	//}
	//else {
	//	output.color = float4(0, 0, 1, 1);
	//}

	//output.screenPosition = float4(sin(input.screenPosition.x), input.screenPosition.y, input.screenPosition.z, input.screenPosition.w);
	
    //output.color = y * float4(1, 0, 0, 1);
	
	//output.color = float4(1, 0, sqrt(sin(input.uv)));
	
    output.uv.x = sin(input.uv.x);
    output.uv.y = cos(input.uv.y);
	
    output.color = colorTint * output.uv.x * output.uv.y;

    //output.color = float4(colorTint.x * sin(input.uv.x * 5), colorTint.y * cos(input.uv.y * 5), colorTint.z * tan(input.uv.x * 5), 1);

	
    return output.color;
}