uniform extern float4x4 gWorld;
uniform extern float4x4 gView;
uniform extern float4x4 gProjection;

// Diffuse, Normal, Specular, Emission 순으로 0, 1, 2, 3이렇게 간다

uniform extern texture gTexture0;

sampler texture0Sampler = sampler_state
{
	Texture = <gTexture0>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

//---------------------------------------------------------------
// Base 관련
//---------------------------------------------------------------
struct vs_input 
{
   float4 position : POSITION0;
   float3 normal : NORMAL0;
	float2 texcoord : TEXCOORD0;

   //float3 binormal : BINORMAL0;
   //float3 tangent : TANGENT0;
   //float2 texcoord : TEXCOORD0;
};

struct vs_output 
{
   float4 position : POSITION0;
   float2 texcoord : TEXCOORD0;

   //float3 Normal : TEXCOORD1;
   //float3 Binormal : TEXCOORD2;
   //float3 Tangent : TEXCOORD3;
   //float3 viewDir : TEXCOORD4;
   //float3 worldPos : TEXCOORD5;
   //float4 FinalPos : TEXCOORD6;
};

vs_output vs_main( vs_input input )
{
   vs_output result = (vs_output)0;

   float4 worldPos = mul( input.position, gWorld );
   result.position = mul( worldPos, gView);
   result.position = mul( result.position, gProjection);
   
   result.texcoord = input.texcoord;
   
   //result.Normal = mul( Input.Normal, (float3x3)matWorld );
   //result.Binormal = mul( Input.Binormal, (float3x3)matWorld );  
   //result.Tangent = mul( Input.Tangent, (float3x3)matWorld ); 
   
   //result.viewDir = vEyePos.xyz - worldPos.xyz;
   //result.worldPos = worldPos;

   //result.FinalPos = Output.Position;

   return result;
}

float4 ps_main(vs_output input) : COLOR
{
	return tex2D(texture0Sampler, input.texcoord);
}

//--------------------------------------------------------------//
// Technique Section for Mesh
//--------------------------------------------------------------//
technique Base
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();
   }
}

