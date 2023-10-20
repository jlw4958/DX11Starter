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
	// UVs are usually between 0 and 1
	// have colors change over time?
	// how would we use this to draw a sine wave?
	// how can we have the

	float4 color;

	// get the number out of the 0 to 1 range and round down
	float solidNumU = trunc(input.uv.x * 10);
	float solidNumV = trunc(input.uv.y * 10);
	if (solidNumV % 2) {

		//if (greenValue < 1) {
		//	greenValue* totalTime;
		//}
		//if (greenValue >= 1) {
		//	greenValue / totalTime;
		//}

		//while (greenValue < 1.0f) {
		//	greenValue *= totalTime;
		//	color = float4(sin(input.uv.x), greenValue, .6f, 1.0f) * frac(sin(dot(input.uv, float2(12.9898, 78.233))) * 43758.5453123);
		//}
		//if (greenValue >= 1.0f) {
		//	greenValue = 0.1f;
		//}

		color = float4(sin(input.uv.x), input.uv.x*totalTime, .6f, 1.0f) * frac(sin(dot(input.uv, float2(12.9898, 78.233))) * 43758.5453123);
	}
	else {
		// red
		// add some noise!
		color = float4(cos(input.uv.y), 0.0f, .6f, 1.0f) * frac(cos(dot(input.uv, float2(12.9898, 78.233))) * 43758.5453123);
	}
	
    return color;
}