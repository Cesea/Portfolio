#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	//�⺻ ȭ�� ����
	_fov = 60.f * ONE_RAD;

	//�⺻ Near
	_camNear = 0.001f;

	//�⺻ Far
	_camFar = 1000.f;

	_moveSpeed = 1.0f;
	_rotationSpeed = 1.0f;

	EventChannel channel;
	channel.Add<InputSystem::KeyDownEvent, Camera>(*this) ;
}

Camera::~Camera()
{
}

void Camera::UpdateMatrix()
{
	//�̵����� �����ִٸ� ��������
	if (!_toMove.IsZero())
	{
		this->MovePositionSelf(_toMove);
		_toMove = Vector3(0.0f, 0.0f, 0.0f);
	}
	//ȭ���� ���� Projection ��� ������Ʈ
	MatrixPerspectiveFovLH(
		&_matProjection,
		_fov,
		static_cast<float>(WINSIZEX) / static_cast<float>(WINSIZEY),
		_camNear,
		_camFar);

	//����� ī�޶� ������ġ�� ���� ������̴�.
	MatrixInverse(&_matView, NULL, &_matFinal);

	_matViewProjection = _matView * _matProjection;
}

void Camera::UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice)
{	//��� ������Ʈ ���ְ� 
	this->UpdateMatrix();
	//����
	pDevice->SetTransform(D3DTS_VIEW, &_matView);
	pDevice->SetTransform(D3DTS_PROJECTION, &_matProjection);
}

void Camera::ComputeRay(Ray * pOutRay, const Vector2 * screenPos)
{
	//Device �� ����Ʈ�� ��´�.
	D3DVIEWPORT9 viewPort;
	gpDevice->GetViewport(&viewPort);

	//��ũ���� ��ġ ������ ���� 
	float factorX = screenPos->x / viewPort.Width;
	float factorY = (1.0f - (screenPos->y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;

	//��������� ��´�.
	Matrix matProj = GetProjectionMatrix();

	//

	//������ ��ġ�� ȭ�� �����Ϸ��� ������.
	Vector3 direction(
		factorX / matProj._11,
		factorY / matProj._22,
		1.0f);

	//ī�޶� ���� ���
	Matrix matCamWorld = GetFinalMatrix();

	//������ ������ �������� ���� ���͸� ī�޶� ���� ��Ʈ����
	//���� ������ ���´�.
	Vec3TransformNormal(
		&direction, &direction, &matCamWorld);

	//���⺤�� ����
	Vec3Normalize(&direction, &direction);

	//������ ����
	pOutRay->direction = direction;

	//������ �������� ī�޶� ��ġ�� �ȴ�.
	pOutRay->origin = this->GetWorldPosition();
}

bool Camera::GetWorldPosToScreenPos(Vector2 * pScreenPos, const Vector3 * pWorldPos)
{
	//ī�޶��� ViewProjection ����� ��´�.
	Matrix matViewProj = GetViewProjectionMatrix();

	//���� ��ġ�� ������...
	Vector3 pos;
	Vec3TransformCoord(&pos, pWorldPos, &matViewProj);

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
	pScreenPos->x = halfScreenW + (halfScreenW * pos.x);
	pScreenPos->y = halfScreenH - (halfScreenH * pos.y);	//Y ���� ����

	return true;
}

void Camera::Handle(const InputSystem::KeyDownEvent & event)
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
