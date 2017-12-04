#include "ModelPixel.fx"

//
// 전역변수
//

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos : ViewPosition;

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

struct vs_diffuse_output 
{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
	float4 FinalPos : TEXCOORD3;
};

vs_diffuse_output vs_diffuse_main(VS_INPUT input)
{
	vs_diffuse_output result=  (vs_diffuse_output)0;
	float4 worldPos = mul( input.Position, matWorld );
	result.position = mul( worldPos, matViewProjection );

	result.texcoord = input.Texcoord;
	result.normal = input.Normal;

	result.worldPos = worldPos.rgb;
	result.FinalPos = result.position;

	return result;
}


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
   Output.worldPos = worldPos.xyz;

   Output.FinalPos = Output.Position;

   return( Output );
}


//---------------------------------------------------------------
// CreateShadow 관련
//---------------------------------------------------------------

struct VS_INPUT_SHADOW
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT_SHADOW
{
   float4 Position : POSITION0;
   float4 FinalPos : TEXCOORD0;
   float2 Texcoord : TEXCOORD1;
};

VS_OUTPUT_SHADOW vs_CreateShadow(VS_INPUT_SHADOW Input)
{
	VS_OUTPUT_SHADOW Output = (VS_OUTPUT_SHADOW)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.FinalPos = Output.Position;

   Output.Texcoord = Input.Texcoord;

   return( Output );
}


//--------------------------------------------------------------//
// Render With ShadowMap 
//--------------------------------------------------------------//

struct VS_INPUT_RECIVESHADOW
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal : NORMAL0;
   float3 Binormal : BINORMAL0;
   float3 Tangent : TANGENT0;
};

struct vs_output_recieve_shadow_foliage
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;

	float4 FinalPos : TEXCOORD3;
	float4 LightClipPos : TEXCOORD4;		//광원 입장에서 바라본 위치
};


struct VS_OUTPUT_RECIVESHADOW
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Binormal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 viewDir : TEXCOORD4;
	float3 worldPos : TEXCOORD5;

	float4 FinalPos : TEXCOORD6;
	float4 LightClipPos : TEXCOORD7;		//광원 입장에서 바라본 위치
};

VS_OUTPUT_RECIVESHADOW vs_ReciveShadow( VS_INPUT_RECIVESHADOW Input )
{
   VS_OUTPUT_RECIVESHADOW Output = (VS_OUTPUT_RECIVESHADOW)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.Texcoord = Input.Texcoord;
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   Output.Binormal = mul( Input.Binormal, (float3x3)matWorld );  
   Output.Tangent = mul( Input.Tangent, (float3x3)matWorld ); 
   
   Output.viewDir = vEyePos.xyz - worldPos.xyz;
   Output.worldPos = worldPos.xyz;

   Output.FinalPos = Output.Position;		//변환 정보
   Output.LightClipPos = mul( worldPos, matLightViewProjection );	//광원 입장에서 본 위치

   return( Output );
}

vs_output_recieve_shadow_foliage vs_foliage_main( VS_INPUT_RECIVESHADOW Input )
{
   vs_output_recieve_shadow_foliage Output = (vs_output_recieve_shadow_foliage)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.Texcoord = Input.Texcoord;
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   //Output.Binormal = mul( Input.Binormal, (float3x3)matWorld );  
   //Output.Tangent = mul( Input.Tangent, (float3x3)matWorld ); 
   
   //Output.viewDir = vEyePos.xyz - worldPos.xyz;
   Output.worldPos = worldPos;

   Output.FinalPos = Output.Position;
   Output.LightClipPos = mul( worldPos, matLightViewProjection );	//광원 입장에서 본 위치

   return( Output );
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

//technique Rock
//{
 //  pass Pass_0
  // {
   //   VertexShader = compile vs_3_0 vs_main();
    //  PixelShader = compile ps_3_0 ps_main();
   //}
//}

//technique Tree
//{
//	pass Pass_0
//	{
//		VertexShader = compile vs_3_0 vs_main();
//		PixelShader = compile ps_3_0 ps_main();
//
//		AlphaTestEnable = true;
//		AlphaFunc = GreaterEqual;
//		AlphaRef = 100;
//	}
//}

//technique Grass
//{
//	pass Pass_0
//	{
//		VertexShader = compile vs_3_0 vs_diffuse_main();
//		PixelShader = compile ps_3_0 ps_diffuse();
//
//		AlphaTestEnable = true;
//		AlphaFunc = GreaterEqual;
//		AlphaRef = 180;
//	}
//}

technique Toon
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_Toon();
	}
}

technique CreateShadow
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_CreateShadow();
      PixelShader = compile ps_3_0 ps_CreateShadow();
   }
}

technique ReciveShadow
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_ReciveShadow();
      PixelShader = compile ps_3_0 ps_ReciveShadow();
   }
}

technique ReciveShadowFoliage
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vs_foliage_main();
		PixelShader = compile ps_3_0 ps_recieve_shadow_foliage();
	}
}

technique ReciveShadowToon
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vs_ReciveShadow();
		PixelShader = compile ps_3_0 ps_ReciveShadowToon();
	}
}

technique Distort
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vs_ReciveShadow();
		PixelShader = compile ps_3_0 ps_Distort();
	}
}
