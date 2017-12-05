#pragma once
#include "PSystem.h"
class fireBall :
	public PSystem
{
	void initParticle(Particle& out)override
	{
		// Time particle is created relative to the global running
		// time of the particle system.
		out.initialTime = mTime;

		float _lifeTime = RandFloat(1.0f, 4.0f);
		out.lifeTime = _lifeTime;

		// Initial size in pixels.
		out.initialSize = RandFloat(8.0f, 10.0f);

		// Give a very small initial velocity to give the flares
		// some randomness.
		Vector3 rndVec = Vector3(RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f));
		D3DXVec3Normalize(&rndVec, &rndVec);
		rndVec = rndVec*_radius;


		out.initialPos = Pos;

		out.initialVelocity = rndVec;
		// Scalar value used in vertex shader as an
		// amplitude factor.
		out.mass = RandFloat(1.0f, 2.0f);

		// Start color at 50-100% intensity when born for
		// variation.
		out.initialColor = WHITENESS;

		float r = 10.0f;
	}
private:
	Vector3 direction;
public:
	fireBall(const std::string& fxName,
		const std::string& techName,
		const std::string& texName,
		const Vector3& accel,
		int maxNumParticles,
		float timePerParticle, Vector3 pos, float radius);
	~fireBall();

};

