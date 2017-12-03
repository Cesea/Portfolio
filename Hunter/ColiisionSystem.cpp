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

		aabb0 = SetAABB(collision._boundingBox._xSize, collision._boundingBox._ySize, collision._boundingBox._zSize, transform.GetWorldPosition());

		for (uint32 j = 0; j < entities.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			TransformComponent& transform2 = entities[j].GetComponent<TransformComponent>();
			CollisionComponent & collision2 = entities[j].GetComponent<CollisionComponent>();

			
			aabb1 = SetAABB(collision2._boundingBox._xSize, collision2._boundingBox._ySize, collision2._boundingBox._zSize, transform2.GetWorldPosition());

			Vector3 distanceVec = transform.GetWorldPosition() - transform2.GetWorldPosition();
			float distance = Vec3Length(&distanceVec);
			if (distance < checkRange)
			{
				//�Ѵ� ���������� �˻���������
				if (collision2._locked && collision._locked) continue;
				//���� �ϳ��� �����Ǿ�����
				//if ((collision._locked && !collision2._locked) || (!collision._locked && collision2._locked))
				else
				{
					//�浹�ߴٸ�
					if (Collision_AABBToAABB(aabb0._min,
						aabb0._max,
						aabb1._min, 
						aabb1._max))
					{
						//�����ϼ� ������ �̺�Ʈ
						//Console::Log("Collidededed!!!!\n");
						//Ʈ���� �˻�
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
