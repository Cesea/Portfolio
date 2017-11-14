#include "stdafx.h"
#include "TransformComponent.h"


DEFINE_META(TransformComponent)
{
	ADD_MEMBER(_right);
	ADD_MEMBER(_up);
	ADD_MEMBER(_forward);
	ADD_MEMBER(_position);
	ADD_MEMBER(_dummyPosition);
	ADD_MEMBER(_scale);
	ADD_MEMBER(_matFinal);
	ADD_MEMBER(_matLocal);
	ADD_MEMBER(_pParent);
	ADD_MEMBER(_pFirstChild);
	ADD_MEMBER(_pNextSibling);
	ADD_MEMBER(_transformDirty);
};

TransformComponent::TransformComponent()
{

	this->_pParent = NULL;
	this->_pFirstChild = NULL;
	this->_pNextSibling = NULL;

	this->_transformDirty = false;

	//���� ����
	this->Reset();

	MatrixIdentity(&_matFinal);
	MatrixIdentity(&_matLocal);
}


TransformComponent::~TransformComponent()
{
}

//TransformComponent ������ �ʱ�ȭ
void TransformComponent::Reset(int resetFlag /*= -1*/)
{
	if (resetFlag & RESET_POSITION)
	{
		//��ġ�� 0, 0, 0
		this->_position.x = 0;
		this->_position.y = 0;
		this->_position.z = 0;
	}

	if (resetFlag & RESET_ROTATION)
	{
		//ȸ�������� 
		//this->_axis[0] = Vector3( 1, 0, 0 );
		//this->_axis[1] = Vector3( 0, 1, 0 );
		//this->_axis[2] = Vector3( 0, 0, 1 );

		this->_right = Vector3(1, 0, 0);
		this->_up = Vector3(0, 1, 0);
		this->_forward = Vector3(0, 0, 1);
	}

	if (resetFlag & RESET_SCALE)
	{
		//�������� �ʱ�ȭ ��
		this->_scale = Vector3(1, 1, 1);
	}

	_transformDirty = true;
}

//Ư�� Child �� ���ڽ����� ���δ�.
void TransformComponent::AddChild(TransformComponent* pNewChild)
{
	//�̹� �������� ���ʿ� ����
	if (pNewChild->_pParent == this)
		return;

	//���̻��� �������� ���÷��� �θ�� ���� �����...
	pNewChild->ReleaseParent();

	//�θ��� ������� ��ǥ������ �����ϱ����� 
	//�θ��� final ������� ���Ѵ�.
	Matrix matInvFinal;
	MatrixInverse(&matInvFinal, NULL, &this->_matFinal);

	//�ڽ��� Position �� Axis �� Scale ����
	Vec3TransformCoord(&pNewChild->_position, &pNewChild->_position, &matInvFinal);

	//��3�� ��ȯ�ϰ� 
	for (int32 i = 0; i < 3; i++) 
	{
		Vec3TransformNormal(pNewChild->_axis + i, pNewChild->_axis + i, &matInvFinal);
	}

	//3�࿡ ���� ���̰��� ��´�.
	pNewChild->_scale.x = Vec3Length(&pNewChild->_right);
	pNewChild->_scale.y = Vec3Length(&pNewChild->_up);
	pNewChild->_scale.z = Vec3Length(&pNewChild->_forward);

	//����ȭ
	Vec3Normalize(&pNewChild->_right, &pNewChild->_right);
	Vec3Normalize(&pNewChild->_up, &pNewChild->_up);
	Vec3Normalize(&pNewChild->_forward, &pNewChild->_forward);

	//���ο� ���� �θ�� ���� �ȴ�.
	pNewChild->_pParent = this;

	//���� �ڽĳ� ������
	TransformComponent* pChild = this->_pFirstChild;

	//�ڽ��� ���� ������ ���ų����̶��...
	if (nullptr == pChild ) 
	{
		//�Ƚ��ϰ� �߰�
		this->_pFirstChild = pNewChild;
		pNewChild->_pParent = this;
	}

	//�밡���� ����.
	else 
	{
		while (nullptr != pChild) 
		{
			//���� �� �ڸ��� ã�Ҵٸ�...
			if (nullptr == pChild->_pNextSibling)
			{
				pChild->_pNextSibling = pNewChild;
				pNewChild->_pParent = this;
				break;
			}

			pChild = pChild->_pNextSibling;
		}
	}

	//������Ʈ
	_transformDirty = true;
}

//Ư�� TransformComponent �� �ٴ´�.
void TransformComponent::AttachTo(TransformComponent* pNewParent)
{
	pNewParent->AddChild(this);
}

//�θ�� �ȳ�
void TransformComponent::ReleaseParent()
{
	//�θ� ����?
	if (nullptr == _pParent)
	{
		return;
	}

	//�θ�� ���� ���� ���� �θ���� �ڽĿ��� �����...
	TransformComponent* pChild = _pParent->_pFirstChild;

	//���� �θ��� ù°�ڽ��̴�?
	if (this == pChild ) 
	{
		//������ �ڽ��� ù��° �ڽ��� �ȴ�.
		this->_pParent->_pFirstChild = this->_pNextSibling;

		//�������� ���� ���´�.
		this->_pNextSibling = NULL;
	}

	else
	{
		while (nullptr != pChild) 
		{
			//���� �ڽ��� ������ ����?
			if (this == pChild->_pNextSibling) 
			{
				pChild->_pNextSibling = this->_pNextSibling;

				//�������� ���� ���´�.
				this->_pNextSibling = NULL;

				break;
			}

			//���� �ڽĺ���.
			pChild = pChild->_pNextSibling;
		}
	}

	//�θ�� ���� �����...
	this->_pParent = nullptr;

	//�ڽ��� ���� ���� ��ġ�� ���� ������ �ʿ��ϴ�.
	//��¥���� ��ġ�� matFinal �� �� ������ �ִ�.

	//���� ��ġ ����
	this->_position.x = this->_matFinal._41;
	this->_position.y = this->_matFinal._42;
	this->_position.z = this->_matFinal._43;

	//3�� ���´�.
	Vector3 forwardScaled(this->_matFinal._31, this->_matFinal._32, this->_matFinal._33);
	Vector3 upScaled(this->_matFinal._21, this->_matFinal._22, this->_matFinal._23);
	Vector3 rightScaled(this->_matFinal._11, this->_matFinal._12, this->_matFinal._13);

	//3�࿡�� ������ ����
	float scaleX = Vec3Length(&rightScaled);
	float scaleY = Vec3Length(&upScaled);
	float scaleZ = Vec3Length(&forwardScaled);

	//����ȭ
	Vector3 forwardUnit;
	Vector3 upUnit;
	Vector3 rightUnit;
	Vec3Normalize(&rightUnit, &rightScaled);
	Vec3Normalize(&upUnit, &upScaled);
	Vec3Normalize(&forwardUnit, &forwardScaled);

	//����ȭ�� 3�� ����
	this->_forward = forwardUnit;
	this->_right = rightUnit;
	this->_up = upUnit;

	//������ ����
	this->_scale.x = scaleX;
	this->_scale.y = scaleY;
	this->_scale.z = scaleZ;

	//������ ������ �Ǿ���...

	//�θ�� ����� ������� Update �ѹ� ��
	_transformDirty = true;
}


//��ġ�� ���� ��ǥ��� �����Ѵ�. 
void TransformComponent::SetWorldPosition(float x, float y, float z)
{
	Vector3 pos(x, y, z);

	//�θ� �ִٸ� �θ��� �������� ��ġ�� �ٲ��...
	if (nullptr != this->_pParent )
	{
		//�θ��� ���� ����� �����
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		//�� ���࿡ Pos ����
		Vec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	_transformDirty = true;
}

void TransformComponent::SetWorldPosition(Vector3 pos)
{
	//�θ� �ִٸ� �θ��� �������� ��ġ�� �ٲ��...
	if (this->_pParent != NULL)
	{
		//�θ��� ���� ����� �����
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		//�� ���࿡ Pos ����
		Vec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	_transformDirty = true;
}

//��ġ�� ���� ��ǥ��� �����Ѵ�.  ( �θ� �ִ� ��� ������ �������� ��ġ )
void TransformComponent::SetLocalPosition(float x, float y, float z)
{
	this->_position.x = x;
	this->_position.y = y;
	this->_position.z = z;

	_transformDirty = true;
}

void TransformComponent::SetLocalPosition(Vector3 pos)
{
	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	_transformDirty = true;
}

//�ڽ��� ��������� �̵� ��Ų��.
void TransformComponent::MovePositionSelf(float dx, float dy, float dz)
{
	//�̵� ����
	Vector3 move(0, 0, 0);

	//�ڽ��� �̵� �� ��´�.
	Vector3 moveAxis[3];
	this->GetUnitAxies(moveAxis);
	move += moveAxis[0] * dx;
	move += moveAxis[1] * dy;
	move += moveAxis[2] * dz;

	//���� �̵�
	Vector3 nowWorldPos = this->GetWorldPosition();

	//�Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->SetWorldPosition(nowWorldPos + move);
}

void TransformComponent::MovePositionSelf(Vector3 delta)
{
	//�̵� ����
	Vector3 move(0, 0, 0);

	//�ڽ��� �̵� �� ��´�.
	Vector3 moveAxis[3];
	this->GetUnitAxies(moveAxis);
	move += moveAxis[0] * delta.x;
	move += moveAxis[1] * delta.y;
	move += moveAxis[2] * delta.z;

	//���� �̵�
	Vector3 nowWorldPos = this->GetWorldPosition();

	//�Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->SetWorldPosition(nowWorldPos + move);
}

//���� �������� �̵� ��Ų��.
void TransformComponent::MovePositionWorld(float dx, float dy, float dz)
{
	//�̵� ����
	Vector3 move(dx, dy, dz);

	//���� �̵�
	Vector3 nowWorldPos = this->GetWorldPosition();

	//�Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->SetWorldPosition(nowWorldPos + move);
}

void TransformComponent::MovePositionWorld(Vector3 delta)
{
	//���� �̵�
	Vector3 nowWorldPos = this->GetWorldPosition();

	//�Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->SetWorldPosition(nowWorldPos + delta);
}


//�θ� �ִ� ��� ���� �������� �̵� ��Ų��.
void TransformComponent::MovePositionLocal(float dx, float dy, float dz)
{
	this->_position.x += dx;
	this->_position.y += dy;
	this->_position.z += dz;

	_transformDirty = true;
}

void TransformComponent::MovePositionLocal(Vector3 delta)
{
	this->_position += delta;

	_transformDirty = true;
}


//������ ���� 
void TransformComponent::SetScale(float x, float y, float z)
{
	this->_scale.x = x;
	this->_scale.y = y;
	this->_scale.z = z;

	_transformDirty = true;
}

void TransformComponent::SetScale(Vector3 scale)
{
	this->_scale = scale;

	_transformDirty = true;
}

//�����ϸ�
void TransformComponent::Scaling(float dx, float dy, float dz)
{
	this->_scale.x += dx;
	this->_scale.y += dy;
	this->_scale.z += dz;

	
	_transformDirty = true;
}

void TransformComponent::Scaling(Vector3 deltaScale)
{
	this->_scale += deltaScale;

	_transformDirty = true;
}

//���� �������� ȸ�� 
void TransformComponent::RotateWorld(float angleX, float angleY, float angleZ)
{
	//�θ� �ִ� ���
	if (this->_pParent)
	{
		//��¥�ο��� ��
		Vector3 worldAxis[3];
		this->GetUnitAxies(worldAxis);

		//�� �࿡ ���� ȸ�� ���
		Matrix matRotateX;
		MatrixRotationX(&matRotateX, angleX);

		Matrix matRotateY;
		MatrixRotationY(&matRotateY, angleY);

		Matrix matRotateZ;
		MatrixRotationZ(&matRotateZ, angleZ);

		//���� ȸ�� ���
		Matrix matRotate = matRotateY * matRotateX * matRotateZ;

		//�θ��� ����ķ� �ٽ� ȸ��
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		//���� ȸ�� ��� ��� ȸ�� ��Ų��.
		for (int i = 0; i < 3; i++)
			Vec3TransformNormal(this->_axis + i, worldAxis + i, &matRotate);

	_transformDirty = true;
	}


	//�θ� ���� ���
	else
	{
		RotateLocal(angleX, angleY, angleZ);
	}


}

void TransformComponent::RotateWorld(const Vector3 &angle)
{
	//�θ� �ִ� ���
	if (this->_pParent)
	{
		//��¥�ο��� ��
		Vector3 worldAxis[3];
		this->GetUnitAxies(worldAxis);

		//�� �࿡ ���� ȸ�� ���
		Matrix matRotateX;
		MatrixRotationX(&matRotateX, angle.x);

		Matrix matRotateY;
		MatrixRotationY(&matRotateY, angle.y);

		Matrix matRotateZ;
		MatrixRotationZ(&matRotateZ, angle.z);


		//���� ȸ�� ��ĸ��鶧 �̸� ����ļ����� ���س����� 1 ���� ȸ����Ű�� �ȴ�.

		//���� ȸ�� ���
		Matrix matRotate = matRotateY * matRotateX * matRotateZ;

		//�θ��� ����ķ� �ٽ� ȸ��
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		//���� ȸ�� ��� ��� ȸ�� ��Ų��.
		for (int i = 0; i < 3; i++)
			Vec3TransformNormal(this->_axis + i, worldAxis + i, &matRotate);


	
	_transformDirty = true;
	}

	//�θ� ���� ���
	else
	{
		RotateLocal(angle.x, angle.y, angle.z);
	}
}

//�ڽ��� ��������� ȸ��
void TransformComponent::RotateSelf(float angleX, float angleY, float angleZ)
{
	//�� �࿡ ���� ȸ�� ���
	Matrix matRotateX;
	MatrixRotationAxis(&matRotateX, &this->GetRight(), angleX);

	Matrix matRotateY;
	MatrixRotationAxis(&matRotateY, &this->GetUp(), angleY);

	Matrix matRotateZ;
	MatrixRotationAxis(&matRotateZ, &this->GetForward(), angleZ);


	//���� ȸ�� ���
	Matrix matRotate = matRotateY * matRotateX * matRotateZ;


	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);


	
	_transformDirty = true;
}

void TransformComponent::RotateSelf(const Vector3 &angle)
{
	//�� �࿡ ���� ȸ�� ���
	Matrix matRotateX;
	MatrixRotationAxis(&matRotateX, &this->GetRight(), angle.x);

	Matrix matRotateY;
	MatrixRotationAxis(&matRotateY, &this->GetUp(), angle.y);

	Matrix matRotateZ;
	MatrixRotationAxis(&matRotateZ, &this->GetForward(), angle.z);


	//���� ȸ�� ���
	Matrix matRotate = matRotateY * matRotateX * matRotateZ;


	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);


	
	_transformDirty = true;
}

//�θ� �ִ� ��� �θ� ������ ��������� ȸ��
void TransformComponent::RotateLocal(float angleX, float angleY, float angleZ)
{
	//�� �࿡ ���� ȸ�� ���
	Matrix matRotateX;
	MatrixRotationX(&matRotateX, angleX);

	Matrix matRotateY;
	MatrixRotationY(&matRotateY, angleY);

	Matrix matRotateZ;
	MatrixRotationZ(&matRotateZ, angleZ);

	//���� ȸ�� ���
	Matrix matRotate = matRotateY * matRotateX * matRotateZ;


	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);


	
	_transformDirty = true;
}

void TransformComponent::RotateLocal(const Vector3 &angle)
{
	//�� �࿡ ���� ȸ�� ���
	Matrix matRotateX;
	MatrixRotationX(&matRotateX, angle.x);

	Matrix matRotateY;
	MatrixRotationY(&matRotateY, angle.y);

	Matrix matRotateZ;
	MatrixRotationZ(&matRotateZ, angle.z);

	//���� ȸ�� ���
	Matrix matRotate = matRotateY * matRotateX * matRotateZ;

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	
	_transformDirty = true;
}


//Ư�� ������ �ٶ󺸰� ȸ���ض�.
void TransformComponent::LookDirection(const Vector3 &dir, const Vector3 &_up /*= Vector3(0, 1, 0)*/)
{
	//���� ����
	Vector3 newForward = dir;

	//�����ʺ��� ( �Ű������� ���� Up �� ������ ���� )
	Vector3 newRight;
	Vec3Cross(&newRight, &_up, &newForward);
	Vec3Normalize(&newRight, &newRight);

	//�� 
	Vector3 newUp;
	Vec3Cross(&newUp, &newForward, &newRight);
	Vec3Normalize(&newUp, &newUp);

	//���� �θ� �ִٸ�...
	if (this->_pParent)
	{
		//���ο� �� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		Vec3TransformNormal(&this->_forward, &newForward, &matInvParentFinal);
		Vec3TransformNormal(&this->_right, &newRight, &matInvParentFinal);
		Vec3TransformNormal(&this->_up, &newUp, &matInvParentFinal);
	}

	else
	{
		this->_forward = newForward;
		this->_right = newRight;
		this->_up = newUp;
	}

	
	_transformDirty = true;
}

//Ư�� ������ �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
void TransformComponent::LookDirection(const Vector3 &dir, float angle)
{
	//��¥�ο��� ��
	Vector3 worldAxis[3];
	this->GetUnitAxies(worldAxis);

	//���� ���Ϳ� �ٶ� ������ �������� ����...
	float distRadian = acos(
		ClampMinusOnePlusOne(Vec3Dot(worldAxis + AXIS_Z, &dir)));

	//�������� ���� ���ٸ� ������...
	if (FloatZero(distRadian)) return;

	//���� ( ������� Ÿ�ٱ����� ������ ���� )
	Vector3 cross;
	Vec3Cross(&cross, worldAxis + AXIS_Z, &dir);
	Vec3Normalize(&cross, &cross);

	//���������� ������ŭ ȸ�� ��Ű�� ���
	Matrix matRotate;
	MatrixRotationAxis(&matRotate, &cross, min(angle, distRadian));

	//���� �θ� �ִٸ�...
	if (this->_pParent)
	{
		//ȸ�� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//����
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(this->_axis + i, worldAxis + i, &matRotate);

	
	_transformDirty = true;
}

//Ư����ġ�� �ٶ󺸰� ȸ���ض�.
void TransformComponent::LookPosition(const Vector3 &pos, const Vector3 &_up /*= Vector3(0, 1, 0)*/)
{
	//��ġ�� ���� ���⺤�͸� ��´�.
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 dir = pos - worldPos;

	Vec3Normalize(&dir, &dir);
	this->LookDirection(dir, _up);
}

//Ư����ġ��  �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
void TransformComponent::LookPosition(const Vector3 &pos, float angle)
{
	//��ġ�� ���� ���⺤�͸� ��´�.
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 dir = pos - worldPos;

	Vec3Normalize(&dir, &dir);
	this->LookDirection(dir, angle);
}

//������� �̿��� Ư�� ȸ�������� ȸ������ ������....
void TransformComponent::SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ)
{
	//����� �غ�
	Quaternion quatRot;
	QuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//������� ���� ����غ�
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//������� ���� ȸ�������� ȸ������� ���������.

															//���� �θ� �ִٸ�...
	if (this->_pParent)
	{
		//ȸ�� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//�ฮ��
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

void TransformComponent::SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ)
{
	//����� �غ�
	Quaternion quatRot;
	QuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//������� ���� ����غ�
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//������� ���� ȸ�������� ȸ������� ���������.

															//�ฮ��
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

// ȸ�� ����� �־��ָ� �� ȸ�� ��Ĵ�� ȸ���Ѵ�.
void TransformComponent::SetRotateWorld(const Matrix& matWorldRotate)
{
	Matrix matRotate = matWorldRotate;

	//���� �θ� �ִٸ�...
	if (this->_pParent)
	{
		//ȸ�� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//�ฮ��
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

void TransformComponent::SetRotateLocal(const Matrix& matWorldRotate)
{
	Matrix matRotate = matWorldRotate;

	//�ฮ��
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

// ȸ�� ������� �־��ָ� �� ȸ���� ��� ȸ���Ѵ�.
void TransformComponent::SetRotateWorld(const Quaternion& matWorldRotate)
{
	//����� �غ�
	Quaternion quatRot = matWorldRotate;

	//������� ���� ����غ�
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//������� ���� ȸ�������� ȸ������� ���������.

															//���� �θ� �ִٸ�...
	if (this->_pParent)
	{
		//ȸ�� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//�ฮ��
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

void TransformComponent::SetRotateLocal(const Quaternion& matWorldRotate)
{
	//����� �غ�
	Quaternion quatRot = matWorldRotate;

	//������� ���� ����غ�
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//������� ���� ȸ�������� ȸ������� ���������.

															//�ฮ��
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}


// �ڽ��� ȸ�� ���� from �� to ������ ȸ������ŭ ȸ������(���麸��) �Ͽ� ����
void TransformComponent::RotateSlerp(const TransformComponent& from, const TransformComponent& to, float t)
{
	t = Clamp01(t);

	Quaternion fromQuat = from.GetWorldRotateQuaternion();
	Quaternion toQuat = to.GetWorldRotateQuaternion();

	//t �� 0 �� ���ٸ�...
	if (FloatZero(t))
	{
		this->SetRotateWorld(fromQuat);
	}

	//t �� 1 �� ���ٸ�...
	else if (FloatEqual(t, 1.0f))
	{
		this->SetRotateWorld(toQuat);
	}


	//0 �� 1 ������ ���϶��� ����
	else
	{
		Quaternion result;

		//from �� to ��������� t ��ŭ�� ȸ�������� �Ͽ�
		//result ������� ����
		QuaternionSlerp(&result, &fromQuat, &toQuat, t);

		//������ ����� ������ ȸ�� ����
		this->SetRotateWorld(result);
	}
}

// �ڽ��� ��ġ ���� from �� to ������ ��ġ��ŭ �������� �Ͽ� ����
void TransformComponent::PositionLerp(const TransformComponent& from, const TransformComponent& to, float t)
{
	t = Clamp01(t);

	Vector3 fromWorldPos = from.GetWorldPosition();
	Vector3 toWorldPos = to.GetWorldPosition();

	//t �� 0 �� ���ٸ�...
	if (FloatZero(t))
	{
		this->SetWorldPosition(fromWorldPos);
	}

	//t �� 1 �� ���ٸ�...
	else if (FloatEqual(t, 1.0f))
	{
		this->SetWorldPosition(toWorldPos);
	}


	//0 �� 1 ������ ���϶��� ����
	else
	{
		Vector3 result;
		Vec3Lerp(&result, &fromWorldPos, &toWorldPos, t);
		this->SetWorldPosition(result);
	}
}

// �ڽ��� ��� ������ from �� to ������ ������ŭ ���� �Ͽ� ����
void TransformComponent::Interpolate(const TransformComponent& from, const TransformComponent& to, float t)
{
	t = Clamp01(t);

	Vector3 resultScale;
	Vector3 resultPosition;
	Quaternion resultRotate;

	//t �� 0 �� ���ٸ�...
	if (FloatZero(t))
	{
		resultScale = from._scale;
		resultPosition = from.GetWorldPosition();
		resultRotate = from.GetWorldRotateQuaternion();
	}

	//t �� 1 �� ���ٸ�...
	else if (FloatEqual(t, 1.0f))
	{
		resultScale = to._scale;
		resultPosition = to.GetWorldPosition();
		resultRotate = to.GetWorldRotateQuaternion();
	}


	//0 �� 1 ������ ���϶��� ����
	else
	{
		Vector3 fromScale = from._scale;
		Vector3 toScale = to._scale;

		Vector3 fromPosition = from.GetWorldPosition();
		Vector3 toPosition = to.GetWorldPosition();

		Quaternion fromQuat = from.GetWorldRotateQuaternion();
		Quaternion toQuat = to.GetWorldRotateQuaternion();


		//������ ����
		Vec3Lerp(&resultScale, &fromScale, &toScale, t);

		//��ġ ����
		Vec3Lerp(&resultPosition, &fromPosition, &toPosition, t);

		//ȸ�� ����
		QuaternionSlerp(&resultRotate, &fromQuat, &toQuat, t);


	}

	

	//TODO : Interpolation�� ��� �ؾ��ұ�????
	//�ʰ� ������ � ���� ���ϰ� �־���?
	//bool bPrevAutoUpdate = this->_autoUpdate;

	////�ϴ� ���������Ʈ ���´�
	//this->_autoUpdate = false;

	//this->SetScale(resultScale);
	//this->SetWorldPosition(resultPosition);
	//this->SetRotateWorld(resultRotate);

	////����
	//this->_autoUpdate = bPrevAutoUpdate;

	//if (this->_autoUpdate)
	//	this->UpdateTransform();
}

// Get ��..................
//
//���� ����� ��´�.
Matrix TransformComponent::GetFinalMatrix() const
{
	return this->_matFinal;
}

Matrix TransformComponent::GetWorldRotateMatrix() const
{
	//�ڽ��� ������ ȸ�� ����� ����� �糤��
	Matrix matRotate;
	MatrixIdentity(&matRotate);

	//3���� ��´�.
	Vector3 _axis[3];
	this->GetUnitAxies(_axis);

	//��Ŀ� ����
	memcpy(&matRotate._11, _axis + 0, sizeof(Vector3));
	memcpy(&matRotate._21, _axis + 1, sizeof(Vector3));
	memcpy(&matRotate._31, _axis + 2, sizeof(Vector3));

	return matRotate;
}

Quaternion TransformComponent::GetWorldRotateQuaternion() const
{
	Quaternion quat;

	Matrix matRotate = this->GetWorldRotateMatrix();

	//ȸ�� ��ķ� ������� �����.
	QuaternionRotationMatrix(&quat, &matRotate);

	return quat;
}

//��ġ ���� ��´�.
Vector3 TransformComponent::GetWorldPosition() const
{
	Vector3 pos = this->_position;

	if (this->_pParent) {
		Vec3TransformCoord(&pos, &pos, &this->_pParent->_matFinal);
	}

	return pos;
}

Vector3 TransformComponent::GetLocalPosition() const
{
	return this->_position;
}

//���� ��´�. ( ���� ���� )
void TransformComponent::GetScaledAxies(Vector3* pVecArr) const
{
	for (int i = 0; i < 3; i++) {
		pVecArr[i] = this->_axis[i];
	}

	//�θ� �ִٸ�..
	if (this->_pParent) {
		Matrix matParentFinal = this->_pParent->_matFinal;
		for (int i = 0; i < 3; i++) {
			Vec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}

void TransformComponent::GetUnitAxies(Vector3* pVecArr) const
{
	for (int i = 0; i < 3; i++) {
		Vec3Normalize(pVecArr + i, this->_axis + i);
	}

	//�θ� �ִٸ�..
	if (this->_pParent) {
		Matrix matParentFinal = this->_pParent->_matFinal;
		for (int i = 0; i < 3; i++) {
			Vec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}

Vector3 TransformComponent::GetScaledAxis(int axisNum) const
{
	Vector3 result = this->_axis[axisNum];

	//�θ� �ִٸ�..
	if (this->_pParent) {
		Matrix matParentFinal = this->_pParent->_matFinal;
		Vec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}

Vector3 TransformComponent::GetUnitAxis(int axisNum) const
{
	Vector3 result;
	Vec3Normalize(&result, this->_axis + axisNum);

	//�θ� �ִٸ�..
	if (this->_pParent) {
		Matrix matParentFinal = this->_pParent->_matFinal;
		Vec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}

Vector3 TransformComponent::GetForward(bool bNormalize /*= true*/) const
{
	if (bNormalize)
		return this->GetUnitAxis(AXIS_Z);

	return this->GetScaledAxis(AXIS_Z);
}

Vector3 TransformComponent::GetUp(bool bNormalize /*= true*/) const
{
	if (bNormalize)
		return this->GetUnitAxis(AXIS_Y);

	return this->GetScaledAxis(AXIS_Y);
}

Vector3 TransformComponent::GetRight(bool bNormalize /*= true*/) const
{
	if (bNormalize)
	{
		return this->GetUnitAxis(AXIS_X);
	}
	return this->GetScaledAxis(AXIS_X);
}

//TransformComponent �� ���� ����� �׸���.
//void TransformComponent::RenderGizmo(bool applyScale /*= false*/)
//{
//	Vector3 worldPos = this->GetWorldPosition();
//	Vector3 _axis[3];
//
//	if (applyScale)
//		this->GetScaledAxies(_axis);
//	else
//		this->GetUnitAxies(_axis);
//
//	//GIZMO_MGR->Line(worldPos, worldPos + _axis[2], 0xff0000ff);
//	//GIZMO_MGR->Line(worldPos, worldPos + _axis[1], 0xff00ff00);
//	//GIZMO_MGR->Line(worldPos, worldPos + _axis[0], 0xffff0000);
//
//}
