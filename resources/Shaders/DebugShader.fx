uniform extern float4x4 matWorld;
uniform extern float4x4 matViewProjection;

struct vs_input
{
	float3 position : POSITION0;
	float4 color : COLOR0;
};

struct vs_output
{
	float4 position : POSITION0;
	float4 color : COLOR0;
};

vs_output vs_main(vs_input input)
{
	vs_output result = (vs_output)(0);

	float4 worldPos = mul(float4(input.position, 1.0f), matWorld);
	result.position = mul(worldPos, matViewProjection);

	result.color = input.color;
	return result;
}

float4 ps_main(vs_output input) : COLOR
{
	return input.color;
}

technique Base
{
	pass p0
	{
		vertexShader = compile vs_3_0 vs_main();
		pixelShader = compile ps_3_0 ps_main();
	}
};
