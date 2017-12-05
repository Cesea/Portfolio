#include "stdafx.h"
#include "ParticleSystem.h"
#include "Camera.h"
ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}
void ParticleSystem::update(float deltaTime)
{
	auto &entities = GetEntities();
	for (int i = 0; i < entities.size(); i++)
	{
		ParticleComponent & particle = entities[i].GetComponent<ParticleComponent>();
		TransformComponent & transComp = entities[i].GetComponent<TransformComponent>();
		particle._particle->setCamera(_cam, _camPos);
		particle._particle->update(deltaTime);
	}
}

void ParticleSystem::render()
{
	auto &entities = GetEntities();
	for (int i = 0; i < entities.size(); i++)
	{
		ParticleComponent & particle = entities[i].GetComponent<ParticleComponent>();
		TransformComponent & transComp = entities[i].GetComponent<TransformComponent>();
		particle._particle->draw();
	}
}

void ParticleSystem::setCamera(Camera* camera, Vector3 camPos)
{
	_cam = camera;
	_camPos = camPos;
}
