#pragma once
#include "PSystem.h"
class BloodParticle :
	public PSystem
{
	void initParticle(Particle& out)override
	{

		out.initialTime = mTime;

		out.lifeTime = 1.0f;

		out.initialSize = 1.0f;


		Vector3 rndVec = Vector3(RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f));
		D3DXVec3Normalize(&rndVec, &rndVec);

		out.initialPos = Pos;

		out.initialVelocity = -rndVec * 2;

		out.mass = 0;


		out.initialColor = WHITENESS;
	}
public:
	BloodParticle(const std::string& fxName,
		const std::string& techName,
		const std::string& texName,
		const Vector3& accel,
		int maxNumParticles,
		float timePerParticle,
		Vector3 pos, Vector3 Min, Vector3 Max);
	~BloodParticle();
};
