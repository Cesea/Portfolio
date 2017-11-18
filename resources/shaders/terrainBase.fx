float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;

//-----------------------------------------------------------------------------
// Base
//-----------------------------------------------------------------------------
struct vs_input 
{
   float3 position : POSITION0;
   float3 normal : NORMAL0;
   float3 biNormal : BINORMAL0;
   float3 tangent : TANGENT0;
   float2 baseUV : TEXCOORD0;
   float2 tileUV : TEXCOORD1;
};

struct vs_output
{
   float4 position : POSITION0;
   float2 tileUV : TEXCOORD0;
   float2 controlUV : TEXCOORD1;
   float3 normal : TEXCOORD2;
   float4 finalPos : TEXCOORD3;
};

vs_output vs_main( vs_input input )
{
   vs_output output = (vs_output)0;

   output.position = mul( float4(input.position, 1.0f), matWorld );
   output.position = mul( output.position, matViewProjection );
  
   output.tileUV = input.tileUV;
   output.controlUV = input.baseUV;
     
   output.normal = mul( input.normal, (float3x3)matWorld );
  
   output.finalPos = output.position;
   return( output );
}

struct ps_input 
{
   float2 tileUV : TEXCOORD0;
   float2 controlUV : TEXCOORD1;
   float3 normal : TEXCOORD2;
   float4 finalPos : TEXCOORD3;
};

//픽셀셰이더 출력 구조체
struct ps_output
{
	float4 baseColor : COLOR0;
	float4 normalDepth : COLOR1;
};

texture gTexture0;
sampler2D Terrain0 = sampler_state
{
   Texture = (gTexture0);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

texture gTexture1;
sampler2D Terrain1 = sampler_state
{
   Texture = (gTexture1);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

texture gTexture2;
sampler2D Terrain2 = sampler_state
{
   Texture = (gTexture2);
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
};

texture gTexture3;
sampler2D Terrain3 = sampler_state
{
   Texture = (gTexture3);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MAXANISOTROPY = 0;
};

texture gTexture4;
sampler2D TerrainControl = sampler_state
{
   Texture = (gTexture4);
   MAGFILTER = ANISOTROPIC;
   MAXANISOTROPY = 16;
   MINFILTER = ANISOTROPIC;
   MIPFILTER = ANISOTROPIC;
};

float4 ps_main( ps_input input ) : COLOR
{   
	//ps_output output = (ps_output)0;

   //Terrain Tile 컬러를 얻는다.
   float3 terrain0 = tex2D( Terrain0, input.tileUV ).rgb;
   float3 terrain1 = tex2D( Terrain1, input.tileUV ).rgb;
   float3 terrain2 = tex2D( Terrain2, input.tileUV ).rgb;
   float3 terrain3 = tex2D( Terrain3, input.tileUV ).rgb;
  
   //Control Texture 의 컬러를 얻는다.
   float3 controlColor = tex2D( TerrainControl, input.controlUV ).rgb;

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

   return  float4(finalColor , 1);
}

technique Base
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();

	  //CullMode  = None;
   }
}
