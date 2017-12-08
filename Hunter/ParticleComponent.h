#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H
#include "Component.h"
#include "Particle.h"
#include "PSystem.h"
#include "ParticleString.h"
class ParticleComponent :
	public Component
{
public:
	enum PARTICLE_TYPE
	{
		PARTICLE_TYPE_FIRERING,
		PARTICLE_TYPE_FIREBALL,
		PARTICLE_TYPE_FIREBREATH,
		PARTICLE_TYPE_CHARGE,
		PARTICLE_TYPE_SMOKE,
		PARTICLE_TYPE_BLOOD_FOG,
		PARTICLE_TYPE_BLOOD_PARTICLE,
		PARTICLE_TYPE_NORMALBREATH,
		PARTICLE_TYPE_NORMALFIREBALL,
		PARTICLE_TYPE_NORMALFIREBALL2
	};
	PARTICLE_TYPE _type;

	void init(PARTICLE_TYPE type, int maxParticleNum, float emission, Vector3 accel, Vector3 pos);
	PSystem* _particle;

	Vector3 min;
	Vector3 max;
	float duration{ 0.0f };
	float delay{ 0.0f };
	bool isEmission{ true };
	
	//움직이는 파티클들
	bool canMove{ false };
	Vector3 velocity;

	ParticleComponent();
	virtual ~ParticleComponent();
};

#endif