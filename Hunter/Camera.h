#ifndef CAMERA_H
#define CAMERA_H

#define PLAYER_TO_CAMERA_DIST 3.0f

#include "TransformComponent.h"
#include "Frustum.h"
#include "BaseGameObject.h"

enum cameraState
{
	cCreativeMode,
	cNormal,
	cPlaying,
	cDirection,
	//Editer
};

class Camera
{
public:
	cameraState _cameraState;

	float _curDist;

	float _fov;
	float _camNear;
	float _camFar;

	Camera();
	~Camera();

	void CreateFromWorld(World &world);

	void PreUpdateMatrix();
	void UpdateMatrix();
	void UpdateCamToDevice();
	void UpdateFrustum();

	void SetTargetObject(BaseGameObject *pTargetObject = nullptr);

	void SetMoveSpeed(float speed) { _moveSpeed = speed; }
	void SetRotationSpeed(float speed) { _rotationSpeed = speed; }

	const Matrix &GetViewMatrix() const { return _matView; }
	const Matrix &GetProjectionMatrix() const { return _matProjection; }
	const Matrix &GetViewProjectionMatrix() const { return _matViewProjection; }

	void SetFov(float fov) { _fov = fov; }
	float GetFov() const { return _fov; }

	void OrthoToggle() { _ortho = !_ortho; }

	void ComputeRay(const Vector2 &screenPos, Ray *pOutRay);
	bool GetWorldPosToScreenPos(const Vector3 &worldPos, Vector2* pOutScreenPos);

	const Frustum &GetFrustum() const { return _frustum; }

	void Handle(const InputManager::KeyDownEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);
	void Handle(const InputManager::MouseReleasedEvent &event);
	void Handle(const InputManager::MouseMoveEvent &event);

	const Entity &GetEntity() const { return _entity; }

	const Entity &GetDummyEntity() const { return _dummyEntity; }

private:

	

	void NormalCameraUpdate(void);
protected:

	Entity _entity;

	Matrix _matView;
	Matrix _matProjection;
	Matrix _matViewProjection;

	bool32 _ortho;
	bool32 _rotating{false};

	float _moveSpeed{1.0f};
	float _rotationSpeed{0.1f};

	float _verticalAngle{};
	float _horizontalAngle{};

	TransformComponent* cameraTransform;
	TransformComponent* targetTransform;

	TransformComponent* dummyTransform;

	Entity _dummyEntity;

	Vector3 _toMove;

	Frustum _frustum;
	BaseGameObject *_pTargetObject{};
	
};

#endif