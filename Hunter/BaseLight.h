#ifndef BASE_LIGHT_H
#define BASE_LIGHT_H

class Light
{
public:
	Light();
	~Light();

	void CreateFromWorld(World &world);

	//void DestroyFromWorld(World &world);


	// ����Ʈ ���
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	void SetWorldPosition(const Vector3 &position);
	void MoveSelf(const Vector3 &position);
	void SetRotateWorld(float x, float y, float z);

	virtual void SetTarget(const Vector3 &target) {};

	//����Ʈ ����� ��´�.
	virtual Matrix GetLightMatrix() = 0;

	//����Ʈ ����� �׸���.
	virtual void RenderGizmo() = 0;

public:
	D3DXCOLOR _color;
	float _intensity;
	Entity _entity;
};

#endif
