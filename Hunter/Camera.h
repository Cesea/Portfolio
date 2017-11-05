#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"

class Camera : public Transform
{
protected:
	float _fov;			//ī�޶� ȭ��
	float _camNear;		//ī�޶� Near
	float _camFar;		//ī�޶� Far

	Matrix _matView;		//�����
	Matrix _matProjection;	//�������
	Matrix _matViewProjection;	//�� * �������

	bool _bOrtho;		//������?

public:
	Camera();
	~Camera();


	//������İ� View ����� ������Ʈ
	void UpdateMatrix();

	//View ��İ� ���� ����� ����
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);

	//ī�޶� ���� ����� ��´�.
	const Matrix &GetViewMatrix() const 
	{
		return _matView;
	}

	const Matrix &GetProjectionMatrix() const 
	{
		return _matProjection;
	}

	const Matrix &GetViewProjectionMatrix() const 
	{
		return _matViewProjection;
	}

	//ȭ�� ����
	void SetFov(float fov) 
	{
		_fov = fov;
	}

	float GetFov() 
	{
		return _fov;
	}

	void OrthoToggle() 
	{
		_bOrtho = !_bOrtho;
	}

	//ȭ���� ��ġ�� ������ ī�޶��� ���� ���̸� ��´�
	void ComputeRay(Ray *pOutRay, const Vector2* screenPos);

	//���� ��ġ��  ȭ���� ��ġ�� ��´�.
	bool GetWorldPosToScreenPos(Vector2* pScreenPos, const Vector3* pWorldPos);
};



#endif