
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

OutputVS FireBarrierVS(float3 posL  : POSITION0,
	float3 vel : TEXCOORD0,
	float size : TEXCOORD1,
	float time : TEXCOORD2,
	float lifeTime : TEXCOORD3,
	float mass : TEXCOORD4,
	float4 color : COLOR0)
{
	OutputVS outVS = (OutputVS)0;


	float t = gTime - time;

	float a1 = vel.y*t;
	if (a1 < 0) a1 *= -1;

	float velSize = sqrt(vel.x*vel.x + vel.y*vel.y + vel.z*vel.z);

	float rd = sqrt(velSize*velSize - a1*a1);


	posL.y += a1;

	float sine, cosine;
	sincos(10.0f*t*velSize, sine, cosine);

	posL.x = posL.x + sine*t;
	posL.z = posL.z + cosine*t;
	//posL = posL + vel*t;

	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	size +=t*t*8;

	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	outVS.color = (1.0f - (t / lifeTime));
	return outVS;
}

float4 FireBarrierPS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	return color*tex2D(TexS, tex0);
}

technique FireBarrierTech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;
		
		SrcBlend = One;
		DestBlend = One;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 FireBarrierVS();
		pixelShader = compile ps_2_0 FireBarrierPS();
	}
}