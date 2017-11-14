uniform extern float4x4 gWorld;
uniform extern float4x4 gView;
uniform extern float4x4 gProjection;

struct vs_input 
{
   	float3 position : POSITION0;
   	float3 normal : NORMAL0;
   	float2 texcoord : TEXCOORD0;
};

struct vs_output 
{
   float4 position : POSITION0;
   float2 texcoord : TEXCOORD0;
};

vs_output vs_main( vs_input input )
{
   vs_output result = (vs_output)0;

   float4 worldPos = mul(float4(input.position, 1.0f), gWorld);
   result.position = mul(worldPos, gView);
   result.position = mul(result.position, gProjection);
   
   //result.texcoord = input.texcoord;

   return result;
};

float4 ps_main(vs_output input) : COLOR
{
	return float4(0.0f, 1.0f, 1.0f, 1.0f);
};

technique Base
{
   pass pass0
   {
      vertexShader = compile vs_2_0 vs_main();
      pixelShader = compile ps_2_0 ps_main();

	  FillMode = WireFrame;
   }
}

