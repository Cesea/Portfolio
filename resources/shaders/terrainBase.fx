float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;

float camNear;			//ī�޶� �ٰŸ� ���
float camFar;			//ī�޶� ���Ÿ� ���

float4x4 baseDirectionalLight;

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
   float4 FinalPos : TEXCOORD3;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = (VS_OUTPUT)0;

   Output.Position = mul( Input.Position, matWorld );
   Output.Position = mul( Output.Position, matViewProjection );
  
   Output.TileUV = Input.TileUV;
   Output.ControlUV = Input.BaseUV;
     
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
  
   Output.FinalPos = Output.Position;
   return( Output );
}



struct PS_INPUT 
{
   float2 TileUV : TEXCOORD0;
   float2 ControlUV : TEXCOORD1;
   float3 Normal : TEXCOORD2;
   float4 FinalPos : TEXCOORD3;
};

//�ȼ����̴� ��� ����ü
struct PS_OUTPUT
{
	float4 baseColor : COLOR0;			//0�� �������� �÷�
	float4 normalDepth : COLOR1;		//1�� �������� �÷� ( RGB �븻, A ���� )
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

PS_OUTPUT ps_main( PS_INPUT Input )
{   
	PS_OUTPUT  Output = (PS_OUTPUT)0;

   // Terrain Tile �÷��� ��´�.
   float3 terrain0 = tex2D( Terrain0, Input.TileUV ).rgb;
   float3 terrain1 = tex2D( Terrain1, Input.TileUV ).rgb;
   float3 terrain2 = tex2D( Terrain2, Input.TileUV ).rgb;
   float3 terrain3 = tex2D( Terrain3, Input.TileUV ).rgb;
  
   //Control Texture �� �÷��� ��´�.
   float3 controlColor = tex2D( TerrainControl, Input.ControlUV ).rgb;
   

   //��Ʈ�� Texture ���� ��������� ��´�.
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

   //��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
   float depth = Input.FinalPos.z / Input.FinalPos.w;

   //���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
   //Perspective Projection Linear Depth
   float z = depth;
   float a = camFar / (camFar - camNear);
   float b = -camNear / (camFar - camNear);
   depth = b / (z - a);

   Output.baseColor = float4(finalColor * diff, 1);
   //Output.baseColor = float4(diff, diff, diff, 1.0f);
   Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

   return Output;
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

