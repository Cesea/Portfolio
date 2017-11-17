#ifndef CAMERA_H
#define CAMERA_H

#include "TransformComponent.h"

#include "Frustum.h"

class Camera
{

public:
	Camera();
	~Camera();

	void UpdateMatrix();

	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);

	void SetMoveSpeed(float speed) { _moveSpeed = speed; }
	void SetRotationSpeed(float speed) { _rotationSpeed = speed; }

	const Matrix &GetViewMatrix() const { return _matView; }
	const Matrix &GetProjectionMatrix() const { return _matProjection; }
	const Matrix &GetViewProjectionMatrix() const { return _matViewProjection; }

	void SetFov(float fov) { _fov = fov; }
	float GetFov() { return _fov; }

	void OrthoToggle() { _ortho = !_ortho; }

	//화면의 위치를 가지고 카메라의 투영 레이를 얻는다
	void ComputeRay(const Vector2 &screenPos, Ray *pOutRay);
	//월드 위치로  화면의 위치를 얻는다.
	bool GetWorldPosToScreenPos(const Vector3 &worldPos, Vector2* pOutScreenPos);

	const Frustum &GetFrustum() const { return _frustum; }

	const TransformComponent &GetTransform() const { return _transform; }
	TransformComponent &GetTransform() { return _transform; }

	void Handle(const InputManager::KeyDownEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);
	void Handle(const InputManager::MouseReleasedEvent &event);
	void Handle(const InputManager::MouseMoveEvent &event);

protected:

	float _fov;
	float _camNear;
	float _camFar;	

	Matrix _matView;
	Matrix _matProjection;
	Matrix _matViewProjection;
	TransformComponent _transform;

	bool32 _ortho;
	bool32 _rotating{false};

	float _moveSpeed{1.0f};
	float _rotationSpeed{0.4f};

	Vector3 _toMove;

	Frustum _frustum;
};

#endif