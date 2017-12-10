#ifndef CAMERA_H
#define CAMERA_H

#include "TransformComponent.h"
#include "Frustum.h"

#include "Terrain.h"

class Player;

enum CAMERA_STATE
{
	CAMERASTATE_CREATE,
	CAMERASTATE_INCOMBAT,
	CAMERASTATE_UI,
};

constexpr float CAMERA_TARGET_DEFAULT_RADIUS = 5.0f;

//카메라 클래스는 예외적으로 texture를 가지고 있다
class Camera
{
public:

	float _fov;
	float _camNear;
	float _camFar;
	float _aspect;
	bool _ortho;
	float _orthoSize;

	Camera();
	~Camera();

	void CreateFromWorld(World &world);

	void MoveAndRotate(float deltaTime, const InputManager &input);

	const Entity &GetEntity() const { return _entity; }

	void UpdateMatrix();
	void UpdateCamToDevice();
	void UpdateFrustum();

	void SetTargetObject(Player *pTargetObject = nullptr);

	void SetMoveSpeed(float speed) { _moveSpeed = speed; }
	void SetRotationSpeed(float speed) { _rotationSpeed = speed; }

	void SetCameraState(CAMERA_STATE state);

	const Matrix &GetViewMatrix() const { return _matView; }
	const Matrix &GetProjectionMatrix() const { return _matProjection; }
	const Matrix &GetViewProjectionMatrix() const { return _matViewProjection; }

	void SetFov(float fov) { _fov = fov; }
	float GetFov() const { return _fov; }

	void OrthoToggle() { _ortho = !_ortho; }

	void ComputeRay(const Vector2 &screenPos, Ray *pOutRay);
	bool GetWorldPosToScreenPos(const Vector3 &worldPos, Vector2* pOutScreenPos);

	const Frustum &GetFrustum() const { return _frustum; }

	void ReadyRenderToTexture( int32 width, int32 height );
	void ReadyShadowTexture( int32 size );
	void RenderTextureBegin( uint32 backColor );
	void RenderTextureEnd();
	LPDIRECT3DTEXTURE9 GetRenderTexture();

private:

	LPDIRECT3DTEXTURE9 _pRenderTexture{};
	LPDIRECT3DSURFACE9 _pRenderSurface{};
	LPDIRECT3DSURFACE9 _pDeviceTargetSurface{};
	LPDIRECT3DSURFACE9 _pDeviceDepthAndStencilSurface{};

protected:
	Entity _entity;

	Matrix _matView;
	Matrix _matProjection;
	Matrix _matViewProjection;

	bool32 _rotating{false};

	float _moveSpeed{1.0f};
	float _rotationSpeed{0.1f};

	float _targetRadius{CAMERA_TARGET_DEFAULT_RADIUS};

	float _verticalAngle{};
	float _horizontalAngle{};

	Frustum _frustum;
	Player *_pTargetObject{};

	float _offsetForwardMult{};
	float _offsetUpMult{};

	Quaternion _camRot;
	CAMERA_STATE _cameraState{};

	TerrainTilePos _currentTilePos;
	TerrainTilePos _prevTilePos;


	void RepositionEntity(const TerrainTilePos &currentPos, const TerrainTilePos &prevPos);
};

#endif