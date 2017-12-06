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

	//static int32 counter = 0;
	//if (counter % 60 == 0)
	//{
	//	Console::Log("LoopCount : %d\n", entities.size());
	//}
	//counter++;

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
				bool checkCollision = false;
				if (collision2._locked && collision._locked) continue;
				//둘중 하나가 고정되어있음
				//if ((collision._locked && !collision2._locked) || (!collision._locked && collision2._locked))
				else
				{
					//충돌타입이 어떻게 되는가
					switch (collision._type)
					{
					case CollisionComponent::COLLISION_TYPE_BOX:
					{
						if (collision2._type == CollisionComponent::COLLISION_TYPE_BOX)
						{
							if (Collision_AABBToAABB(aabb0._min,
								aabb0._max,
								aabb1._min,
								aabb1._max))
							{
								if (collision._isTrigger)
								{
									//behind : someone that send  front : someone that receive
									switch (collision._triggerType)
									{
									case CollisionComponent::TRIGGER_TYPE_ENEMY:
									case CollisionComponent::TRIGGER_TYPE_PLAYER:
										_channel.Broadcast<ActorTriggerEvent>(
											ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_OBJECT:
										_channel.Broadcast<ObjectTriggerEvent>(
											ObjectTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									}

								}
								if (collision2._isTrigger)
								{
									switch (collision._triggerType)
									{
									case CollisionComponent::TRIGGER_TYPE_ENEMY:
									case CollisionComponent::TRIGGER_TYPE_PLAYER:
										_channel.Broadcast<ActorTriggerEvent>(
											ActorTriggerEvent(entities[j], entities[i]));
										break;
									case CollisionComponent::TRIGGER_TYPE_OBJECT:
										_channel.Broadcast<ObjectTriggerEvent>(
											ObjectTriggerEvent(entities[j], entities[i]));
										break;
									case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									}
								}
								//오브젝트와 충돌했다면 민다
								if (collision._triggerType == CollisionComponent::TRIGGER_TYPE_OBJECT)
								{
									IsBlocking(&transform, &aabb0, &transform2, &aabb1, 0.0f);
								}
								if (collision2._triggerType == CollisionComponent::TRIGGER_TYPE_OBJECT)
								{
									IsBlocking(&transform, &aabb0, &transform2, &aabb1, 1.0f);
								}

							}
						}
						else if(collision2._type == CollisionComponent::COLLISION_TYPE_OBB)
						{
							Matrix a = transform2.GetFinalMatrix();
							float b;
							MatrixInverse(&a, &b, &a);
							Vector3 xVec = Vector3(a._11, a._21, a._31);
							Vector3 yVec = Vector3(a._12, a._22, a._32);
							Vector3 zVec = Vector3(a._13, a._23, a._33);
							if (Collision_AABBToOBB(aabb0._min, aabb0._max, aabb1._center, 
								-xVec, yVec, -zVec, 
								aabb1._xSize, aabb1._ySize, aabb1._zSize))
							{
								if (collision._isTrigger)
								{
									switch (collision._triggerType)
									{
									case CollisionComponent::TRIGGER_TYPE_ENEMY:
									case CollisionComponent::TRIGGER_TYPE_PLAYER:
										_channel.Broadcast<ActorTriggerEvent>(
											ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_OBJECT:
										_channel.Broadcast<ObjectTriggerEvent>(
											ObjectTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									}

								}
								if (collision2._isTrigger)
								{
									switch (collision._triggerType)
									{
									case CollisionComponent::TRIGGER_TYPE_ENEMY:
									case CollisionComponent::TRIGGER_TYPE_PLAYER:
										_channel.Broadcast<ActorTriggerEvent>(
											ActorTriggerEvent(entities[j], entities[i]));
										break;
									case CollisionComponent::TRIGGER_TYPE_OBJECT:
										_channel.Broadcast<ObjectTriggerEvent>(
											ObjectTriggerEvent(entities[j], entities[i]));
										break;
									case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									}
								}
								//오브젝트와 충돌했다면 민다
								if (collision._triggerType == CollisionComponent::TRIGGER_TYPE_OBJECT)
								{
									IsBlocking(&transform, &aabb0, &transform2, &aabb1, 0.0f);
								}
								if (collision2._triggerType == CollisionComponent::TRIGGER_TYPE_OBJECT)
								{
									IsBlocking(&transform, &aabb0, &transform2, &aabb1, 1.0f);
								}
							}
						}
					}
						break;
					case CollisionComponent::COLLISION_TYPE_OBB:
					{
						if (collision2._type == CollisionComponent::COLLISION_TYPE_BOX)
						{
							Matrix a = transform.GetFinalMatrix();
							float b;
							MatrixInverse(&a, &b, &a);
							Vector3 xVec = Vector3(a._11, a._21, a._31);
							Vector3 yVec = Vector3(a._12, a._22, a._32);
							Vector3 zVec = Vector3(a._13, a._23, a._33);
							if (Collision_AABBToOBB(aabb1._min, aabb1._max, aabb0._center, -xVec, yVec, -zVec, aabb0._xSize, aabb0._ySize, aabb0._zSize))
							{
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
									case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
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
									case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									}
								}
								//오브젝트와 충돌했다면 민다
								if (collision._triggerType == CollisionComponent::TRIGGER_TYPE_OBJECT)
								{
									IsBlocking(&transform, &aabb0, &transform2, &aabb1, 0.0f);
								}
								if (collision2._triggerType == CollisionComponent::TRIGGER_TYPE_OBJECT)
								{
									IsBlocking(&transform, &aabb0, &transform2, &aabb1, 1.0f);
								}
							}
						}
						else if (collision2._type == CollisionComponent::COLLISION_TYPE_OBB)
						{
							Matrix a = transform.GetFinalMatrix();
							float b;
							MatrixInverse(&a, &b, &a);
							Vector3 xVec = Vector3(a._11, a._21, a._31);
							Vector3 yVec = Vector3(a._12, a._22, a._32);
							Vector3 zVec = Vector3(a._13, a._23, a._33);

							a = transform2.GetFinalMatrix();
							MatrixInverse(&a, &b, &a);
							Vector3 xVec2 = Vector3(a._11, a._21, a._31);
							Vector3 yVec2 = Vector3(a._12, a._22, a._32);
							Vector3 zVec2 = Vector3(a._13, a._23, a._33);

							if (Collision_OBBToOBB(aabb0._center,-xVec,yVec,-zVec,aabb0._xSize, aabb0._ySize, aabb0._zSize,aabb1._center,-xVec2, yVec2, -zVec2,aabb1._xSize,aabb1._ySize,aabb1._zSize))
							{
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
									case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
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
									case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX:
										_channel.Broadcast<ActorTriggerEvent>(ActorTriggerEvent(entities[i], entities[j]));
										break;
									}
								}
								//오브젝트와 충돌했다면 민다
								if (collision._triggerType == CollisionComponent::TRIGGER_TYPE_OBJECT)
								{
									IsBlocking(&transform, &aabb0, &transform2, &aabb1, 0.0f);
								}
								if (collision2._triggerType == CollisionComponent::TRIGGER_TYPE_OBJECT)
								{
									IsBlocking(&transform, &aabb0, &transform2, &aabb1, 1.0f);
								}
							} 
						}
					}
						break;
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

void CollisionSystem::render()
{
	auto &entities = GetEntities();

	Vector3 hitPos;
	for (uint32 i = 0; i < entities.size(); ++i)
	{
		CollisionComponent &refCollision = entities[i].GetComponent<CollisionComponent>();
		TransformComponent &transform = entities[i].GetComponent<TransformComponent>();

		refCollision.RenderBoxGizmo(transform);

		Matrix a = transform.GetFinalMatrix();
		float b;
		MatrixInverse(&a, &b, &a);
		Vector3 xVec = Vector3(a._11, a._21, a._31);
		Vector3 yVec = Vector3(a._12, a._22, a._32);
		Vector3 zVec = Vector3(a._13, a._23, a._33);

		GIZMOMANAGER->Line(transform.GetWorldPosition(), transform.GetWorldPosition()- xVec, 0xFFFF0000);
		GIZMOMANAGER->Line(transform.GetWorldPosition(), transform.GetWorldPosition()+yVec, 0xFF00FF00);
		GIZMOMANAGER->Line(transform.GetWorldPosition(), transform.GetWorldPosition()-zVec, 0xFF0000FF);

	}
}
