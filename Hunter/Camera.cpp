#include "stdafx.h"
#include "Camera.h"

constexpr float MAX_VERT_ANGLE = 85.0f;
constexpr float MIN_VERT_ANGLE = -85.0f;

Camera::Camera()
	:_ortho(false)
{
	//�⺻ ȭ�� ����
	_fov = 60.f * ONE_RAD;

	//�⺻ Near
	_camNear = 0.01f;

	//�⺻ Far
#if defined (DEBUG) || defined (_DEBUG)
	_camFar = 1000.0f;
#else 
	_camFar = 1000.0f;
#endif

	_moveSpeed = 1.0f;
	_rotationSpeed = 1.0f;

	_aspect = (float)WINSIZEX / (float)WINSIZEY;
	_orthoSize = 10;


	EventChannel channel;
	channel.Add<InputManager::KeyDownEvent, Camera>(*this);
	channel.Add<InputManager::MousePressedEvent, Camera>(*this);
	channel.Add<InputManager::MouseReleasedEvent, Camera>(*this);
	channel.Add<InputManager::MouseMoveEvent, Camera>(*this);
}

Camera::~Camera()
{
	SAFE_RELEASE(_pRenderTexture);
	SAFE_RELEASE(_pRenderSurface);
}

void Camera::UpdateCamToDevice()
{
	gpDevice->SetTransform(D3DTS_VIEW, &_matView);
	gpDevice->SetTransform(D3DTS_PROJECTION, &_matProjection);
}

void Camera::PreUpdateMatrix()
{
	//�̵����� �����ִٸ� ��������
	if (!_toMove.IsZero())
	{
		_transform.MovePositionSelf(_toMove);
		_toMove = Vector3(0.0f, 0.0f, 0.0f);
	}
}

void Camera::UpdateMatrix()
{
	//ȭ���� ���� Projection ��� ������Ʈ
	if (_ortho == false)
	{
		MatrixPerspectiveFovLH( &_matProjection, _fov, static_cast<float>(WINSIZEX) / static_cast<float>(WINSIZEY),
			_camNear, _camFar);
	}
	else
	{
		MatrixOrthoLH(&_matProjection, _aspect * _orthoSize, _orthoSize, _camNear, _camFar);
	}
	//����� ī�޶� ������ġ�� ���� ������̴�.
	MatrixInverse(&_matView, NULL, &_transform._matFinal);

	_matViewProjection = _matView * _matProjection;

}

void Camera::UpdateFrustum()
{
	_frustum.UpdateFrustum(*this);
}

void Camera::ComputeRay(const Vector2 & screenPos, Ray * pOutRay)
{
	//Device �� ����Ʈ�� ��´�.
	D3DVIEWPORT9 viewPort;
	gpDevice->GetViewport(&viewPort);

	//��ũ���� ��ġ ������ ���� 
	float factorX = screenPos.x / viewPort.Width;
	float factorY = (1.0f - (screenPos.y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;

	//������ ��ġ�� ȭ�� �����Ϸ��� ������.
	Vector3 direction( factorX / _matProjection._11, factorY / _matProjection._22, 1.0f);

	//ī�޶� ���� ���
	Matrix matCamWorld = _transform.GetFinalMatrix();

	//������ ������ �������� ���� ���͸� ī�޶� ���� ��Ʈ����
	//���� ������ ���´�.
	Vec3TransformNormal(
		&direction, &direction, &matCamWorld);

	//���⺤�� ����
	Vec3Normalize(&direction, &direction);

	//������ ����
	pOutRay->direction = direction;

	//������ �������� ī�޶� ��ġ�� �ȴ�.
	pOutRay->origin = _transform.GetWorldPosition();
}

bool Camera::GetWorldPosToScreenPos(const Vector3 & worldPos, Vector2 * pOutScreenPos)
{
	//ī�޶��� ViewProjection ����� ��´�.
	Matrix matViewProj = GetViewProjectionMatrix();

	//���� ��ġ�� ������...
	Vector3 pos;
	Vec3TransformCoord(&pos, &worldPos, &matViewProj);

	//��������
	//				*-------*<-( 1, 1, 1 )
	//			   /|      /|
	//			  *-------* |
	//            | *-----|-*
	//		      |/      |/
	//(-1,-1, 0)->*-------* 

	//0 ~ 1 ���� ���� ������ �̸� ī�޶� �ڿ� �ִٴ¿���
	if (pos.z < 0.0f || pos.z > 1.0f)
		return false;

	//ȭ�� �߾ӿ� �ִٸ� pos xy ���� ������ ����.
	//pos.x == 0  
	//pos.y == 0

	//ȭ�� ���� ������
	float halfScreenW = WINSIZEX * 0.5f;
	float halfScreenH = WINSIZEY * 0.5f;

	//���� ȭ�� ��ġ
	pOutScreenPos->x = halfScreenW + (halfScreenW * pos.x);
	pOutScreenPos->y = halfScreenH - (halfScreenH * pos.y);	//Y ���� ����

	return true;
}

void Camera::ReadyRenderToTexture(int32 width, int32 height)
{
	SAFE_RELEASE(_pRenderTexture);
	SAFE_RELEASE(_pRenderSurface);

	//RenderTarget �� Texture �����
	gpDevice->CreateTexture(
		width,						//Texture ���� �ػ� 
		height,						//Texture ���� �ػ�
		1,							//�Ӹ�ü�� ����
		D3DUSAGE_RENDERTARGET,		//RenderTarget �� Texture�̴�
		D3DFMT_A8R8G8B8,			//TEXTURE ���� RenderTexture ����D3DFMT_A8R8G8B8 ������
		D3DPOOL_DEFAULT,			//RenderTarget �� Texture �� Pool �� Default
		&_pRenderTexture,			//������ Texture �޾ƿ� ������
		NULL
	);

	//Render �� Surface 
	gpDevice->CreateDepthStencilSurface(
		width,					//Texture ���� �ػ� 
		height,					//Texture ���� �ػ�
		D3DFMT_D24S8,				//Deapth �� 24 ��Ʈ Stencil �� 8 ��Ʈ	
		D3DMULTISAMPLE_NONE,		//��Ƽ ���ø� ��Ƽ�˸��ƽ��� �������� �ʴ´�, 
		0,							//��Ƽ ���ø� ����Ƽ�� 0
		TRUE,						//���� ��ü�� ���� �۹������� �������� �ʴ�? ( TRUE �� ���� ���۱�ü �ɶ� ������ ���� ���۳����� ������� �ʴ´� )
		&_pRenderSurface,			//���� ������...
		NULL);
}

void Camera::ReadyShadowTexture(int32 size)
{
	SAFE_RELEASE(_pRenderTexture);
	SAFE_RELEASE(_pRenderSurface);

	//RenderTarget �� Texture �����
	gpDevice->CreateTexture(
		size,						//Texture ���� �ػ� 
		size,						//Texture ���� �ػ�
		1,							//�Ӹ�ü�� ����
		D3DUSAGE_RENDERTARGET,		//RenderTarget �� Texture�̴�
		D3DFMT_R32F,				//��� �÷� ���۸� Red �� �ٻ�� ( 4byte float �Ǽ��� ��� )
		D3DPOOL_DEFAULT,			//RenderTarget �� Texture �� Pool �� Default
		&_pRenderTexture,			//������ Texture �޾ƿ� ������
		NULL
	);

	//Render �� Surface 
	gpDevice->CreateDepthStencilSurface(
		size,					//Texture ���� �ػ� 
		size,					//Texture ���� �ػ�
		D3DFMT_D24S8,				//Deapth �� 24 ��Ʈ Stencil �� 8 ��Ʈ	
		D3DMULTISAMPLE_NONE,		//��Ƽ ���ø� ��Ƽ�˸��ƽ��� �������� �ʴ´�, 
		0,							//��Ƽ ���ø� ����Ƽ�� 0
		TRUE,						//���� ��ü�� ���� �۹������� �������� �ʴ�? ( TRUE �� ���� ���۱�ü �ɶ� ������ ���� ���۳����� ������� �ʴ´� )
		&_pRenderSurface,			//���� ������...
		NULL);
}

void Camera::RenderTextureBegin(uint32 backColor)
{
	//�� ����̽��� Target ������ǥ��� DepthStencil ������ ǥ�������� ���
	gpDevice->GetRenderTarget(0, &_pDeviceTargetSurface);
	gpDevice->GetDepthStencilSurface(&_pDeviceDepthAndStencilSurface);

	//RenderTexture �� Surface �� ��´�.
	LPDIRECT3DSURFACE9 texSurface = nullptr;
	if (SUCCEEDED(this->_pRenderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//Texture ǥ���� Device �� Target ���۷� �����Ѵ�.
		gpDevice->SetRenderTarget(0, texSurface);

		//���õ� Surface ������ �ٷ� �����ִ� ���Ǹ� ������...
		SAFE_RELEASE(texSurface);
	}

	//Depth ���ۿ� Stencil ������ Surface �� m_pRenderSurface ����
	gpDevice->SetDepthStencilSurface(_pRenderSurface);

	//����̽� ���� Ŭ���� ( ��� ������ Setting �� Textuer �� Surface �� Ŭ���� �ȴ�  )
	gpDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, backColor, 1.0f, 0);
}

void Camera::RenderTextureEnd()
{
	//Render Texture �� �׸� �۾��� �������� �ٽ� ���󺹱��ϴ� ����....
	gpDevice->SetRenderTarget(0, _pDeviceTargetSurface);
	gpDevice->SetDepthStencilSurface(_pDeviceDepthAndStencilSurface);

	//���õ� Surface ������ �ٷ� �����ִ� ���Ǹ� ������...
	SAFE_RELEASE(_pDeviceTargetSurface);
	SAFE_RELEASE(_pDeviceDepthAndStencilSurface);
}

LPDIRECT3DTEXTURE9 Camera::GetRenderTexture()
{
	return _pRenderTexture;
}

void Camera::Handle(const InputManager::KeyDownEvent & event)
{
	Vector3 diff = Vector3(0.0f, 0.0f, 0.0f);
	float deltaTime = APPTIMER->GetTargetTime();

	//move forward
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
}

void Camera::Handle(const InputManager::MousePressedEvent & event)
{
	if (event.code == MOUSE_BUTTON_RIGHT)
	{
		_rotating = true;
	}
}

void Camera::Handle(const InputManager::MouseReleasedEvent & event)
{
	if (event.code == MOUSE_BUTTON_RIGHT)
	{
		_rotating = false;
	}
}

void Camera::Handle(const InputManager::MouseMoveEvent & event)
{
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

		_transform.SetRotateWorld(_verticalAngle * ONE_RAD, _horizontalAngle * ONE_RAD, 0.0f);
	}
}
