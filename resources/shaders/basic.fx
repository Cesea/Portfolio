uniform extern float4x4 gWorld;
uniform extern float4x4 gView;
uniform extern float4x4 gProjection;

struct VSInput
{
	float3 position :POSITION;
	float4 color : COLOR0;
};

struct VSOutput
{
	float4 position : POSITION;
	float4 color : COLOR0;
};


VSOutput vs_main(VSInput input)
{
	VSOutput output = (VSOutput)0;

	output.position = mul(float4(input.position, 1.0f), gWorld);
	output.position = mul(output.position, gView);
	output.position = mul(output.position, gProjection);

	output.color = input.color;
	return output;
}

float4 ps_main(VSOutput input) : COLOR
{
	return input.color;
}

technique Basic
{
	pass p0
	{
		vertexShader = compile vs_2_0 vs_main();
		pixelShader = compile ps_2_0 ps_main();
	}
}
