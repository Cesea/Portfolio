#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H
#include "Component.h"
class ParticleComponent :
	public Component
{
public:
	enum PARTICLE_EMISSION_TYPE
	{
		PZERO,
		SPHERE,
		SPHERE_OUTLINE,
		BOX
	};

	ParticleComponent();
	virtual ~ParticleComponent();
};

#endif