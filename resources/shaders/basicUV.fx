uniform extern float4x4 gWorld;
uniform extern float4x4 gView;
uniform extern float4x4 gProjection;

uniform extern texture gTexture0;

sampler texture0Sampler = sampler_state
{
	Texture = <gTexture0>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct vs_input
{
	float3 position :POSITION;
	float2 uv : TEXCOORD0;
};

struct vs_output
{
	float4 position : POSITION;
	float2 uv : TEXCOORD0;
};

vs_output vs_main(vs_input input)
{
	vs_output output = (vs_output)0;

	output.position = mul(float4(input.position, 1.0f), gWorld);
	output.position = mul(output.position, gView);
	output.position = mul(output.position, gProjection);

	output.uv = input.uv;
	return output;
}

float4 ps_main(vs_output input) : COLOR
{
	return tex2D(texture0Sampler, input.uv);
}

technique BasicUV
{
	pass p0
	{
		vertexShader = compile vs_2_0 vs_main();
		pixelShader = compile ps_2_0 ps_main();
	}
}
