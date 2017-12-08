#pragma once
#include "BaseGameObject.h"
class DamageBox : public BaseGameObject
{
public:
	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);


public:

	DamageBox();
	~DamageBox();

	void RepositionEntity(const TerrainTilePos &currentPos, const TerrainTilePos &prevPos);
};

