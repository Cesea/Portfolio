#pragma once

#include "SingletonBase.h"

#define CIRCLEGIZMO_SEGMENTS 36

class GizmoManager : public SingletonBase<GizmoManager>
{
private:
	typedef struct GizmoVertex
	{
		Vector3 pos;
		uint32 color;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};

private:
	LPDIRECT3DDEVICE9 _pDevice;
	Vector3		  _pCirclePos[CIRCLEGIZMO_SEGMENTS + 1];

public:
	bool Init(LPDIRECT3DDEVICE9 pDevice);
	void Release();

	void Line(const Vector3 &startPos, const Vector3 &endPos,
		uint32 color = 0xFF808080)const;

	void WorldGrid(float cellsize, int cellNum)const;

	void Circle(const Vector3 &centerPos, float radius,
		const Vector3 &axis = Vector3(0, 0, 1),
		uint32 color = 0xFF808080) const;

	void WireSphere(const Vector3 &centerPos, float radius,
		uint32 color = 0xFF808080)const;

	void AABBBox(const Vector3 &minPos, const Vector3 &maxPos,
		uint32 color = 0xFF808080)const;

	void RenderWorldGizmo(const Vector3 &pos);
};

#define GIZMOMANAGER GizmoManager::GetInstance()

