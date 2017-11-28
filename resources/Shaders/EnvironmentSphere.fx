
uniform extern float4x4 matWVP;
uniform extern texture  EnvironmentTexture;

sampler EnvMapS = sampler_state
{
    Texture   = <EnvironmentTexture>;
    MinFilter = LINEAR; 
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};


void SkyVS(float3 posL : POSITION0, 
			float3 normal : NORMAL0,
           out float4 oPosH : POSITION0, 
           out float3 oEnvTex : TEXCOORD0)
{
	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    oPosH = mul(float4(posL, 1.0f), matWVP).xyzw; 
    
    // Use skymesh vertex position, in local space, as index into cubemap. 
    oEnvTex = normalize(normal);
}

float4 SkyPS(float3 envTex : TEXCOORD0) : COLOR
{
    return texCUBE(EnvMapS, envTex);
}

technique SkyTech
{
    pass P0
    {
        vertexShader = compile vs_3_0 SkyVS();
        pixelShader  = compile ps_3_0 SkyPS();
		CullMode = None;
		ZFunc = Always; // Always write sky to depth buffer
		StencilEnable = true;
		StencilFunc   = Always;
		StencilPass   = Replace;
		StencilRef    = 0; // clear to zero
    }
}

