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
		PARTICLE_TYPE_SMOKE
	};
	PARTICLE_TYPE _type;

	void init(PARTICLE_TYPE type, int maxParticleNum, float emission, Vector3 accel, Vector3 pos);
	PSystem* _particle;

	Vector3 min;
	Vector3 max;

	ParticleComponent();
	virtual ~ParticleComponent();
};

#endif