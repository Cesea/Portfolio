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

		//������ �ð��� ��´�
		if (particle.delay > 0.0f)
		{	
			particle.delay -= deltaTime;
			continue;
		}

		//���ӽð��� ��� 0���ϰ� �Ǹ� ��������
		particle.duration -= deltaTime;
		if (particle.duration <= 0)
		{
			particle._particle->makeParticle = false;
		}

		//�����̴� ��ƼŬ�̸� �̵��Ѵ�
		if (particle.canMove)
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + particle.velocity/20);
			particle._particle->setPos(transComp.GetWorldPosition());
		}

		//�����������Ŀ� �ð��� �帣�� ��ƼƼ ����
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
