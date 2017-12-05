#pragma once
#include "PSystem.h"
class charge :
	public PSystem
{
	void initParticle(Particle& out)override
	{

		out.initialTime = mTime;

		out.lifeTime = RandFloat(1.0f, 3.0f);

		out.initialSize = RandFloat(2.0f, 4.0f);


		Vector3 rndVec = Vector3(RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f));
		D3DXVec3Normalize(&rndVec, &rndVec);
		rndVec *= _radius;

		out.initialPos = Pos + rndVec;

		out.initialVelocity = -rndVec;

		out.mass = 0;

		out.initialColor = WHITENESS;
	}
public:
	charge(const std::string& fxName,
		const std::string& techName,
		const std::string& texName,
		const Vector3& accel,
		int maxNumParticles,
		float timePerParticle,
		Vector3 pos, float radius);
	~charge();
};
