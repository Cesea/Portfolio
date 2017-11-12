#ifndef FRUSTUM_H
#define FRUSTUM_H

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

	//���� ����ü �ȿ� �ִ��� �Ǻ��Ѵ�
	bool IsSphereInFrustum(const Vector3 &center, float radius) const;

	void RenderGizmo();
};

#endif