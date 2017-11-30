#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{

}

//for (uint32 i = 0; i < entities.size(); ++i)
//{
//	TransformComponent &transform = entities[i].GetComponent<TransformComponent>();
//	CollisionComponent &collision = entities[i].GetComponent<CollisionComponent>();
//
//	TerrainGridPosition gridPosition = ConvertWorldPositionToGridPosition( transform._position);
//
//	TerrainGrid &terrainGrid = TERRAIN->GetGrid(gridPosition);
//	for (int32 j = 0; j < terrainGrid._baseObject.size(); ++j)
//	{
//		TransformComponent &otherTransform = terrainGrid._objects[j]._entities;
//		CollisionComponent &otherCollision = terrainGrid._objects[j]._entities;

//		Vector3 diff = transform.GetWorldPosition() - otherTransform.GetWorldPosition();
//		float distance = Vec3Length(&diff);
//		if (distance < checkRange)
//		{

//		}
//	}
//	terrainGrid.
//}


void CollisionSystem::Update(float deltaTime, float checkRange)
{
	auto &entities = GetEntities();

	AABB aabb0;
	AABB aabb1;

	for (uint32 i = 0; i < entities.size(); ++i)
	{
		TransformComponent & transform = entities[i].GetComponent<TransformComponent>();
		CollisionComponent & collision = entities[i].GetComponent<CollisionComponent>();

		aabb0._min = collision._boundingBox._localMinPos;
		aabb0._max = collision._boundingBox._localMaxPos;

		TransformAABB(&aabb0, transform);

		for (uint32 j = 0; j < entities.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			TransformComponent& transform2 = entities[j].GetComponent<TransformComponent>();
			CollisionComponent & collision2 = entities[j].GetComponent<CollisionComponent>();

			aabb1._min = collision2._boundingBox._localMinPos;
			aabb1._max = collision2._boundingBox._localMaxPos;

			TransformAABB(&aabb1, transform2);

			Vector3 distanceVec = transform.GetWorldPosition() - transform2.GetWorldPosition();
			float distance = Vec3Length(&distanceVec);
			if (distance < checkRange)
			{
				//둘다 고정됬으면 검사하지않음
				if (collision2._locked && collision._locked) continue;
				//둘중 하나가 고정되어있음
				//if ((collision._locked && !collision2._locked) || (!collision._locked && collision2._locked))
				else
				{
					//충돌했다면
					if (Collision_AABBToAABB(aabb0._min,
						aabb0._max,
						aabb1._min, 
						aabb1._max))
					{
						//움직일수 없음의 이벤트
						//Console::Log("Collidededed!!!!\n");
						//트리거 검사
						if (collision._isTrigger)
						{
							switch (collision._triggerType)
							{
							case CollisionComponent::TRIGGER_TYPE_ENEMY:
							case CollisionComponent::TRIGGER_TYPE_PLAYER:
								_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
								break;
							case CollisionComponent::TRIGGER_TYPE_OBJECT:
								_channel.Broadcast<ObjectTriggerEvent>(ObjectTriggerEvent(entities[i], entities[j]));
								break;
							}

						}
						if (collision2._isTrigger)
						{
							switch (collision._triggerType)
							{
							case CollisionComponent::TRIGGER_TYPE_ENEMY:
							case CollisionComponent::TRIGGER_TYPE_PLAYER:
								_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[j], entities[i]));
								break;
							case CollisionComponent::TRIGGER_TYPE_OBJECT:
								_channel.Broadcast<ObjectTriggerEvent>(ObjectTriggerEvent(entities[j], entities[i]));
								break;
							}
						}

					}
				}
				//둘다 고정되어 있지 않음
				//else if (!collision._locked && !collision2._locked)
				//{
				//	IsBlocking(&transform, &collision._boundingBox, &transform2, &collision2._boundingBox, 1.0f);
				//	//트리거 검사
				//	if (collision._isTrigger)
				//	{

				//	}
				//	if (collision2._isTrigger)
				//	{

				//	}
				//}

			}
		}
	}
}

void CollisionSystem::QueryRayEntityHit(const Ray &ray, std::vector<Entity>* pOutCollidingEntity, 
	std::vector<float> *pOutDistance)
{
	auto &entities = GetEntities();

	Vector3 hitPos;
	for (uint32 i = 0; i < entities.size(); ++i)
	{
		CollisionComponent &refCollision = entities[i].GetComponent<CollisionComponent>();
		TransformComponent &transform = entities[i].GetComponent<TransformComponent>();

		if (IsRayHitBound(ray, refCollision._boundingSphere, transform, &hitPos, nullptr))
		{
			pOutCollidingEntity->push_back(entities[i]);

			float distance = Vec3LengthSq(&(hitPos - ray.origin));
			pOutDistance->push_back(distance);
		}
	}

	int a = 0;
}

void CollisionSystem::Initialize()
{
}

void CollisionSystem::OnEntityAdded(Entity & entity)
{
}

void CollisionSystem::OnEntityRemoved(Entity & entity)
{
}
