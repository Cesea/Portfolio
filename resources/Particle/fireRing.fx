
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

// Input corresponds to the Particle structure data members.
OutputVS FireRingVS(float3 posL  : POSITION0,
	float3 vel : TEXCOORD0,
	float size : TEXCOORD1,
	float time : TEXCOORD2,
	float lifeTime : TEXCOORD3,
	float mass : TEXCOORD4,
	float4 color : COLOR0)
{
	// Zero out our output.
	OutputVS outVS = (OutputVS)0;

	// Get age of particle from creation time.
	float t = gTime - time;

	// Rotate the particles about local space about z-axis as a
	// function of time. These are just the rotation equations.
	float sine, cosine;
	sincos(0.5f*mass*t, sine, cosine);
	float x = posL.x*cosine+ posL.y*-sine;
	float y = posL.x*sine+ posL.y*cosine;

	// Oscillate particles up and down.
	float s = sin(6.0f*t);
	posL.x = x;
	posL.y = y + mass*s;

	// Constant acceleration (Equation 19.1).
	posL = posL + vel*t + 0.5f * gAccel * t * t;

	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	// Ramp up size over time to simulate the flare expanding
	// over time. Formula found by experimenting.
	size += 8.0f*t*t;

	// Also compute size as a function of the distance from
	// the camera, and the viewport height. The constants
	// were found by experimenting.
	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	// Fade color from white to black over the particle's lifetime
	// to fade it out gradually.
	outVS.color = (1.0f - (t / lifeTime));

	// Done--return the output.
	return outVS;
}

float4 FireRingPS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color.
	return color*tex2D(TexS, tex0);
}

OutputVS FireBreathVS(float3 posL  : POSITION0,
	float3 vel : TEXCOORD0,
	float size : TEXCOORD1,
	float time : TEXCOORD2,
	float lifeTime : TEXCOORD3,
	float mass : TEXCOORD4,
	float4 color : COLOR0)
{
	// Zero out our output.
	OutputVS outVS = (OutputVS)0;

	// Get age of particle from creation time.
	float t = gTime - time;

	// Constant acceleration (Equation 19.1).
	posL = posL + vel*t*5 + 0.5f * gAccel * t * t;

	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	// Ramp up size over time to simulate the flare expanding
	// over time. Formula found by experimenting.
	size += 16.0f*t*t;

	// Also compute size as a function of the distance from
	// the camera, and the viewport height. The constants
	// were found by experimenting.
	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	// Fade color from white to black over the particle's lifetime
	// to fade it out gradually.
	outVS.color = (1.0f - (t / lifeTime));

	// Done--return the output.
	return outVS;
}

float4 FireBreathPS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color.
	return color*tex2D(TexS, tex0);
}


OutputVS FireBallVS(float3 posL  : POSITION0,
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

	float a1 = vel.y*t;
	//if (vel.y < 0) a1 *= -1;

	float velSize = sqrt(vel.x*vel.x + vel.y*vel.y + vel.z*vel.z)*t;

	float rd = sqrt(velSize*velSize - a1*a1);

	
	float sine, cosine;
	sincos(500.0f*t*vel.x*vel.z, sine, cosine);

	posL.x = posL.x+ sine*rd;
	posL.z = posL.z + cosine*rd;
	posL.y += a1;
	//posL = posL + vel*t;

	
	outVS.posH = mul(float4(posL, 1.0f), gWVP);


	size += 2.0f*t*t;

	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	outVS.color = (1.0f - (t / lifeTime));

	return outVS;
}

float4 FireBallPS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color.
	return color*tex2D(TexS, tex0);
}

OutputVS NormalBreathVS(float3 posL  : POSITION0,
	float3 vel : TEXCOORD0,
	float size : TEXCOORD1,
	float time : TEXCOORD2,
	float lifeTime : TEXCOORD3,
	float mass : TEXCOORD4,
	float4 color : COLOR0)
{
	// Zero out our output.
	OutputVS outVS = (OutputVS)0;

	// Get age of particle from creation time.
	float t = gTime - time;

	// Constant acceleration (Equation 19.1).
	posL = posL + vel*t * 5 + 0.5f * gAccel * t * t;

	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	// Ramp up size over time to simulate the flare expanding
	// over time. Formula found by experimenting.
	size += 2.0f*t*t;

	// Also compute size as a function of the distance from
	// the camera, and the viewport height. The constants
	// were found by experimenting.
	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	// Fade color from white to black over the particle's lifetime
	// to fade it out gradually.
	outVS.color = (1.0f - (t / lifeTime));

	// Done--return the output.
	return outVS;
}

float4 NormalBreathPS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color.
	return color*tex2D(TexS, tex0);
}

OutputVS FireBall2VS(float3 posL  : POSITION0,
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

	float a1 = vel.y*t;
	//if (vel.y < 0) a1 *= -1;

	float velSize = sqrt(vel.x*vel.x + vel.y*vel.y + vel.z*vel.z)*t;

	float rd = sqrt(velSize*velSize - a1*a1);


	float sine, cosine;
	sincos(500.0f*t*vel.x*vel.z, sine, cosine);

	posL.x = posL.x + sine*rd;
	posL.z = posL.z + cosine*rd;
	posL.y += a1;
	//posL = posL + vel*t;
	//posL = posL  +  gAccel * t * t/10;

	outVS.posH = mul(float4(posL, 1.0f), gWVP);


	size = t*t*2;

	float d = distance(posL, gEyePosL);
	outVS.size = gViewportHeight*size / (1.0f + 8.0f*d);

	outVS.color = (1.0f - (t / lifeTime));

	return outVS;
}

float4 FireBall2PS(float4 color : COLOR0,
	float2 tex0 : TEXCOORD0) : COLOR
{
	// Multiply the faded color with the texture color.
	return color*tex2D(TexS, tex0);
}


technique FireRingTech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;
		
		SrcBlend = One;
		DestBlend = One;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 FireRingVS();
		pixelShader = compile ps_2_0 FireRingPS();
	}
}

technique FireBreathTech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;

		SrcBlend = One;
		DestBlend = One;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 FireBreathVS();
		pixelShader = compile ps_2_0 FireBreathPS();
	}
}

technique FireBallTech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;

		SrcBlend = SrcAlpha;
		DestBlend = One;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 FireBallVS();
		pixelShader = compile ps_2_0 FireBallPS();
	}
}

technique NormalBreathTech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;

		SrcBlend = One;
		DestBlend = One;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 NormalBreathVS();
		pixelShader = compile ps_2_0 NormalBreathPS();
	}
}

technique FireBall2Tech
{
	pass P0
	{
		PointSpriteEnable = true;
		AlphaBlendEnable = true;

		SrcBlend = One;
		DestBlend = One;

		ZWriteEnable = false;
		vertexShader = compile vs_2_0 FireBall2VS();
		pixelShader = compile ps_2_0 FireBall2PS();
	}
}



