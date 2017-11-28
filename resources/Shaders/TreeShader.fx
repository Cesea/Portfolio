uniform extern texture DiffuseTexture;
sampler DiffuseSampler = sampler_state
{
	Texture = <DiffuseTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};

uniform extern texture NormalTexture;
sampler NormalSampler = sampler_state
{
	Texture = <NormalTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};

uniform extern texture SpecularTexture;
sampler SpecularSampler = sampler_state
{
	Texture = <SpecularTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos : ViewPosition;

uniform extern float4x4 baseDirectionalLight;

//---------------------------------------------------------------
// Base ฐüทร
//---------------------------------------------------------------
struct vs_input 
{
   float4 position : POSITION0;
   float2 texcoord : TEXCOORD0;
   float3 normal : NORMAL0;
   float3 tangent : TANGENT0;
   float3 binormal : BINORMAL0;
};

struct vs_output 
{
   float4 position : POSITION0;
   float2 texcoord : TEXCOORD0;
   float3 lightDir : TEXCOORD1;
   float3 viewDir : TEXCOORD2;
};

vs_output vs_main( vs_input input )
{
   vs_output result = (vs_output)0;

   float4 worldPos = mul( input.position, matWorld );
   result.position = mul( worldPos, matViewProjection );

   float3x3 TNB;
   TNB[0] = mul(normalize(input.tangent), (float3x3)matWorld);
   TNB[1] = mul(normalize(input.binormal), (float3x3)matWorld);
   TNB[2] = mul(normalize(input.normal), (float3x3)matWorld);
   
   result.texcoord = input.texcoord;

   float3 lightDirection = -float3(baseDirectionalLight._21, baseDirectionalLight._22, baseDirectionalLight._23);

   result.lightDir = normalize(mul(TNB, lightDirection));
   result.viewDir = normalize(mul(TNB, vEyePos - worldPos));

   return( result );
}

float4 ps_main(vs_output input) : COLOR
{
	float4 diffuseColor = tex2D(DiffuseSampler, input.texcoord);

	float3 normal = (2.0f * tex2D(NormalSampler, input.texcoord)) - 1.0f;

	//Diffuse
	float d = saturate(dot(normal, input.lightDir));
	//Reflection
	float3 r = normalize(2 * d * normal - input.lightDir);

	float s = pow(saturate(dot(r, input.viewDir)), 8);

	float4 ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);

	return diffuseColor;
}

technique Base
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();

	  AlphaRef = 200;
	  AlphaFunc = GreaterEqual;
	  AlphaTestEnable = true;

	  CullMode = None;
   }
}

