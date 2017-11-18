#include "stdafx.h"
#include "Frustum.h"

#define PLANE_EPSILON 5.0f

Frustum::Frustum()
{
	_rHWPos[0] = Vector3(-1, 1, 0);
	_rHWPos[1] = Vector3(1, 1, 0);
	_rHWPos[2] = Vector3(-1, -1, 0);
	_rHWPos[3] = Vector3(1, -1, 0);

	_rHWPos[4] = Vector3(-1, 1, 1);
	_rHWPos[5] = Vector3(1, 1, 1);
	_rHWPos[6] = Vector3(-1, -1, 1);
	_rHWPos[7] = Vector3(1, -1, 1);
}

Frustum::~Frustum()
{
}

void Frustum::UpdateFrustum(const Matrix & matViewProj)
{
	//viewPojection 역행렬
	Matrix inverse;
	MatrixInverse(&inverse, NULL, &matViewProj);

	for (int i = 0; i < 8; i++)
	{
		Vec3TransformCoord(_pos + i, _rHWPos + i, &inverse);
	}

	//정면플랜
	_plane[0].Create(_pos[0], _pos[1], _pos[2]);
	//후면
	_plane[1].Create(_pos[5], _pos[4], _pos[7]);
	//우측
	_plane[2].Create(_pos[1], _pos[5], _pos[3]);
	//좌측
	_plane[3].Create(_pos[4], _pos[0], _pos[6]);
	//상단
	_plane[4].Create(_pos[4], _pos[5], _pos[0]);
	//하단
	_plane[5].Create(_pos[2], _pos[3], _pos[6]);
}

bool Frustum::IsPointIntFrustum(const Vector3 & point) const
{
	float fDist{};
	fDist = D3DXPlaneDotCoord(&_plane[3], &point);
	if (fDist > PLANE_EPSILON)
	{
		return false;
	}
	fDist = D3DXPlaneDotCoord(&_plane[2], &point);
	if (fDist > PLANE_EPSILON)
	{
		return false;
	}
	fDist = D3DXPlaneDotCoord(&_plane[3], &point);
	if (fDist > PLANE_EPSILON)
	{
		return false;
	}
	return true;
}

bool Frustum::IsSphereInFrustum(const Vector3 &center, float radius) const
{
	Vector3 normal;
	for (int i = 0; i < 6; i++)
	{
		normal.x = _plane[i].a;
		normal.y = _plane[i].b;
		normal.z = _plane[i].c;

		//면과 거리 
		float distance = Vec3Dot(&normal, &center) + _plane[i].d;
		//반지름 이상 벗어나면 정두체 안에 없음
		if (distance > radius)
		{
			return false;
		}
	}
	return true;
}

void Frustum::RenderGizmo()
{
}
