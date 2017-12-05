#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H


#include "TransformComponent.h"
#include "ParticleComponent.h"

class ParticleSystem :
	public System<Requires<TransformComponent, ParticleComponent>>
{
public:
	void update(float deltaTime);
	void render();
public:
	ParticleSystem();
	virtual ~ParticleSystem();
};
#endif

