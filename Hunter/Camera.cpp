#include "stdafx.h"
#include "Camera.h"

constexpr float MAX_VERT_ANGLE = 85.0f;
constexpr float MIN_VERT_ANGLE = -85.0f;

Camera::Camera()
{
	//�⺻ ȭ�� ����
	_fov = 60.f * ONE_RAD;

	//�⺻ Near
	_camNear = 0.01f;

	//�⺻ Far
#if defined (DEBUG) || defined (_DEBUG)
	_camFar = 100.0f;
#else 
	_camFar = 200.0f;
#endif

	_moveSpeed = 1.0f;
	_rotationSpeed = 1.0f;

	EventChannel channel;
	channel.Add<InputManager::KeyDownEvent, Camera>(*this);
	channel.Add<InputManager::MousePressedEvent, Camera>(*this);
	channel.Add<InputManager::MouseReleasedEvent, Camera>(*this);
	channel.Add<InputManager::MouseMoveEvent, Camera>(*this);
}

Camera::~Camera()
{
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
	MatrixPerspectiveFovLH(
		&_matProjection,
		_fov,
		static_cast<float>(WINSIZEX) / static_cast<float>(WINSIZEY),
		_camNear,
		_camFar);

	//����� ī�޶� ������ġ�� ���� ������̴�.
	MatrixInverse(&_matView, NULL, &_transform._matFinal);

	_matViewProjection = _matView * _matProjection;

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
	if (event.code == MOUSE_BUTTON_LEFT)
	{
		_rotating = true;
	}
}

void Camera::Handle(const InputManager::MouseReleasedEvent & event)
{
	if (event.code == MOUSE_BUTTON_LEFT)
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
