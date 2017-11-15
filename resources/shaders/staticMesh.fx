//#include "ModelPixel.fx"

//
// 전역변수
//

uniform extern float4x4 matWorld : World;
uniform extern float4x4 matViewProjection : ViewProjection;
uniform extern float4 vEyePos : ViewPosition;

uniform extern texture gTexture0;
sampler2D Diffuse = sampler_state
{
	Texture = <gTexture0>;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

uniform extern texture gTexture1;
sampler2D Normal = sampler_state
{
	Texture = <gTexture1>;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

uniform extern texture gTexture2;
sampler2D Specular = sampler_state
{
	Texture = <gTexture2>;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

//---------------------------------------------------------------
// Base 관련
//---------------------------------------------------------------
struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL0;
   float3 Tangent : TANGENT0;
   float3 Binormal : BINORMAL0;
   float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal : TEXCOORD1;
   float3 Binormal : TEXCOORD2;
   float3 Tangent : TEXCOORD3;
   float3 viewDir : TEXCOORD4;
   float3 worldPos : TEXCOORD5;
   float4 FinalPos : TEXCOORD6;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = (VS_OUTPUT)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.Texcoord = Input.Texcoord;
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   Output.Binormal = mul( Input.Binormal, (float3x3)matWorld );  
   Output.Tangent = mul( Input.Tangent, (float3x3)matWorld ); 
   
   Output.viewDir = vEyePos.xyz - worldPos.xyz;
   Output.worldPos = worldPos;

   Output.FinalPos = Output.Position;

   return( Output );
}

float4 ps_main(VS_OUTPUT input) : COLOR
{
	return tex2D(Diffuse, input.Texcoord);
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


