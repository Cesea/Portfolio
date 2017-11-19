#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "TransformComponent.h"

class TransformSystem : public System<Requires<TransformComponent>>
{
public:
	TransformSystem();
	virtual ~TransformSystem() override;

	//������ MatFinal�� ��� ������Ʈ �Ѵ�
	void PreUpdate(float deltaTime);

	void Update(float deltaTime);

	//���࿡ �̵��� ȯ���� �ȴٸ� ������ ��ġ�� ������ ��ġ�� �����Ų��
	void PostUpdate(float deltaTime);

	virtual void Initialize();

	virtual void OnEntityAdded(Entity& entity);

	virtual void OnEntityRemoved(Entity& entity);

	void UpdateTransform(TransformComponent &component);
private :
};


#endif