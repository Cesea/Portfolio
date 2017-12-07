#include "stdafx.h"
#include "ParticleComponent.h"

ParticleComponent::ParticleComponent()
{
}


ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::init(PARTICLE_TYPE type, int maxParticleNum, float emission, Vector3 accel, Vector3 pos)
{
	switch (type)
	{
	case PARTICLE_TYPE_FIRERING:
		_particle = new fireRing("../resources/Particle/fireRing.fx", "FireRingTech", "../resources/Particle/fire.png",
			accel, maxParticleNum, emission, pos);
		break;
	case PARTICLE_TYPE_FIREBALL:
		_particle = new fireBall("../resources/Particle/fireRing.fx", "FireBallTech", "../resources/Particle/ht.png",
			accel, maxParticleNum, emission, pos,0.2f);
		break;
	case PARTICLE_TYPE_FIREBREATH:
		_particle = new fireBreath("../resources/Particle/fireRing.fx", "FireBreathTech", "../resources/Particle/fire.PNG",
			accel, maxParticleNum, emission, pos);
		break;
	case PARTICLE_TYPE_CHARGE:
		_particle = new charge("../resources/Particle/charge.fx", "CenterChargeTech", "../resources/Particle/abcd5.png",
			accel, maxParticleNum, emission, pos, 10.0f);
		break;
	case PARTICLE_TYPE_SMOKE:
		_particle = new Smoke("../resources/Particle/Smoke.fx", "SmokeTech", "../resources/Particle/2.png",
			accel, maxParticleNum, emission, pos, min, max);
		break;
	case PARTICLE_TYPE_BLOOD_FOG:
		_particle = new bloodFog("../resources/Particle/blood.fx", "BloodFogTech", "../1.jpg",
			accel, maxParticleNum, emission, pos, min, max);
		break;
	case PARTICLE_TYPE_BLOOD_PARTICLE:
		_particle = new BloodParticle("../resources/Particle/blood.fx", "BloodParticleTech", "../bloodTex.png",
			accel, maxParticleNum, emission, pos, min, max);
		break;
	}
}