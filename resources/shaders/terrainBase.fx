float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;

float camNear;			//카메라 근거리 평면
float camFar;			//카메라 원거리 평면

float4 vEyePosition;

float4x4 baseDirectionalLight;

static float fogStart = 2.0f;
static float fogRange = 150.0f;
static float3 fogColor = {0.4f, 0.4f, 0.4f};

//-----------------------------------------------------------------------------
// Base
//-----------------------------------------------------------------------------

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL0;
   float3 BiNormal : BINORMAL0;
   float3 Tangent : TANGENT0;
   float2 BaseUV : TEXCOORD0;
   float2 TileUV : TEXCOORD1;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 TileUV : TEXCOORD0;
   float2 ControlUV : TEXCOORD1;
   float3 Normal : TEXCOORD2;
   float FogIntensity : TEXCOORD4;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = (VS_OUTPUT)0;

   float4 worldPosition = mul( Input.Position, matWorld );
   Output.Position = mul( worldPosition, matViewProjection );
  
   Output.TileUV = Input.TileUV;
   Output.ControlUV = Input.BaseUV;
     
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   float dist = distance(worldPosition, vEyePosition);
   Output.FogIntensity = saturate((dist - fogStart) / fogRange);
  
   return( Output );
}



struct PS_INPUT 
{
   float2 TileUV : TEXCOORD0;
   float2 ControlUV : TEXCOORD1;
   float3 Normal : TEXCOORD2;
   float FogIntensity : TEXCOORD4;
};


texture Terrain0_Tex;
sampler2D Terrain0 = sampler_state
{
   Texture = (Terrain0_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture Terrain1_Tex;
sampler2D Terrain1 = sampler_state
{
   Texture = (Terrain1_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture Terrain2_Tex;
sampler2D Terrain2 = sampler_state
{
   Texture = (Terrain2_Tex);
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture Terrain3_Tex;
sampler2D Terrain3 = sampler_state
{
   Texture = (Terrain3_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MAXANISOTROPY = 0;
};

texture TerrainControl_Tex;
sampler2D TerrainControl = sampler_state
{
   Texture = (TerrainControl_Tex);
   MAGFILTER = ANISOTROPIC;
   MAXANISOTROPY = 16;
   MINFILTER = ANISOTROPIC;
   MIPFILTER = ANISOTROPIC;
};

float4 ps_main( PS_INPUT Input ) : COLOR0
{   
   // Terrain Tile 컬러를 얻는다.
   float3 terrain0 = tex2D( Terrain0, Input.TileUV ).rgb;
   float3 terrain1 = tex2D( Terrain1, Input.TileUV ).rgb;
   float3 terrain2 = tex2D( Terrain2, Input.TileUV ).rgb;
   float3 terrain3 = tex2D( Terrain3, Input.TileUV ).rgb;
  
   //Control Texture 의 컬러를 얻는다.
   float3 controlColor = tex2D( TerrainControl, Input.ControlUV ).rgb;
   

   //컨트롤 Texture 에서 색상비율을 얻는다.
   float redFactor = controlColor.r;
   float greenFactor = controlColor.g;
   float blueFactor = controlColor.b;
   float blackFactor = saturate( 1.0f - ( redFactor +  greenFactor + blueFactor ) );

   float3 texColor1 = terrain0 * redFactor;
   float3 texColor2 = terrain1 * greenFactor;
   float3 texColor3 = terrain2 * blueFactor;
   float3 texColor4 = terrain3 * blackFactor;
   
   float3 finalColor = texColor1 + texColor2 + texColor3 + texColor4;

   float3 worldNormal = normalize(Input.Normal);

   float3 lightDir = -float3(baseDirectionalLight._21, baseDirectionalLight._22, baseDirectionalLight._23);
   lightDir = normalize(lightDir);

   float diff = saturate(dot(worldNormal, lightDir));

   return float4(lerp(finalColor * diff, fogColor, Input.FogIntensity), 1.0f);
}

float bias = 0.01f;


technique Base
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();
   }
}

technique WireFrame
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
		
		FillMode = WireFrame;
	}
}
