#pragma once
#include "PSystem.h"
class fireBreath :
	public PSystem
{
	void initParticle(Particle& out)override
	{
		// Time particle is created relative to the global running
		// time of the particle system.
		out.initialTime = mTime;

		// Flare lives for 2-4 seconds.
		out.lifeTime = RandFloat(4.0f, 6.0f);

		// Initial size in pixels.
		out.initialSize = RandFloat(40.0f, 60.0f);

		// Give a very small initial velocity to give the flares
		// some randomness.
		Vector3 rndVec = Vector3(RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f), RandFloat(0.0f, 1.0f));
		rndVec = -direction * 5 + rndVec;
		rndVec.y /= 2;
		out.initialVelocity = rndVec;
		// Scalar value used in vertex shader as an
		// amplitude factor.
		out.mass = RandFloat(1.0f, 2.0f);

		// Start color at 50-100% intensity when born for
		// variation.
		out.initialColor = RandFloat(0.5f, 1.0f)*WHITENESS;

		out.initialPos = Pos;
	}
private:
	Vector3 direction;
public:
	fireBreath(const std::string& fxName,
		const std::string& techName,
		const std::string& texName,
		const Vector3& accel,
		int maxNumParticles,
		float timePerParticle, Vector3 pos);
	~fireBreath();
};