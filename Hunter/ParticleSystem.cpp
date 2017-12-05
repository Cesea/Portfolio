#include "stdafx.h"
#include "ParticleSystem.h"

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
		particle.update(deltaTime, transComp.GetWorldPosition());
	}
}

void ParticleSystem::render()
{
	auto &entities = GetEntities();
	for (int i = 0; i < entities.size(); i++)
	{
		ParticleComponent & particle = entities[i].GetComponent<ParticleComponent>();
		TransformComponent & transComp = entities[i].GetComponent<TransformComponent>();


		particle.render(transComp.GetWorldPosition());

	}
}
