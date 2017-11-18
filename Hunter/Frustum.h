#ifndef FRUSTUM_H
#define FRUSTUM_H

#define FRUSTUM_OUT (0)
#define FRUSTUM_COMPLETLY_IN (1)
#define FRUSTUM_PARTIALLY_IN (2)

struct RenderComponent;

class Frustum
{
private:
	Vector3 _rHWPos[8]; // viewProjection ��ȯ�� ��ġ
	Vector3 _pos[8]; // ��ȯ�� ��ġ
	Plane _plane[6];
public:
	Frustum();
	~Frustum();

	//�������� ���� ������Ʈ(View * projection ��� �ʿ�)
	void UpdateFrustum(const Matrix &matViewProj);

	bool IsPointIntFrustum(const Vector3 &point) const;
	//���� ����ü �ȿ� �ִ��� �Ǻ��Ѵ�
	bool IsSphereInFrustum(const Vector3 &center, float radius) const;

	void RenderGizmo();
};

#endif