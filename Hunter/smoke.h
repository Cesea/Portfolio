#pragma once
#include "PSystem.h"
class Smoke :
	public PSystem
{
	void initParticle(Particle& out)override
	{

		out.initialTime = mTime;

		out.lifeTime = RandFloat(3.0f, 4.0f);

		out.initialSize = RandFloat(40.0f, 50.0f);


		Vector3 rndVec = Vector3(RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f));
		out.initialVelocity = rndVec;

		out.mass = 0;

		out.initialColor = WHITENESS;

		out.initialPos.x = RandFloat(AABBMin.x, AABBMax.x);
		out.initialPos.y = RandFloat(AABBMin.y, AABBMax.y);
		out.initialPos.z = RandFloat(AABBMin.z, AABBMax.z);
	}
public:
	Smoke(const std::string& fxName,
		const std::string& techName,
		const std::string& texName,
		const Vector3 & accel,
		int maxNumParticles,
		float timePerParticle,
		Vector3 pos, Vector3 Min, Vector3 Max);
	~Smoke();
};

