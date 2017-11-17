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
// Base ฐüทร
//---------------------------------------------------------------
struct vs_input 
{
   float3 position : POSITION0;
   float3 normal : NORMAL0;
   //float3 tangent : TANGENT0;
   //float3 binormal : BINORMAL0;
   float2 texcoord : TEXCOORD0;
};

struct vs_output 
{
   float4 position : POSITION0;
   float2 texcoord : TEXCOORD0;
   float3 normal : TEXCOORD1;
   float3 binormal : TEXCOORD2;
   float3 tangent : TEXCOORD3;
   float3 viewDir : TEXCOORD4;
   float3 worldPos : TEXCOORD5;
   float4 finalPos : TEXCOORD6;
};

vs_output vs_main( vs_input input )
{
   vs_output output = (vs_output)0;

   float4 worldPos = mul( float4(input.position, 1.0f), matWorld );
   output.position = mul( worldPos, matViewProjection );
   
   output.texcoord = input.texcoord;
   
   output.normal = mul( input.normal, (float3x3)matWorld );
   //output.binormal = mul( input.binormal, (float3x3)matWorld );  
   //output.tangent = mul( input.tangent, (float3x3)matWorld ); 

   output.binormal = float3(0.0f, 0.0f, 0.0f);
   output.tangent = float3(0.0f, 0.0f, 0.0f);
   
   output.viewDir = vEyePos.xyz - worldPos.xyz;
   output.worldPos = (float3)worldPos;

   output.finalPos = output.position;

   return( output );
}

float4 ps_main(vs_output input) : COLOR
{
	return tex2D(Diffuse, input.texcoord);
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

	  CullMode = None;
   }
}


