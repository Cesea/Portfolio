
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

OutputVS BloodVS(float3 posL  : POSITION0,
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

	posL = posL + vel*t / 2 + gAccel / 2 *t;

	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	// Fade color from white to black over the particle's lifetime
	// to fade it out gradually.

	float p = t / lifeTime;

	outVS.color = (0.5f - p / 2);


	return outVS;
}

OutputVS Blood2VS(float3 posL  : POSITION0,
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

	posL = posL + vel*t + gAccel * t*t;

	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	// Fade color from white to black over the particle's lifetime
	// to fade it out gradually.

	float p = t / lifeTime;

	outVS.color = 1.0f-p;



	return outVS;
}

float4 BloodPS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color.
	return color*tex2D(TexS, tex0)/3;
}
float4 Blood2PS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color.
	return color*tex2D(TexS, tex0);
}

technique BloodFogTech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;


		SrcBlend = SrcAlpha;
		DestBlend = One;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 BloodVS();
		pixelShader = compile ps_2_0 BloodPS();
	}
}

technique BloodParticleTech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;


		SrcBlend = One;
		DestBlend = One;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 Blood2VS();
		pixelShader = compile ps_2_0 Blood2PS();
	}
}