#ifndef CAMERA_H
#define CAMERA_H

#include "TransformComponent.h"

#include "Frustum.h"

class Camera
{

public:
	Camera();
	~Camera();

	//투영행렬과 View 행렬, 절두체를 최신화 한다
	void UpdateMatrix();

	//View 행렬과 투영 행렬을 셋팅
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);

	//카메라 관련 행렬을 얻는다.
	const Matrix &GetViewMatrix() const { return _matView; }
	const Matrix &GetProjectionMatrix() const { return _matProjection; }
	const Matrix &GetViewProjectionMatrix() const { return _matViewProjection; }

	//화각 셋팅
	void SetFov(float fov) { _fov = fov; }
	float GetFov() { return _fov; }

	void OrthoToggle() { _bOrtho = !_bOrtho; }

	//화면의 위치를 가지고 카메라의 투영 레이를 얻는다
	void ComputeRay(const Vector2 &screenPos, Ray *pOutRay);
	//월드 위치로  화면의 위치를 얻는다.
	bool GetWorldPosToScreenPos(const Vector3 &worldPos, Vector2* pOutScreenPos);

	const Frustum &GetFrustum() const { return _frustum; }

	const TransformComponent &GetTransform() const { return _transform; }
	TransformComponent &GetTransform() { return _transform; }

	void Handle(const InputManager::KeyDownEvent &event);
protected:
	float _fov;			//카메라 화각
	float _camNear;		//카메라 Near
	float _camFar;		//카메라 Far

	Matrix _matView;		//뷰행렬
	Matrix _matProjection;	//투영행렬
	Matrix _matViewProjection;	//뷰 * 투영행렬

	bool _bOrtho;		//직교니?

	float _moveSpeed;
	float _rotationSpeed;

	Vector3 _toMove;
	//float _friction;
	//float _aceel;

	Frustum _frustum;

	void UpdateFrustum();

	TransformComponent _transform;
};



#endif