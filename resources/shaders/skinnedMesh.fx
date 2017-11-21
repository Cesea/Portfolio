//------------------------------------------------------------------------------------
// SkinnedMesh Vertex Processing
//------------------------------------------------------------------------------------

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


extern float4x4 FinalTransforms[40]; 
extern int NumVertInfluences = 2; // <--- Normally set dynamically.

float4x4 matWorld : World;
float4x4 staticWorld;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos;

struct vs_input
{
    float3  position        : POSITION;
    float3  blendWeights    : BLENDWEIGHT;
    float4  blendIndices    : BLENDINDICES;
    float3  normal          : NORMAL;
    //float3  Tangent         : TANGENT;
    //float3  Binormal        : Binormal;
    float2  texcoord        : TEXCOORD0;
};

struct vs_out 
{
    float4  position		: POSITION;
    float2  texcoord        : TEXCOORD0;
	float3  normal          : TEXCOORD1;
	float3  binormal        : TEXCOORD2;
	float3  tangent         : TEXCOORD3;
	float3  viewDir			: TEXCOORD4;
	float3  worldPos		: TEXCOORD5;
	float4 finalPos : TEXCOORD6;
};

//Skinning Vertex Shader//////////////////
vs_out vs_skinning(vs_input input)
{
    vs_out result;

    float4 p = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 norm = float3(0.0f, 0.0f, 0.0f);
    float lastWeight = 0.0f;
    int n = NumVertInfluences - 1;

	input.normal = normalize(input.normal);

    for(int i = 0; i < n; ++i)
    {
        lastWeight += input.blendWeights[i];
	    p += input.blendWeights[i] * mul(float4(input.position, 1.0f), FinalTransforms[input.blendIndices[i]]);
	    norm += input.blendWeights[i] * (float3)mul(float4(input.normal, 0.0f), FinalTransforms[input.blendIndices[i]]);
    }
    lastWeight = 1.0f - lastWeight;

	p += lastWeight *  mul(float4(input.position, 1.0f), FinalTransforms[input.blendIndices[n]]);
	norm += lastWeight * (float3)mul(float4(input.normal, 0.0f), FinalTransforms[input.blendIndices[n]]);

    p.w = 1.0f;    	
	float4 posWorld = mul(p, matWorld);
    result.position = mul(posWorld, matViewProjection);
    result.texcoord = input.texcoord;

	result.normal = float3(0.0f, 0.0f, 0.0f);
	result.binormal = float3(0.0f, 0.0f, 0.0f);
	result.tangent = float3(0.0f, 0.0f, 0.0f);
	result.viewDir = float3(0.0f, 0.0f, 0.0f);
	result.worldPos = float3(0.0f, 0.0f, 0.0f);
	result.finalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return result;
}

//Normal Vertex Shader //////////////////
vs_out vs_normal(vs_input input)
{
	vs_out result = (vs_out)0;

    float4 p = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 norm = float3(0.0f, 0.0f, 0.0f);

	input.normal = normalize(input.normal);

	p = mul(float4(input.position, 1.0f), staticWorld);
	result.position = mul(p, matViewProjection);
	result.texcoord = input.texcoord;

	result.normal = float3(0.0f, 0.0f, 0.0f);
	result.binormal = float3(0.0f, 0.0f, 0.0f);
	result.tangent = float3(0.0f, 0.0f, 0.0f);
	result.viewDir = float3(0.0f, 0.0f, 0.0f);
	result.worldPos = float3(0.0f, 0.0f, 0.0f);
	result.finalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return result;
}


float4 ps_main(vs_out input) : COLOR
{
	return tex2D(Diffuse, input.texcoord);
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique Skinning
{
    pass p0
    {
        VertexShader = compile vs_3_0 vs_skinning();
		PixelShader = compile ps_3_0 ps_main();

		CullMode = None;
    }
}

technique Basic
{
    pass p0
    {
        VertexShader = compile vs_3_0 vs_normal();
		PixelShader = compile ps_3_0 ps_main();
    }
}

