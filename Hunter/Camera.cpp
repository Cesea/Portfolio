#include "stdafx.h"
#include "Camera.h"

constexpr float MAX_VERT_ANGLE = 85.0f;
constexpr float MIN_VERT_ANGLE = -85.0f;

Camera::Camera()
{
	//기본 화각 설정
	_fov = 60.f * ONE_RAD;

	//기본 Near
	_camNear = 0.01f;

	//기본 Far
#if defined (DEBUG) || defined (_DEBUG)
	_camFar = 500.0f;
#else 
	_camFar = 600.0f;
#endif

	_moveSpeed = 1.0f;
	_rotationSpeed = 1.0f;

	_cameraState = cCreativeMode;

	_curDist = 0;

	//playerPos = { 0,0,0 };
}

Camera::~Camera()
{
	//if(플레이어를 따라가는 모드일때)
	//{
	//	TransformComponent &targetTransform = _pTargetObject->_entity.GetComponent<TransformComponent>();
	//}
}

void Camera::CreateFromWorld(World & world)
{
	EventChannel channel;
	channel.Add<InputManager::KeyDownEvent, Camera>(*this);
	channel.Add<InputManager::MousePressedEvent, Camera>(*this);
	channel.Add<InputManager::MouseReleasedEvent, Camera>(*this);
	channel.Add<InputManager::MouseMoveEvent, Camera>(*this);

	_entity = world.CreateEntity();
	_entity.AddComponent<TransformComponent>();


	_entity.Activate();


	_dummyEntity = world.CreateEntity();
	_dummyEntity.AddComponent<TransformComponent>();

	_dummyEntity.Activate();

	cameraTransform = &this->GetEntity().GetComponent<TransformComponent>();

	dummyTransform = &this->GetDummyEntity().GetComponent<TransformComponent>();



	int a = 0;
}

void Camera::PreUpdateMatrix()
{
	//이동값이 남아있다면 움직여라
	if (!_toMove.IsZero())
	{
		_entity.GetComponent<TransformComponent>().MovePositionSelf(_toMove);
		_toMove = Vector3(0.0f, 0.0f, 0.0f);
	}
}

void Camera::UpdateMatrix()
{
	//화각에 의한 Projection 행렬 업데이트
	MatrixPerspectiveFovLH(
		&_matProjection,
		_fov,
		static_cast<float>(WINSIZEX) / static_cast<float>(WINSIZEY),
		_camNear,
		_camFar);

	//뷰행렬 카메라 월드위치에 대한 역행렬이다.
	MatrixInverse(&_matView, NULL, &_entity.GetComponent<TransformComponent>()._matFinal);

	_matViewProjection = _matView * _matProjection;
}

void Camera::UpdateCamToDevice()
{
	
	gpDevice->SetTransform(D3DTS_VIEW, &_matView);
	gpDevice->SetTransform(D3DTS_PROJECTION, &_matProjection);

	if (_cameraState == cNormal)
	{
		Vector3 tPos = targetTransform->GetWorldPosition();
		tPos.y = targetTransform->GetWorldPosition().y + 1.5f;
		
		dummyTransform->SetWorldPosition(targetTransform->GetWorldPosition().x, targetTransform->GetWorldPosition().y + 2.5, targetTransform->GetWorldPosition().z);

		if (_curDist < PLAYER_TO_CAMERA_DIST)
		{
			Vector3 dir = tPos - cameraTransform->GetWorldPosition();
			D3DXVec3Normalize(&dir, &dir);

			cameraTransform->SetForward(dir);

			NormalCameraUpdate();
		}
		
		//cameraTransform->SetWorldPosition(cameraTransform->GetWorldPosition().x, cameraTransform->GetWorldPosition().y, cameraTransform->GetWorldPosition().z + _curDist);
	}

}

void Camera::UpdateFrustum()
{
	_frustum.UpdateFrustum(*this);
}

void Camera::SetTargetObject(BaseGameObject * pTargetObject)
{
	if (nullptr == pTargetObject)
	{
		return;
	}
	_pTargetObject = pTargetObject;
}

void Camera::ComputeRay(const Vector2 & screenPos, Ray * pOutRay)
{
	//Device 의 뷰포트를 얻는다.
	D3DVIEWPORT9 viewPort;
	gpDevice->GetViewport(&viewPort);

	//스크린의 위치 비율을 얻자 
	float factorX = screenPos.x / viewPort.Width;
	float factorY = (1.0f - (screenPos.y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;

	//동차로 위치에 화각 스케일량을 나눈다.
	Vector3 direction( factorX / _matProjection._11, factorY / _matProjection._22, 1.0f);

	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	//카메라 월드 행렬
	Matrix matCamWorld = refTransform.GetFinalMatrix();

	//레이의 방향을 동차에서 얻은 벡터를 카메라 월드 매트릭스
	//월드 방향이 나온다.
	Vec3TransformNormal(
		&direction, &direction, &matCamWorld);

	//방향벡터 정규
	Vec3Normalize(&direction, &direction);

	//레이의 방향
	pOutRay->direction = direction;

	//레이의 오리진은 카메라 위치가 된다.
	pOutRay->origin = refTransform.GetWorldPosition();
}

bool Camera::GetWorldPosToScreenPos(const Vector3 & worldPos, Vector2 * pOutScreenPos)
{
	//카메라의 ViewProjection 행렬을 얻는다.
	Matrix matViewProj = GetViewProjectionMatrix();

	//월드 위치를 동차로...
	Vector3 pos;
	Vec3TransformCoord(&pos, &worldPos, &matViewProj);

	//동차공간
	//				*-------*<-( 1, 1, 1 )
	//			   /|      /|
	//			  *-------* |
	//            | *-----|-*
	//		      |/      |/
	//(-1,-1, 0)->*-------* 

	//0 ~ 1 사이 값에 없으면 이면 카메라 뒤에 있다는예기
	if (pos.z < 0.0f || pos.z > 1.0f)
		return false;

	//화면 중앙에 있다면 pos xy 값은 다음과 같다.
	//pos.x == 0  
	//pos.y == 0

	//화면 절반 사이즈
	float halfScreenW = WINSIZEX * 0.5f;
	float halfScreenH = WINSIZEY * 0.5f;

	//최종 화면 위치
	pOutScreenPos->x = halfScreenW + (halfScreenW * pos.x);
	pOutScreenPos->y = halfScreenH - (halfScreenH * pos.y);	//Y 값은 반전

	return true;
}

void Camera::Handle(const InputManager::KeyDownEvent & event)
{
	Vector3 diff = Vector3(0.0f, 0.0f, 0.0f);
	float deltaTime = APPTIMER->GetTargetTime();

	if (event.code == '1'&& _cameraState != cCreativeMode)
	{
		_curDist = 0.0f;
		_cameraState = cCreativeMode;
		//ShowCursor(true);
		cameraTransform->ReleaseParent();
	}
	if (event.code == '2' && _cameraState != cNormal)
	{
		_cameraState = cNormal;

		targetTransform = &_pTargetObject->_entity.GetComponent<TransformComponent>();

		//cameraTransform->SetWorldPosition(targetTransform->GetWorldPosition().x, targetTransform->GetWorldPosition().y + 2.5, targetTransform->GetWorldPosition().z + 0.01);

		//cameraTransform->SetRotateWorld(targetTransform.GetWorldRotateQuaternion());

		//cameraTransform->SetForward(-targetTransform->GetForward());

		/*_dummyEntity.GetComponent<TransformComponent>().SetForward(-targetTransform->GetForward());

		_dummyEntity.GetComponent<TransformComponent>().SetWorldPosition(targetTransform->GetWorldPosition().x, targetTransform->GetWorldPosition().y + 2.5, targetTransform->GetWorldPosition().z);

		_dummyEntity.GetComponent<TransformComponent>().AddChild(cameraTransform);*/

		dummyTransform->SetForward(targetTransform->GetForward());

		dummyTransform->SetWorldPosition(targetTransform->GetWorldPosition().x, targetTransform->GetWorldPosition().y + 2.5, targetTransform->GetWorldPosition().z);

		dummyTransform->AddChild(cameraTransform);

		

		cameraTransform->SetLocalPosition(0, 0, 0);

		

		//ShowCursor(false);
	}
	if (event.code == '3')
	{
		if (_curDist > 3)
		{
			int a = 0;
		}
	}

	//move forward
	switch (_cameraState)
	{
	case cCreativeMode:
		if (event.code == 'W')
		{
			_toMove.z += _moveSpeed * deltaTime;
		}
		else if (event.code == 'S')
		{
			_toMove.z -= _moveSpeed * deltaTime;
		}
		else if (event.code == 'A')
		{
			_toMove.x -= _moveSpeed * deltaTime;
		}
		else if (event.code == 'D')
		{
			_toMove.x += _moveSpeed * deltaTime;
		}
		else if (event.code == 'Q')
		{
			_toMove.y += _moveSpeed * deltaTime;
		}
		else if (event.code == 'E')
		{
			_toMove.y -= _moveSpeed * deltaTime;
		}
		break;
	case cNormal:

		break;
	}
	


}

void Camera::Handle(const InputManager::MousePressedEvent & event)
{
	switch (_cameraState)
	{
	case cCreativeMode:
		if (event.code == MOUSE_BUTTON_RIGHT)
		{
			_rotating = true;
		}
		break;
	}
	
}

void Camera::Handle(const InputManager::MouseReleasedEvent & event)
{
	switch (_cameraState)
	{
	case cCreativeMode:
		if (event.code == MOUSE_BUTTON_RIGHT)
		{
			_rotating = false;
		}
		break;
	case cNormal:

		break;
	}
}

void Camera::Handle(const InputManager::MouseMoveEvent & event)
{
	switch (_cameraState)
	{
	case cCreativeMode:
		if (_rotating)
		{
			float deltaTime = APPTIMER->GetTargetTime();

			int32 deltaX = event.current.x - event.old.x;
			int32 deltaY = event.current.y - event.old.y;

			if (deltaX != 0)
			{
				_horizontalAngle += _rotationSpeed * deltaTime * (float)deltaX;
			}

			if (deltaY != 0)
			{
				_verticalAngle += _rotationSpeed * deltaTime * (float)deltaY;
			}

			ClampFloat(_verticalAngle, MIN_VERT_ANGLE, MAX_VERT_ANGLE);

			_entity.GetComponent<TransformComponent>().SetRotateWorld(_verticalAngle * ONE_RAD, _horizontalAngle * ONE_RAD, 0.0f);

		}
		break;
	case cNormal:

		float deltaTime = APPTIMER->GetTargetTime();

		int32 deltaX = event.current.x - event.old.x;
		int32 deltaY = event.current.y - event.old.y;

		if (deltaX != 0)
		{
			_horizontalAngle += _rotationSpeed * deltaTime * (float)deltaX;
		}

		if (deltaY != 0)
		{
			_verticalAngle += _rotationSpeed * deltaTime * (float)deltaY;
		}


		ClampFloat(_verticalAngle, MIN_VERT_ANGLE, MAX_VERT_ANGLE);

		//_entity.GetComponent<TransformComponent>().SetRotateWorld(_verticalAngle * ONE_RAD, _horizontalAngle * ONE_RAD, 0.0f);

		dummyTransform->SetRotateWorld(-_verticalAngle * ONE_RAD, _horizontalAngle * ONE_RAD, 0.0f);

		//_dummyEntity.GetComponent<TransformComponent>().SetRotateWorld(_verticalAngle * ONE_RAD, _horizontalAngle * ONE_RAD, 0.0f);

		/*if (_cameraState == cNormal)
		{
			SetCursorPos(WINSTARTX + (WINSIZEX * 0.5), WINSTARTY + (WINSIZEY * 0.5));
		}*/

		break;
	}
}

void Camera::NormalCameraUpdate(void)
{
	Vector3 dist = dummyTransform->GetWorldPosition() - cameraTransform->GetWorldPosition();
	_curDist = D3DXVec3Length(&dist);

	if (_curDist < PLAYER_TO_CAMERA_DIST)
	{
		cameraTransform->_position.z += 0.1f;

	}
	//else if (_curDist > PLAYER_TO_CAMERA_DIST)
	//{
	//	//cameraTransform->_position.z -= 0.1f;
	//	cameraTransform->_position.z = PLAYER_TO_CAMERA_DIST;
	//}
}