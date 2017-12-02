#include "stdafx.h"
#include "Camera.h"

constexpr float MAX_VERT_ANGLE = 85.0f;
constexpr float MIN_VERT_ANGLE = -85.0f;

//constexpr float CAMERA_LOOKDOWN_ANGLE = D3DX_PI /

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

	_cameraState = CAMERASTATE_CREATE;

	_offsetForwardMult = -6.0f;
	_offsetUpMult = 4.0f;

	_ortho = false;
	_aspect = (float)(WINSIZEX) / (float)(WINSIZEY);
	_orthoSize = 10;
}

Camera::~Camera()
{
	COM_RELEASE( _pRenderTexture );
	COM_RELEASE( _pRenderSurface );
}

void Camera::CreateFromWorld(World & world)
{
	_entity = world.CreateEntity();
	_entity.AddComponent<TransformComponent>();
	
	_entity.Activate();
}

	//case cNormal:
	//	POINT pt;
	//	GetCursorPos(&pt);
	//	if (_curDist >= PLAYER_TO_CAMERA_DIST)
	//	{
	//		if (move(pt) == true)
	//		{
	//			float screenCenterX = 892;
	//			float screenCenterY = 519;
	//			float deltaTime = APPTIMER->GetTargetTime();
	//			POINT mousePos = GetMousePos();
	//			mousePos.x = mousePos.x + WINSTARTX;
	//			mousePos.y = mousePos.y + WINSTARTY;
	//			//이동량 ( 중앙에서 멀어진 량 )
	//			int deltaX = mousePos.x - screenCenterX;
	//			int deltaY = mousePos.y - screenCenterY;
	//			if (deltaX != 0)
	//			{
	//				_horizontalAngle += _rotationSpeed * deltaTime * (float)deltaX;
	//			}
	//			if (deltaY != 0)
	//			{
	//				_verticalAngle += _rotationSpeed * deltaTime * (float)deltaY;
	//			}
	//			ClampFloat(_verticalAngle, MIN_VERT_ANGLE, MAX_VERT_ANGLE);
	//			SetCursorPos(CLIENTCENTERX, CLIENTCENTERY);
	//		}
	//	}		break;
	//}
	//

void Camera::MoveAndRotate(float deltaTime, const InputManager & input)
{
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();
	TransformComponent &refTargetTransform =
		_pTargetObject->GetEntity().GetComponent<TransformComponent>();

	Vector3 forward = refTransform.GetForward();
	Vector3 right = refTransform.GetRight();
	Vector3 up = refTransform.GetUp();

	//State 변경
	if (input.keyboard.IsPressed('1'))
	{
		_cameraState = CAMERASTATE_CREATE;
		refTransform.LookDirection(Vector3(0.0f, 0.0f, 1.0f));
		//ShowCursor(true);
	}
	else if (input.keyboard.IsPressed('2'))
	{
		Assert(_pTargetObject);
		_cameraState = CAMERASTATE_INGAME;

		//ShowCursor(false);
	}

	//Mouse Move//////////////////////////////////////////////
	switch (_cameraState)
	{
	case CAMERASTATE_CREATE:
	{
		if (_rotating)
		{
			int32 deltaX = input.mouse.GetMouseDelta().x;
			int32 deltaY = input.mouse.GetMouseDelta().y;

			if (deltaX != 0)
			{
				_horizontalAngle += _rotationSpeed * deltaTime * (float)deltaX;
			}

			if (deltaY != 0)
			{
				_verticalAngle += _rotationSpeed * deltaTime * (float)deltaY;
			}

			ClampFloat(_verticalAngle, MIN_VERT_ANGLE, MAX_VERT_ANGLE);

			refTransform.SetRotateWorld(_verticalAngle * ONE_RAD, _horizontalAngle * ONE_RAD, 0.0f);
		}
	}break;

	case CAMERASTATE_INGAME:
	{
		POINT mousePoint = input.mouse.GetCurrentPoint();

		mousePoint.x += WINSTARTX;
		mousePoint.y += WINSTARTY;

		int32 deltaX = mousePoint.x - (CLIENTCENTERX - 1);
		//int32 deltaY = mousePoint.y - CLIENTCENTERY;

		if (deltaX != 0)
		{
			_horizontalAngle += _rotationSpeed * deltaTime * (float)deltaX;
		}

		//if (deltaY != 0)
		//{
		//	_verticalAngle += _rotationSpeed * deltaTime * (float)deltaY;
		//}

		//ClampFloat(_verticalAngle, MIN_VERT_ANGLE, MAX_VERT_ANGLE);
		refTransform.SetRotateWorld(10 *  ONE_RAD, _horizontalAngle * ONE_RAD, 0.0f);

		Vector3 planerForward = _entity.GetComponent<TransformComponent>().GetForward();
		planerForward.y = 0.0f;
		refTargetTransform.LookDirection(planerForward);

		SetCursorPos(CLIENTCENTERX, CLIENTCENTERY);
	} break;
	}


	Vector3 diff = Vector3(0.0f, 0.0f, 0.0f);
	////move forward
	switch (_cameraState)
	{
	case CAMERASTATE_CREATE:
	{
		if (input.keyboard.IsDown('W')) { diff += forward * _moveSpeed * deltaTime; }
		else if (input.keyboard.IsDown('S')) { diff -= forward * _moveSpeed * deltaTime; }

		if (input.keyboard.IsDown('A')) { diff -= right * _moveSpeed * deltaTime; }
		else if (input.keyboard.IsDown('D')) { diff += right * _moveSpeed * deltaTime; }

		if (input.keyboard.IsDown('Q')) { diff += up * _moveSpeed * deltaTime; }
		else if (input.keyboard.IsDown('E')) { diff -= up * _moveSpeed * deltaTime; }

		if (!FloatZero(diff.IsZero()))
		{
			Vec3Normalize(&diff, &diff);
		}
		refTransform.MovePositionWorld(diff);
	} break;

	case CAMERASTATE_INGAME:
	{
		Vector3 offsetVector;
		offsetVector += refTargetTransform.GetForward() * _offsetForwardMult;
		offsetVector += refTargetTransform.GetUp() * _offsetUpMult;

		refTransform.SetWorldPosition(refTargetTransform.GetWorldPosition() + offsetVector);
	} break;
	}

	//Mouse Pressed////////////////////////////////////////////
	switch (_cameraState)
	{
	case CAMERASTATE_CREATE:
	{
		if (input.mouse.IsPressed(MOUSE_BUTTON_RIGHT))
		{
			_rotating = true;
		}
		if (input.mouse.IsReleased(MOUSE_BUTTON_RIGHT))
		{
			_rotating = false;
		}
	} break;
	}
}

void Camera::UpdateMatrix()
{
	//if (_cameraState == cNormal)
	//{
	//	Vector3 tPos = targetTransform->GetWorldPosition();
	//	tPos.y = targetTransform->GetWorldPosition().y + 1.5f;
	//	
	//	dummyTransform->SetWorldPosition(
	//		targetTransform->GetWorldPosition().x, 
	//		targetTransform->GetWorldPosition().y + 2.5, 
	//		targetTransform->GetWorldPosition().z);
	//	if (_curDist < PLAYER_TO_CAMERA_DIST)
	//	{
	//		Vector3 dir = tPos - cameraTransform->GetWorldPosition();
	//		Vec3Normalize(&dir, &dir);
	//		cameraTransform->SetForward(dir);
	//		NormalCameraUpdate();
	//	}
	//}

	if (_ortho)
	{
		MatrixOrthoLH(&_matProjection, _fov, _aspect, _camNear, _camFar);
	}
	else
	{
		//화각에 의한 Projection 행렬 업데이트
		MatrixPerspectiveFovLH( &_matProjection, _fov,
			static_cast<float>(WINSIZEX) / static_cast<float>(WINSIZEY), _camNear, _camFar);
	}

	//뷰행렬 카메라 월드위치에 대한 역행렬이다.
	MatrixInverse(&_matView, nullptr, &_entity.GetComponent<TransformComponent>()._matFinal);

	_matViewProjection = _matView * _matProjection;
}

void Camera::UpdateCamToDevice()
{
	gpDevice->SetTransform(D3DTS_VIEW, &_matView);
	gpDevice->SetTransform(D3DTS_PROJECTION, &_matProjection);
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

void Camera::ReadyRenderToTexture(int32 width, int32 height)
{
	COM_RELEASE(_pRenderTexture);
	COM_RELEASE(_pRenderSurface );

	//RenderTarget 빈 Texture 만들기
	gpDevice->CreateTexture(
		width,						//Texture 가로 해상도 
		height,						//Texture 세로 해상도
		1,							//밉맵체인 레벨
		D3DUSAGE_RENDERTARGET,		//RenderTarget 용 Texture이다
		D3DFMT_A8R8G8B8,			//TEXTURE 포맷 RenderTexture 용은D3DFMT_A8R8G8B8 로하자
		D3DPOOL_DEFAULT,			//RenderTarget 용 Texture 는 Pool 을 Default
		&_pRenderTexture,			//생성된 Texture 받아올 포인터
		NULL						
		);

	//Render 할 Surface 
	gpDevice->CreateDepthStencilSurface(
		width,					//Texture 가로 해상도 
		height,					//Texture 세로 해상도
		D3DFMT_D24S8,				//Deapth 는 24 비트 Stencil 은 8 비트	
		D3DMULTISAMPLE_NONE,		//멀티 샘플링 안티알리아싱은 존재하지 않는다, 
		0,							//멀티 샘플링 퀄리티는 0
		TRUE,						//버퍼 교체시 이전 퍼버내용을 유지하지 않니? ( TRUE 로 쓰면 버퍼교체 될때 이전에 써진 버퍼내용을 기억하지 않는다 )
		&_pRenderSurface,			//얻어올 포인터...
		NULL);
}

void Camera::ReadyShadowTexture(int32 size)
{
	COM_RELEASE(_pRenderTexture);
	COM_RELEASE(_pRenderSurface );

	HRESULT result = 0;
	//RenderTarget 빈 Texture 만들기
	result = gpDevice->CreateTexture(
		size,						//Texture 가로 해상도 
		size,						//Texture 세로 해상도
		1,							//밉맵체인 레벨
		D3DUSAGE_RENDERTARGET,		//RenderTarget 용 Texture이다
		D3DFMT_R32F,				//모든 컬러 버퍼를 Red 로 다사용 ( 4byte float 실수로 사용 )
		D3DPOOL_DEFAULT,			//RenderTarget 용 Texture 는 Pool 을 Default
		&_pRenderTexture,			//생성된 Texture 받아올 포인터
		NULL						
		);

	//Render 할 Surface 
	result = gpDevice->CreateDepthStencilSurface(
		size,					//Texture 가로 해상도 
		size,					//Texture 세로 해상도
		D3DFMT_D24S8,				//Deapth 는 24 비트 Stencil 은 8 비트	
		D3DMULTISAMPLE_NONE,		//멀티 샘플링 안티알리아싱은 존재하지 않는다, 
		0,							//멀티 샘플링 퀄리티는 0
		TRUE,						//버퍼 교체시 이전 퍼버내용을 유지하지 않니? ( TRUE 로 쓰면 버퍼교체 될때 이전에 써진 버퍼내용을 기억하지 않는다 )
		&_pRenderSurface,			//얻어올 포인터...
		NULL);
}

void Camera::RenderTextureBegin(uint32 backColor)
{
	HRESULT result = 0;
	//현 디바이스의 Target 버퍼의표면과 DepthStencil 버퍼의 표면정보를 기억
	result = gpDevice->GetRenderTarget( 0, &_pDeviceTargetSurface );
	result = gpDevice->GetDepthStencilSurface( &_pDeviceDepthAndStencilSurface );

	//RenderTexture 의 Surface 를 얻는다.
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if( SUCCEEDED( this->_pRenderTexture->GetSurfaceLevel( 0, &texSurface ) ) )
	{
		//Texture 표면을 Device 의 Target 버퍼로 셋팅한다.
		result = gpDevice->SetRenderTarget( 0, texSurface );

		//셋팅된 Surface 정보는 바로 날려주는 예의를 갖추자...
		SAFE_RELEASE( texSurface );
	}

	//Depth 버퍼와 Stencil 버퍼의 Surface 로 m_pRenderSurface 셋팅
	result = gpDevice->SetDepthStencilSurface( _pRenderSurface );

	//디바이스 버퍼 클리어 ( 사실 위에서 Setting 된 Textuer 랑 Surface 가 클리어 된다  )
	result = gpDevice->Clear( 0, NULL, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		backColor, 
		1.0f, 
		0 );
}

void Camera::RenderTextureEnd()
{
	//Render Texture 에 그릴 작업이 끝났으면 다시 원상복귀하는 센스....
	HRESULT result = 0;
	result = gpDevice->SetRenderTarget( 0, _pDeviceTargetSurface );
	result = gpDevice->SetDepthStencilSurface( _pDeviceDepthAndStencilSurface );

	//셋팅된 Surface 정보는 바로 날려주는 예의를 갖추자...
	COM_RELEASE( _pDeviceTargetSurface );
	COM_RELEASE( _pDeviceDepthAndStencilSurface );	
}

LPDIRECT3DTEXTURE9 Camera::GetRenderTexture()
{
	return _pRenderTexture;
}

void Camera::NormalCameraUpdate(void)
{
	//Vector3 dist = dummyTransform->GetWorldPosition() - cameraTransform->GetWorldPosition();
	//_curDist = D3DXVec3Length(&dist);

}
//SetCursorPos(WINSTARTX + (WINSIZEX * 0.5), WINSTARTY + (WINSIZEY * 0.5));
//if (_curDist < PLAYER_TO_CAMERA_DIST)
//{
//	cameraTransform->_position.z += 0.1f;
//}
//else if (_curDist > PLAYER_TO_CAMERA_DIST)
//{
//	//cameraTransform->_position.z -= 0.1f;
//	cameraTransform->_position.z = PLAYER_TO_CAMERA_DIST;
//}
//}
//bool Camera::move(POINT pt)
//{
//	if (pt.x != tempPt.x || pt.y != tempPt.y)
//	{
//		tempPt.x = pt.x;
//		tempPt.y = pt.y;
//		return true;
//	}
//	return false;
//}