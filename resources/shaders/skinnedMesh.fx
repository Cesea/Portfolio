//------------------------------------------------------------------------------------
// SkinnedMesh Vertex Processing
//------------------------------------------------------------------------------------

#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 35
#endif



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


extern float4x4 FinalTransforms[45]; 
extern int NumVertInfluences = 2; // <--- Normally set dynamically.

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos;

struct VS_INPUT
{
    float3  Position        : POSITION;
    float3  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3  Tangent         : TANGENT;
    float3  Binormal        : Binormal;
    float2  Texcoord        : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  Position		: POSITION;
    float2  Texcoord        : TEXCOORD0;
	float3  Normal          : TEXCOORD1;
	float3  Binormal        : TEXCOORD2;
	float3  Tangent         : TEXCOORD3;
	float3  viewDir			: TEXCOORD4;
	float3  worldPos		: TEXCOORD5;
	float4 FinalPos : TEXCOORD6;
};

//버택스 진입
VS_OUTPUT VertSkinning(VS_INPUT Input)
{
    VS_OUTPUT Output;

    float4 p = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 norm = float3(0.0f, 0.0f, 0.0f);
    float lastWeight = 0.0f;
    int n = NumVertInfluences - 1;

	Input.Normal = normalize(Input.Normal);

    for(int i = 0; i < n; ++i)
    {
        lastWeight += Input.BlendWeights[i];
	    p += Input.BlendWeights[i] * mul(float4(Input.Position, 1.0f), FinalTransforms[Input.BlendIndices[i]]);
	    norm += Input.BlendWeights[i] * (float3)mul(float4(Input.Normal, 0.0f), FinalTransforms[Input.BlendIndices[i]]);
    }
    lastWeight = 1.0f - lastWeight;

	p += lastWeight *  mul(float4(Input.Position, 1.0f), FinalTransforms[Input.BlendIndices[n]]);
	norm += lastWeight * (float3)mul(float4(Input.Normal, 0.0f), FinalTransforms[Input.BlendIndices[n]]);

    p.w = 1.0f;    	
	float4 posWorld = mul(p, matWorld);
    Output.Position = mul(posWorld, matViewProjection);
    Output.Texcoord = Input.Texcoord;

	Output.Normal = float3(0.0f, 0.0f, 0.0f);
	Output.Binormal = float3(0.0f, 0.0f, 0.0f);
	Output.Tangent = float3(0.0f, 0.0f, 0.0f);
	Output.viewDir = float3(0.0f, 0.0f, 0.0f);
	Output.worldPos = float3(0.0f, 0.0f, 0.0f);
	Output.FinalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return Output;
}

float4 ps_main(VS_OUTPUT input) : COLOR
{
	return tex2D(Diffuse, input.Texcoord);
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique Base
{
    pass p0
    {
        VertexShader = compile vs_3_0 VertSkinning();
		PixelShader = compile ps_3_0 ps_main();
    }
}

