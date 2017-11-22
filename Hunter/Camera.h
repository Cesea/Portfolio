#ifndef CAMERA_H
#define CAMERA_H

#include "TransformComponent.h"

#include "Frustum.h"

class Camera
{

public:

	float _fov;
	float _camNear;
	float _camFar;
	float _aspect;
	bool32 _ortho;
	float _orthoSize;


	Camera();
	~Camera();

	void UpdateCamToDevice();
	void PreUpdateMatrix();
	void UpdateMatrix();
	void UpdateFrustum();

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

	void ReadyRenderToTexture(int32 width, int32 height);
	void ReadyShadowTexture(int32 size);
	void RenderTextureBegin(uint32 backColor);
	void RenderTextureEnd();

	LPDIRECT3DTEXTURE9 GetRenderTexture();

	const Frustum &GetFrustum() const { return _frustum; }

	const TransformComponent &GetTransform() const { return _transform; }
	TransformComponent &GetTransform() { return _transform; }

	void Handle(const InputManager::KeyDownEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);
	void Handle(const InputManager::MouseReleasedEvent &event);
	void Handle(const InputManager::MouseMoveEvent &event);

private :
	LPDIRECT3DTEXTURE9		_pRenderTexture{};		//랜더링될 Texture
	LPDIRECT3DSURFACE9		_pRenderSurface{};		//랜더 Texture Deapth 버퍼와 Stencil 버퍼가 있는 Surface
	LPDIRECT3DSURFACE9		_pDeviceTargetSurface{};
	LPDIRECT3DSURFACE9		_pDeviceDepthAndStencilSurface{};

protected:

	Matrix _matView;
	Matrix _matProjection;
	Matrix _matViewProjection;
	TransformComponent _transform;

	bool32 _rotating{false};

	float _moveSpeed{1.0f};
	float _rotationSpeed{0.1f};

	float _verticalAngle{};
	float _horizontalAngle{};

	Vector3 _toMove;

	Frustum _frustum;
};

#endif