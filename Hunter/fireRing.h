#pragma once
#include "PSystem.h"
class fireRing :
	public PSystem
{
	void initParticle(Particle& out)override
	{
		// Time particle is created relative to the global running
		// time of the particle system.
		out.initialTime = mTime;

		// Flare lives for 2-4 seconds.
		out.lifeTime = RandFloat(2.0f, 4.0f);

		// Initial size in pixels.
		out.initialSize = RandFloat(10.0f, 15.0f);

		// Give a very small initial velocity to give the flares
		// some randomness.
		Vector3 rndVec = Vector3(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f));
		out.initialVelocity = rndVec;
		// Scalar value used in vertex shader as an
		// amplitude factor.
		out.mass = RandFloat(1.0f, 2.0f);

		// Start color at 50-100% intensity when born for
		// variation.
		out.initialColor = RandFloat(0.5f, 1.0f) * WHITENESS;

		// Generate random particle on the ring in polar
		// coordinates: random radius and random angle.
		float r = RandFloat(10.0f, 14.0f);
		float t = RandFloat(0, 2.0f*D3DX_PI);

		// Convert to Cartesian coordinates.
		out.initialPos.x = Pos.x + r*cosf(t);
		out.initialPos.y = Pos.y + r*sinf(t);

		// Random depth value in [-1, 1] (depth of the ring)
		out.initialPos.z = Pos.z + RandFloat(-1.0f, 1.0f);
	}

public:
	fireRing(const std::string& fxName,
		const std::string& techName,
		const std::string& texName,
		const Vector3& accel,
		int maxNumParticles,
		float timePerParticle,
		Vector3 pos);
	virtual ~fireRing();
};

