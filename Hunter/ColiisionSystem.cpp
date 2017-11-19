#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Update(float deltaTime,float checkRange)
{
	auto &entities = GetEntities();
	for (uint32 i = 0; i < entities.size(); ++i)
	{
		TransformComponent & transform = entities[i].GetComponent<TransformComponent>();
		CollisionComponent & collision = entities[i].GetComponent<CollisionComponent>();
		for (uint32 j = i; j < entities.size(); ++j)
		{
			TransformComponent& transform2 = entities[j].GetComponent<TransformComponent>();
			CollisionComponent & collision2 = entities[j].GetComponent<CollisionComponent>();
			Vector3 distanceVec = transform.GetWorldPosition() - transform2.GetWorldPosition();
			float distance = Vec3Length(&distanceVec);
			if (distance < checkRange)
			{
				//�Ѵ� ���������� �˻���������
				if (collision2.locked&& collision.locked) continue;
				//���� �ϳ��� �����Ǿ�����
				if ((collision.locked && !collision2.locked) || (!collision.locked && collision2.locked))
				{
					//�浹�ߴٸ�
					if (Collision_AABBToAABB(collision._boundingBox.localMinPos, collision2._boundingBox.localMinPos,
						collision._boundingBox.localMaxPos, collision2._boundingBox.localMaxPos))
					{
						//�����ϼ� ������ �̺�Ʈ

						//Ʈ���� �˻�
						if (collision.isTrigger)
						{

						}
						if (collision2.isTrigger)
						{

						}

					}
				}
				//�Ѵ� �����Ǿ� ���� ����
				if (!collision.locked && !collision2.locked)
				{
					IsBlocking(&transform, &collision._boundingBox, &transform2, &collision2._boundingBox, 1.0f);
					//Ʈ���� �˻�
					if (collision.isTrigger)
					{

					}
					if (collision2.isTrigger)
					{

					}
				}
			}
		}
	}
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
