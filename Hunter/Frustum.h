#ifndef FRUSTUM_H
#define FRUSTUM_H

#define FRUSTUM_OUT (0)
#define FRUSTUM_COMPLETLY_IN (1)
#define FRUSTUM_PARTIALLY_IN (2)

struct RenderComponent;

class Frustum
{
private:
	Vector3 _rHWPos[8]; // viewProjection 변환전 위치
	Vector3 _pos[8]; // 변환후 위치
	Plane _plane[6];
public:
	Frustum();
	~Frustum();

	//프러스텀 정보 업데이트(View * projection 행렬 필요)
	void UpdateFrustum(const Matrix &matViewProj);

	bool IsPointIntFrustum(const Vector3 &point) const;
	//구가 절두체 안에 있는지 판별한다
	bool IsSphereInFrustum(const Vector3 &center, float radius) const;

	void RenderGizmo();
};

#endif