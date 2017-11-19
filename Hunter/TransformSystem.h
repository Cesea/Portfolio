#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "TransformComponent.h"

class TransformSystem : public System<Requires<TransformComponent>>
{
public:
	TransformSystem();
	virtual ~TransformSystem() override;

	//내부의 MatFinal을 모두 업데이트 한다
	void PreUpdate(float deltaTime);

	void Update(float deltaTime);

	//만약에 이동이 환정이 된다면 더미의 위치를 현재의 위치에 적용시킨다
	void PostUpdate(float deltaTime);

	virtual void Initialize();

	virtual void OnEntityAdded(Entity& entity);

	virtual void OnEntityRemoved(Entity& entity);

	void UpdateTransform(TransformComponent &component);
private :
};


#endif