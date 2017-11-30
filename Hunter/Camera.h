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

//카메라 클래스는 예외적으로 texture를 가지고 있다
class Camera
{
public:
	cameraState _cameraState{};

	float _curDist;

	float _fov;
	float _camNear;
	float _camFar;

	Camera();
	~Camera();

	void CreateFromWorld(World &world);

	void MoveAndRotate(const InputManager &input);

	//void PreUpdateMatrix();
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

	const Entity &GetEntity() const { return _entity; }

	const Entity &GetDummyEntity() const { return _dummyEntity; }

	void ReadyRenderToTexture( int32 width, int32 height );
	void ReadyShadowTexture( int32 size );
	void RenderTextureBegin( uint32 backColor );
	void RenderTextureEnd();
	LPDIRECT3DTEXTURE9 GetRenderTexture();

private:

	void NormalCameraUpdate(void);

	LPDIRECT3DTEXTURE9 _pRenderTexture{};
	LPDIRECT3DSURFACE9 _pRenderSurface{};
	LPDIRECT3DSURFACE9 _pDeviceTargetSurface{};
	LPDIRECT3DSURFACE9 _pDeviceDepthAndStencilSurface{};

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

	TransformComponent* cameraTransform{};
	TransformComponent* targetTransform{};

	TransformComponent* dummyTransform{};

	Entity _dummyEntity;

	Vector3 _toMove;

	Frustum _frustum;
	BaseGameObject *_pTargetObject{};
	
};

#endif