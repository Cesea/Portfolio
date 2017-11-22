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

	// 라이트 행렬
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	virtual void SetWorldPosition(const Vector3 &position) = 0;
	virtual void SetRotateWorld(float x, float y, float z) = 0;

	//라이트 행렬을 얻는다.
	virtual Matrix GetLightMatrix() = 0;

	//라이트 기즈모를 그린다.
	virtual void RenderGizmo() = 0;
};

#endif
