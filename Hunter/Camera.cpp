#include "stdafx.h"
#include "Camera.h"

//void Camera::UpdateFrustum()
//{
//}
//
//Camera::Camera()
//{
//	//기본 화각 설정
//	_fov = 60.f * ONE_RAD;
//
//	//기본 Near
//	_camNear = 0.001f;
//
//	//기본 Far
//	_camFar = 1000.f;
//
//	_moveSpeed = 1.0f;
//	_rotationSpeed = 1.0f;
//
//	EventChannel channel;
//	channel.Add<InputSystem::KeyDownEvent, Camera>(*this) ;
//}
//
//Camera::~Camera()
//{
//}
//
//void Camera::UpdateMatrix()
//{
//	//이동값이 남아있다면 움직여라
//	if (!_toMove.IsZero())
//	{
//		this->MovePositionSelf(_toMove);
//		_toMove = Vector3(0.0f, 0.0f, 0.0f);
//	}
//	//화각에 의한 Projection 행렬 업데이트
//	MatrixPerspectiveFovLH(
//		&_matProjection,
//		_fov,
//		static_cast<float>(WINSIZEX) / static_cast<float>(WINSIZEY),
//		_camNear,
//		_camFar);
//
//	//뷰행렬 카메라 월드위치에 대한 역행렬이다.
//	MatrixInverse(&_matView, NULL, &_matFinal);
//
//	_matViewProjection = _matView * _matProjection;
//}
//
//void Camera::UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice)
//{	//행렬 업데이트 해주고 
//	this->UpdateMatrix();
//	//셋팅
//	pDevice->SetTransform(D3DTS_VIEW, &_matView);
//	pDevice->SetTransform(D3DTS_PROJECTION, &_matProjection);
//}
//
//void Camera::ComputeRay(Ray * pOutRay, const Vector2 * screenPos)
//{
//	//Device 의 뷰포트를 얻는다.
//	D3DVIEWPORT9 viewPort;
//	gpDevice->GetViewport(&viewPort);
//
//	//스크린의 위치 비율을 얻자 
//	float factorX = screenPos->x / viewPort.Width;
//	float factorY = (1.0f - (screenPos->y / viewPort.Height));
//
//	//0 ~ 1 => -1 ~ 1
//	factorX = factorX * 2.0f - 1.0f;
//	factorY = factorY * 2.0f - 1.0f;
//
//	//투영행렬을 얻는다.
//	Matrix matProj = GetProjectionMatrix();
//
//	//
//
//	//동차로 위치에 화각 스케일량을 나눈다.
//	Vector3 direction(
//		factorX / matProj._11,
//		factorY / matProj._22,
//		1.0f);
//
//	//카메라 월드 행렬
//	Matrix matCamWorld = GetFinalMatrix();
//
//	//레이의 방향을 동차에서 얻은 벡터를 카메라 월드 매트릭스
//	//월드 방향이 나온다.
//	Vec3TransformNormal(
//		&direction, &direction, &matCamWorld);
//
//	//방향벡터 정규
//	Vec3Normalize(&direction, &direction);
//
//	//레이의 방향
//	pOutRay->direction = direction;
//
//	//레이의 오리진은 카메라 위치가 된다.
//	pOutRay->origin = this->GetWorldPosition();
//}
//
//bool Camera::GetWorldPosToScreenPos(Vector2 * pScreenPos, const Vector3 * pWorldPos)
//{
//	//카메라의 ViewProjection 행렬을 얻는다.
//	Matrix matViewProj = GetViewProjectionMatrix();
//
//	//월드 위치를 동차로...
//	Vector3 pos;
//	Vec3TransformCoord(&pos, pWorldPos, &matViewProj);
//
//	//동차공간
//	//				*-------*<-( 1, 1, 1 )
//	//			   /|      /|
//	//			  *-------* |
//	//            | *-----|-*
//	//		      |/      |/
//	//(-1,-1, 0)->*-------* 
//
//	//0 ~ 1 사이 값에 없으면 이면 카메라 뒤에 있다는예기
//	if (pos.z < 0.0f || pos.z > 1.0f)
//		return false;
//
//	//화면 중앙에 있다면 pos xy 값은 다음과 같다.
//	//pos.x == 0  
//	//pos.y == 0
//
//	//화면 절반 사이즈
//	float halfScreenW = WINSIZEX * 0.5f;
//	float halfScreenH = WINSIZEY * 0.5f;
//
//	//최종 화면 위치
//	pScreenPos->x = halfScreenW + (halfScreenW * pos.x);
//	pScreenPos->y = halfScreenH - (halfScreenH * pos.y);	//Y 값은 반전
//
//	return true;
//}
//
//void Camera::Handle(const InputSystem::KeyDownEvent & event)
//{
//	Vector3 diff = Vector3(0.0f, 0.0f, 0.0f);
//	float deltaTime = APPTIMER->GetTargetTime();
//	//move forward
//	if (event.code == 'W')
//	{
//		_toMove.z += _moveSpeed * deltaTime;
//	}
//	else if (event.code == 'S')
//	{
//		_toMove.z -= _moveSpeed * deltaTime;
//	}
//	else if (event.code == 'A')
//	{
//		_toMove.x -= _moveSpeed * deltaTime;
//	}
//	else if (event.code == 'D')
//	{
//		_toMove.x += _moveSpeed * deltaTime;
//	}
//	else if (event.code == 'Q')
//	{
//		_toMove.y += _moveSpeed * deltaTime;
//	}
//	else if (event.code == 'E')
//	{
//		_toMove.y -= _moveSpeed * deltaTime;
//	}
//}
//
//void Frustum::Cull(const AABB * aabbs, bool32 * pOutVisible, int32 numAABB)
//{
//}
