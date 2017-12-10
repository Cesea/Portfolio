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
	if (!_running)
	{
		return;
	}

	auto &entities = GetEntities();
	for (int i = 0; i < entities.size(); i++)
	{
		ParticleComponent & particle = entities[i].GetComponent<ParticleComponent>();
		TransformComponent & transComp = entities[i].GetComponent<TransformComponent>();

		//딜레이 시간을 깎는다
		if (particle.delay > 0.0f)
		{	
			particle.delay -= deltaTime;
			continue;
		}

		//지속시간을 깎고 0이하가 되면 생성중지
		particle.duration -= deltaTime;
		if (particle.duration <= 0)
		{
			particle._particle->makeParticle = false;
		}

		//움직이는 파티클이면 이동한다
		if (particle.canMove)
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + particle.velocity/20);
			particle._particle->setPos(transComp.GetWorldPosition());
		}

		//생성중지된후에 시간이 흐르면 엔티티 삭제
		if (particle.duration <= -4)
		{
			entities[i].Kill();
		}
		else
		{
			particle._particle->setCamera(_cam, _camPos);
			particle._particle->update(deltaTime);
		}
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
