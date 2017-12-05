#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H


#include "TransformComponent.h"
#include "ParticleComponent.h"
class Camera;
class ParticleSystem :
	public System<Requires<TransformComponent, ParticleComponent>>
{
	Camera * _cam;
	Vector3 _camPos;
public:
	void setCamera(Camera* camera,Vector3 camPos);
	void update(float deltaTime);
	void render();
public:
	ParticleSystem();
	virtual ~ParticleSystem();
};
#endif

