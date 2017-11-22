#ifndef BASE_LIGHT_H
#define BASE_LIGHT_H

class Light
{
public:
	D3DXCOLOR			_color;				
	float				_intensity;		

public:
	Light();
	~Light();

	void CreateFromWorld(World &world);
	//void DestroyFromWorld(World &world);

	Entity _entity;

	// ����Ʈ ���
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	virtual void SetWorldPosition(const Vector3 &position) = 0;
	virtual void SetRotateWorld(float x, float y, float z) = 0;

	//����Ʈ ����� ��´�.
	virtual Matrix GetLightMatrix() = 0;

	//����Ʈ ����� �׸���.
	virtual void RenderGizmo() = 0;
};

#endif
