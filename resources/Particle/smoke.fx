
uniform extern float4x4 gWVP;

uniform extern texture  gTex;
// The position of the camera in the particle system's
// local space for distance calculations.
uniform extern float3   gEyePosL;
// Constant acceleration vector.
uniform extern float3   gAccel;
// Particle system time -- corresponds to PSystem::mTime.
uniform extern float    gTime;
// Viewport height for scaling the point sprite sizes; see comment
// in PSystem::draw.
uniform extern int      gViewportHeight;


sampler TexS = sampler_state
{

	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct OutputVS
{
	float4 posH  : POSITION0;
	float4 color : COLOR0;
	float2 tex0  : TEXCOORD0;    // D3D fills in for point sprites.
	float size : PSIZE;        // Size in pixels.
};

OutputVS SmokeVS(float3 posL  : POSITION0,
	float3 vel : TEXCOORD0,
	float size : TEXCOORD1,
	float time : TEXCOORD2,
	float lifeTime : TEXCOORD3,
	float mass : TEXCOORD4,
	float4 color : COLOR0)
{
	// Zero out our output.
	OutputVS outVS = (OutputVS)0;

	float t = gTime - time;

	posL = posL + vel*t+ gAccel/2*t*t;

	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	// Fade color from white to black over the particle's lifetime
	// to fade it out gradually.

	float p = t / lifeTime;

	if (p >0.5f)
	{
		outVS.color = (1.0f-p)/2;
	}
	else
	{
		outVS.color = p/2;
	}


	return outVS;
}

float4 SmokePS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color.
	return color*tex2D(TexS, tex0);
}

technique SmokeTech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;
		
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 SmokeVS();
		pixelShader = compile ps_2_0 SmokePS();
	}
}