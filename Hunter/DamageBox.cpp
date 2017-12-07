#include "stdafx.h"
#include "DamageBox.h"

DamageBox::DamageBox()
{
}


DamageBox::~DamageBox()
{
}

void DamageBox::RepositionEntity(const TerrainTilePos & currentPos, const TerrainTilePos & prevPos)
{
	//타일이 다를....... tile의 entity벡터를 처리해주자
	if (currentPos._tileX != prevPos._tileX || currentPos._tileZ != prevPos._tileZ)
	{
		Terrain::TerrainChunk &refPrevChunk =  TERRAIN->GetChunkAt(prevPos._chunkX, prevPos._chunkZ);
		Terrain::TerrainTile &refPrevTile = refPrevChunk._tiles[Index2D(prevPos._tileX, prevPos._tileZ, TERRAIN_TILE_RES)];

		for (uint32 i = 0; i < refPrevTile._entities.size(); ++i)
		{
			if (refPrevTile._entities[i] == _entity)
			{
				refPrevTile._entities.erase(refPrevTile._entities.begin() + i);
				break;
			}
		}

		Terrain::TerrainChunk &refCurrentChunk =  TERRAIN->GetChunkAt(currentPos._chunkX, currentPos._chunkZ);
		Terrain::TerrainTile &refCurrentTile = refCurrentChunk._tiles[Index2D(currentPos._tileX, currentPos._tileZ, TERRAIN_TILE_RES)];
		refCurrentTile._entities.push_back(_entity);
	}
}


bool DamageBox::CreateFromWorld(World & world, const Vector3 & Pos)
{
	_entity = world.CreateEntity();
	TransformComponent &transform = _entity.AddComponent<TransformComponent>();
	transform.SetWorldPosition((Pos));

	TERRAIN->ConvertWorldPostoTilePos(Pos, &_tilePos);

	CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
	collision._locked = false;
	collision._isTrigger = true;
	collision._type = CollisionComponent::COLLISION_TYPE_BOX;

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(DamageBox,Update, *this));

	collision._valid = true;

	EventChannel channel;
	channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(GameObjectFactory::ObjectCreatedEvent(
		ARCHE_NONE, _entity, Pos));

	_entity.Activate();
	return true;
}

void DamageBox::Update(float deltaTime)
{
	CollisionComponent &collision = _entity.GetComponent<CollisionComponent>();
	TransformComponent &refTrans = _entity.GetComponent<TransformComponent>();
	collision._duration -= deltaTime;
	//위치값 업데이트....

	//위치값을 모두 업데이트 해 준 후에...
	_prevTilePos = _tilePos;
	TERRAIN->ConvertWorldPostoTilePos(refTrans.GetWorldPosition(), &_tilePos);

	RepositionEntity(_tilePos, _prevTilePos);

	if (collision._duration < 0.0f)
	{
		EventChannel channel;
		_valid = false;

		TERRAIN->RemoveEntityInTile(_entity, _tilePos);
		channel.Broadcast<IScene::SceneDirty>(IScene::SceneDirty());
	}
}
