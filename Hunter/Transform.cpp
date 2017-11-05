#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
{
	this->pParent = NULL;
	this->_pFirstChild = NULL;
	this->_pNextSibling = NULL;

	this->_bAutoUpdate = true;

	//���� ����
	this->Reset();

	MatrixIdentity(&_matFinal);
	MatrixIdentity(&_matLocal);
	if (this->_bAutoUpdate)
		this->UpdateTransform();
}


Transform::~Transform()
{
}

//Transform ������ �ʱ�ȭ
void Transform::Reset(int resetFlag /*= -1*/)
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
		//this->axis[0] = Vector3( 1, 0, 0 );
		//this->axis[1] = Vector3( 0, 1, 0 );
		//this->axis[2] = Vector3( 0, 0, 1 );

		this->right = Vector3(1, 0, 0);
		this->up = Vector3(0, 1, 0);
		this->forward = Vector3(0, 0, 1);
	}

	if (resetFlag & RESET_SCALE)
	{
		//�������� �ʱ�ȭ ��
		this->_scale = Vector3(1, 1, 1);
	}

	this->UpdateTransform();
}

//Ư�� Child �� ���ڽ����� ���δ�.
void Transform::AddChild(Transform* pNewChild)
{
	//�̹� �������� ���ʿ� ����
	if (pNewChild->pParent == this)
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
	for (int i = 0; i < 3; i++) {
		Vec3TransformNormal(pNewChild->axis + i, pNewChild->axis + i, &matInvFinal);
	}

	//3�࿡ ���� ���̰��� ��´�.
	pNewChild->_scale.x = Vec3Length(&pNewChild->right);
	pNewChild->_scale.y = Vec3Length(&pNewChild->up);
	pNewChild->_scale.z = Vec3Length(&pNewChild->forward);

	//����ȭ
	Vec3Normalize(&pNewChild->right, &pNewChild->right);
	Vec3Normalize(&pNewChild->up, &pNewChild->up);
	Vec3Normalize(&pNewChild->forward, &pNewChild->forward);

	//���ο� ���� �θ�� ���� �ȴ�.
	pNewChild->pParent = this;

	//���� �ڽĳ� ������
	Transform* pChild = this->_pFirstChild;

	//�ڽ��� ���� ������ ���ų����̶��...
	if (pChild == NULL) {
		//�Ƚ��ϰ� �߰�
		this->_pFirstChild = pNewChild;
		pNewChild->pParent = this;
	}

	//�밡���� ����.
	else {

		while (pChild != NULL) {

			//���� �� �ڸ��� ã�Ҵٸ�...
			if (pChild->_pNextSibling == NULL)
			{
				pChild->_pNextSibling = pNewChild;
				pNewChild->pParent = this;
				break;
			}

			pChild = pChild->_pNextSibling;
		}
	}

	//������Ʈ
	this->UpdateTransform();

}

//Ư�� Transform �� �ٴ´�.
void Transform::AttachTo(Transform* pNewParent)
{
	pNewParent->AddChild(this);
}

//�θ�� �ȳ�
void Transform::ReleaseParent()
{
	//�θ� ����?
	if (this->pParent == NULL)
		return;

	//�θ�� ���� ���� ���� �θ���� �ڽĿ��� �����...
	Transform* pChild = this->pParent->_pFirstChild;

	//���� �θ��� ù°�ڽ��̴�?
	if (pChild == this) {

		//������ �ڽ��� ù��° �ڽ��� �ȴ�.
		this->pParent->_pFirstChild = this->_pNextSibling;

		//�������� ���� ���´�.
		this->_pNextSibling = NULL;
	}

	else
	{
		while (pChild != NULL) {

			//���� �ڽ��� ������ ����?
			if (pChild->_pNextSibling == this) {

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
	this->pParent = NULL;

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
	this->forward = forwardUnit;
	this->right = rightUnit;
	this->up = upUnit;

	//������ ����
	this->_scale.x = scaleX;
	this->_scale.y = scaleY;
	this->_scale.z = scaleZ;

	//������ ������ �Ǿ���...

	//�θ�� ����� ������� Update �ѹ� ��
	this->UpdateTransform();

}


//��ġ�� ���� ��ǥ��� �����Ѵ�. 
void Transform::SetWorldPosition(float x, float y, float z)
{
	Vector3 pos(x, y, z);

	//�θ� �ִٸ� �θ��� �������� ��ġ�� �ٲ��...
	if (this->pParent != NULL)
	{
		//�θ��� ���� ����� �����
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		//�� ���࿡ Pos ����
		Vec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetWorldPosition(Vector3 pos)
{
	//�θ� �ִٸ� �θ��� �������� ��ġ�� �ٲ��...
	if (this->pParent != NULL)
	{
		//�θ��� ���� ����� �����
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		//�� ���࿡ Pos ����
		Vec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//��ġ�� ���� ��ǥ��� �����Ѵ�.  ( �θ� �ִ� ��� ������ �������� ��ġ )
void Transform::SetLocalPosition(float x, float y, float z)
{
	this->_position.x = x;
	this->_position.y = y;
	this->_position.z = z;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetLocalPosition(Vector3 pos)
{
	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//�ڽ��� ��������� �̵� ��Ų��.
void Transform::MovePositionSelf(float dx, float dy, float dz)
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

void Transform::MovePositionSelf(Vector3 delta)
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
void Transform::MovePositionWorld(float dx, float dy, float dz)
{
	//�̵� ����
	Vector3 move(dx, dy, dz);

	//���� �̵�
	Vector3 nowWorldPos = this->GetWorldPosition();

	//�Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->SetWorldPosition(nowWorldPos + move);
}

void Transform::MovePositionWorld(Vector3 delta)
{
	//���� �̵�
	Vector3 nowWorldPos = this->GetWorldPosition();

	//�Ʒ��� �Լ����� TransformUpdate �� �Ͼ
	this->SetWorldPosition(nowWorldPos + delta);
}


//�θ� �ִ� ��� ���� �������� �̵� ��Ų��.
void Transform::MovePositionLocal(float dx, float dy, float dz)
{
	this->_position.x += dx;
	this->_position.y += dy;
	this->_position.z += dz;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::MovePositionLocal(Vector3 delta)
{
	this->_position += delta;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}


//������ ���� 
void Transform::SetScale(float x, float y, float z)
{
	this->_scale.x = x;
	this->_scale.y = y;
	this->_scale.z = z;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetScale(Vector3 scale)
{
	this->_scale = scale;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//�����ϸ�
void Transform::Scaling(float dx, float dy, float dz)
{
	this->_scale.x += dx;
	this->_scale.y += dy;
	this->_scale.z += dz;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::Scaling(Vector3 deltaScale)
{
	this->_scale += deltaScale;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//���� �������� ȸ�� 
void Transform::RotateWorld(float angleX, float angleY, float angleZ)
{
	//�θ� �ִ� ���
	if (this->pParent)
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
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		//���� ȸ�� ��� ��� ȸ�� ��Ų��.
		for (int i = 0; i < 3; i++)
			Vec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);


		if (this->_bAutoUpdate)
			this->UpdateTransform();
	}


	//�θ� ���� ���
	else
	{
		RotateLocal(angleX, angleY, angleZ);
	}


}

void Transform::RotateWorld(Vector3 angle)
{
	//�θ� �ִ� ���
	if (this->pParent)
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
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		//���� ȸ�� ��� ��� ȸ�� ��Ų��.
		for (int i = 0; i < 3; i++)
			Vec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);


		if (this->_bAutoUpdate)
			this->UpdateTransform();
	}

	//�θ� ���� ���
	else
	{
		RotateLocal(angle.x, angle.y, angle.z);
	}
}

//�ڽ��� ��������� ȸ��
void Transform::RotateSelf(float angleX, float angleY, float angleZ)
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
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::RotateSelf(Vector3 angle)
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
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//�θ� �ִ� ��� �θ� ������ ��������� ȸ��
void Transform::RotateLocal(float angleX, float angleY, float angleZ)
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
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::RotateLocal(Vector3 angle)
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
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}


//Ư�� ������ �ٶ󺸰� ȸ���ض�.
void Transform::LookDirection(Vector3 dir, Vector3 up /*= Vector3(0, 1, 0)*/)
{
	//���� ����
	Vector3 newForward = dir;

	//�����ʺ��� ( �Ű������� ���� Up �� ������ ���� )
	Vector3 newRight;
	Vec3Cross(&newRight, &up, &newForward);
	Vec3Normalize(&newRight, &newRight);

	//�� 
	Vector3 newUp;
	Vec3Cross(&newUp, &newForward, &newRight);
	Vec3Normalize(&newUp, &newUp);

	//���� �θ� �ִٸ�...
	if (this->pParent)
	{
		//���ο� �� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		Vec3TransformNormal(&this->forward, &newForward, &matInvParentFinal);
		Vec3TransformNormal(&this->right, &newRight, &matInvParentFinal);
		Vec3TransformNormal(&this->up, &newUp, &matInvParentFinal);
	}

	else
	{
		this->forward = newForward;
		this->right = newRight;
		this->up = newUp;
	}

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//Ư�� ������ �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
void Transform::LookDirection(Vector3 dir, float angle)
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
	if (this->pParent)
	{
		//ȸ�� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//����
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//Ư����ġ�� �ٶ󺸰� ȸ���ض�.
void Transform::LookPosition(Vector3 pos, Vector3 up /*= Vector3(0, 1, 0)*/)
{
	//��ġ�� ���� ���⺤�͸� ��´�.
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 dir = pos - worldPos;

	Vec3Normalize(&dir, &dir);
	this->LookDirection(dir, up);
}

//Ư����ġ��  �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
void Transform::LookPosition(Vector3 pos, float angle)
{
	//��ġ�� ���� ���⺤�͸� ��´�.
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 dir = pos - worldPos;

	Vec3Normalize(&dir, &dir);
	this->LookDirection(dir, angle);
}

//������� �̿��� Ư�� ȸ�������� ȸ������ ������....
void Transform::SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ)
{
	//����� �غ�
	Quaternion quatRot;
	QuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//������� ���� ����غ�
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//������� ���� ȸ�������� ȸ������� ���������.

															//���� �θ� �ִٸ�...
	if (this->pParent)
	{
		//ȸ�� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//�ฮ��
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ)
{
	//����� �غ�
	Quaternion quatRot;
	QuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//������� ���� ����غ�
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//������� ���� ȸ�������� ȸ������� ���������.

															//�ฮ��
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

// ȸ�� ����� �־��ָ� �� ȸ�� ��Ĵ�� ȸ���Ѵ�.
void Transform::SetRotateWorld(const Matrix& matWorldRotate)
{
	Matrix matRotate = matWorldRotate;

	//���� �θ� �ִٸ�...
	if (this->pParent)
	{
		//ȸ�� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//�ฮ��
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetRotateLocal(const Matrix& matWorldRotate)
{
	Matrix matRotate = matWorldRotate;

	//�ฮ��
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

// ȸ�� ������� �־��ָ� �� ȸ���� ��� ȸ���Ѵ�.
void Transform::SetRotateWorld(const Quaternion& matWorldRotate)
{
	//����� �غ�
	Quaternion quatRot = matWorldRotate;

	//������� ���� ����غ�
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//������� ���� ȸ�������� ȸ������� ���������.

															//���� �θ� �ִٸ�...
	if (this->pParent)
	{
		//ȸ�� ���п� �θ� ����� ����....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//�ฮ��
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetRotateLocal(const Quaternion& matWorldRotate)
{
	//����� �غ�
	Quaternion quatRot = matWorldRotate;

	//������� ���� ����غ�
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//������� ���� ȸ�������� ȸ������� ���������.

															//�ฮ��
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}


// �ڽ��� ȸ�� ���� from �� to ������ ȸ������ŭ ȸ������(���麸��) �Ͽ� ����
void Transform::RotateSlerp(const Transform& from, const Transform& to, float t)
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
void Transform::PositionLerp(const Transform& from, const Transform& to, float t)
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
void Transform::Interpolate(const Transform& from, const Transform& to, float t)
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

	//�ʰ� ������ � ���� ���ϰ� �־���?
	bool bPrevAutoUpdate = this->_bAutoUpdate;

	//�ϴ� ���������Ʈ ���´�
	this->_bAutoUpdate = false;

	this->SetScale(resultScale);
	this->SetWorldPosition(resultPosition);
	this->SetRotateWorld(resultRotate);

	//����
	this->_bAutoUpdate = bPrevAutoUpdate;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}



void Transform::UpdateTransform()
{
	//�ڽ��� ������ matLocal ����� �����Ѵ�.
	MatrixIdentity(&this->_matLocal);

	//������ ���� ������
	Vector3 scaledRight = this->right * this->_scale.x;
	Vector3 scaledUp = this->up * this->_scale.y;
	Vector3 scaledForward = this->forward * this->_scale.z;

	memcpy(&this->_matLocal._11, &scaledRight, sizeof(Vector3));
	memcpy(&this->_matLocal._21, &scaledUp, sizeof(Vector3));
	memcpy(&this->_matLocal._31, &scaledForward, sizeof(Vector3));
	memcpy(&this->_matLocal._41, &this->_position, sizeof(Vector3));

	//���� ���� ���
	if (this->pParent == NULL)
	{
		this->_matFinal = _matLocal;
	}

	//���� �θ� �ִٸ�...
	else 
	{
		this->_matFinal = _matLocal * this->pParent->_matFinal;
	}

	//�ڽ� ������Ʈ
	Transform* pChild = this->_pFirstChild;

	//�ڽĵ�� ����鵵 ������Ʈ
	while (pChild != NULL)
	{
		pChild->UpdateTransform();
		pChild = pChild->_pNextSibling;
	}
}


//Device �� �ڽ��� Tansform �� ����� ����
void Transform::SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &_matFinal);
}

//Device �� �ڽ��� Tansform �� ��� ����
void Transform::SetDeviceView(LPDIRECT3DDEVICE9 pDevice)
{
	Matrix matView;
	MatrixInverse(&matView, NULL, &_matFinal);

	pDevice->SetTransform(D3DTS_VIEW, &matView);
}


//
// Get ��..................
//
//���� ����� ��´�.
Matrix Transform::GetFinalMatrix() const
{
	return this->_matFinal;
}

Matrix Transform::GetWorldRotateMatrix() const
{
	//�ڽ��� ������ ȸ�� ����� ����� �糤��
	Matrix matRotate;
	MatrixIdentity(&matRotate);

	//3���� ��´�.
	Vector3 axis[3];
	this->GetUnitAxies(axis);

	//��Ŀ� ����
	memcpy(&matRotate._11, axis + 0, sizeof(Vector3));
	memcpy(&matRotate._21, axis + 1, sizeof(Vector3));
	memcpy(&matRotate._31, axis + 2, sizeof(Vector3));

	return matRotate;
}

Quaternion Transform::GetWorldRotateQuaternion() const
{
	Quaternion quat;

	Matrix matRotate = this->GetWorldRotateMatrix();

	//ȸ�� ��ķ� ������� �����.
	QuaternionRotationMatrix(&quat, &matRotate);

	return quat;
}

//��ġ ���� ��´�.
Vector3 Transform::GetWorldPosition() const
{
	Vector3 pos = this->_position;

	if (this->pParent) {
		Vec3TransformCoord(&pos, &pos, &this->pParent->_matFinal);
	}

	return pos;
}

Vector3 Transform::GetLocalPosition() const
{
	return this->_position;
}

//���� ��´�. ( ���� ���� )
void Transform::GetScaledAxies(Vector3* pVecArr) const
{
	for (int i = 0; i < 3; i++) {
		pVecArr[i] = this->axis[i];
	}

	//�θ� �ִٸ�..
	if (this->pParent) {
		Matrix matParentFinal = this->pParent->_matFinal;
		for (int i = 0; i < 3; i++) {
			Vec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}

void Transform::GetUnitAxies(Vector3* pVecArr) const
{
	for (int i = 0; i < 3; i++) {
		Vec3Normalize(pVecArr + i, this->axis + i);
	}

	//�θ� �ִٸ�..
	if (this->pParent) {
		Matrix matParentFinal = this->pParent->_matFinal;
		for (int i = 0; i < 3; i++) {
			Vec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}

Vector3 Transform::GetScaledAxis(int axisNum) const
{
	Vector3 result = this->axis[axisNum];

	//�θ� �ִٸ�..
	if (this->pParent) {
		Matrix matParentFinal = this->pParent->_matFinal;
		Vec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}

Vector3 Transform::GetUnitAxis(int axisNum) const
{
	Vector3 result;
	Vec3Normalize(&result, this->axis + axisNum);

	//�θ� �ִٸ�..
	if (this->pParent) {
		Matrix matParentFinal = this->pParent->_matFinal;
		Vec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}

Vector3 Transform::GetForward(bool bNormalize /*= true*/) const
{
	if (bNormalize)
		return this->GetUnitAxis(AXIS_Z);

	return this->GetScaledAxis(AXIS_Z);
}

Vector3 Transform::GetUp(bool bNormalize /*= true*/) const
{
	if (bNormalize)
		return this->GetUnitAxis(AXIS_Y);

	return this->GetScaledAxis(AXIS_Y);
}

Vector3 Transform::GetRight(bool bNormalize /*= true*/) const
{
	if (bNormalize)
		return this->GetUnitAxis(AXIS_X);

	return this->GetScaledAxis(AXIS_X);
}

////����Ʈ ��Ʈ�� �����ش�.
//void Transform::DefaultControl(float timeDelta)
//{
//	//����Ʈ ��Ʈ���� ���� ī�޶� Angle ��
//	static float nowAngleH = 0.0f;			//����ޱ�
//	static float nowAngleV = 0.0f;			//�����ޱ�
//	static float maxAngleV = 85.0f;			//���� �ִ� �ޱ�
//	static float minAngleV = -85.0f;			//���� ���� �ޱ�
//	static float sensitivityH = 1.0f;					//���� �ΰ���
//	static float sensitivityV = 1.0f;					//���� �ΰ��� ( �̰��� ������ Invert Mouse )
//	static Vector3 nowVelocity(0, 0, 0);			//���� ����� �ӵ��� ���� ����
//
//	static float accelate = 30.0f;						//�ʴ� �̵� ������
//	static float nowSpeed = 3.0f;						//���� �ӵ�
//	static float maxSpeed = 10.0f;						//�ְ� �ӵ� 
//
//
//														//���� �������� ���콺 ��ġ�� ����� ���� ����
//	if (KEY_MGR->IsOnceDown(VK_RBUTTON)) {
//
//		//ȭ���� �߽���ġ
//		int screenCenterX = WINSIZE_X / 2;
//		int screenCenterY = WINSIZE_Y / 2;
//
//		//�ٽ� ���콺 ��ġ�� ���ͷ�...
//		SetMousePos(screenCenterX, screenCenterY);
//	}
//
//	//��Ŭ���� �Ҷ��� Default Control �� �Ѵ�
//	else if (KEY_MGR->IsStayDown(VK_RBUTTON)) {
//
//		//
//		// �̵� ó��
//		//
//
//		//�Է� ���⺤��
//		Vector3 inputVector(0, 0, 0);
//
//		if (KEY_MGR->IsStayDown('W')) {
//			inputVector.z = 1.0f;
//		}
//
//		else if (KEY_MGR->IsStayDown('S')) {
//			inputVector.z = -1.0f;
//		}
//
//		if (KEY_MGR->IsStayDown('A')) {
//			inputVector.x = -1.0f;
//		}
//
//		else if (KEY_MGR->IsStayDown('D')) {
//			inputVector.x = 1.0f;
//		}
//
//		if (KEY_MGR->IsStayDown('R')) {
//			inputVector.y = 1.0f;
//		}
//
//		else if (KEY_MGR->IsStayDown('F')) {
//			inputVector.y = -1.0f;
//		}
//
//		//���� ���Ͱ� �ƴҶ�
//		if (VECTORZERO(inputVector) == false)
//		{
//			//����ȭ
//			Vec3Normalize(&inputVector, &inputVector);
//		}
//
//		//�̵� �ض�
//		//Ÿ�ٺ��� 
//		Vector3 target = inputVector * maxSpeed;
//		this->MovePositionSelf(target * timeDelta);
//
//		//
//		// ȸ�� ó��
//		// 
//		//ȭ���� �߽���ġ
//		int screenCenterX = WINSIZE_X / 2;
//		int screenCenterY = WINSIZE_Y / 2;
//
//		//���� ���콺 ��ġ
//		POINT mousePos = GetMousePos();
//
//		//�̵��� ( �߾ӿ��� �־��� �� )
//		float deltaX = mousePos.x - screenCenterX;
//		float deltaY = mousePos.y - screenCenterY;
//
//		//�ޱ� �߰�
//		nowAngleH += deltaX * sensitivityH;
//		nowAngleV += deltaY * sensitivityV;
//
//		//�ޱ۰��� min max ���� ������
//		nowAngleV = Clamp(nowAngleV, minAngleV, maxAngleV);
//
//		//�ٽ� ���콺 ��ġ�� ���ͷ�...
//		SetMousePos(screenCenterX, screenCenterY);
//
//
//		this->SetRotateWorld(nowAngleV * ONE_RAD, nowAngleH * ONE_RAD, 0.0f);
//
//	}
//}
//
//void Transform::DefaultControl2(float timeDelta)
//{
//	float deltaMove = 3.0f * timeDelta;
//	float deltaAngle = 90.0f * ONE_RAD * timeDelta;
//
//	if (KEY_MGR->IsStayDown(VK_RBUTTON) == false)
//	{
//		if (KEY_MGR->IsStayDown('A'))
//			this->MovePositionSelf(-deltaMove, 0.0f, 0.0f);
//		else if (KEY_MGR->IsStayDown('D'))
//			this->MovePositionSelf(deltaMove, 0.0f, 0.0f);
//
//		if (KEY_MGR->IsStayDown('W'))
//			this->MovePositionSelf(0.0f, 0.0f, deltaMove);
//		else if (KEY_MGR->IsStayDown('S'))
//			this->MovePositionSelf(0.0f, 0.0f, -deltaMove);
//
//		if (KEY_MGR->IsStayDown(VK_LEFT))
//			this->RotateSelf(0.0f, -deltaAngle, 0.0f);
//		else if (KEY_MGR->IsStayDown(VK_RIGHT))
//			this->RotateSelf(0.0f, deltaAngle, 0.0f);
//
//		if (KEY_MGR->IsStayDown(VK_UP))
//			this->RotateSelf(-deltaAngle, 0.0f, 0.0f);
//		else if (KEY_MGR->IsStayDown(VK_DOWN))
//			this->RotateSelf(deltaAngle, 0.0f, 0.0f);
//	}
//}


//Transform �� ���� ����� �׸���.
void Transform::RenderGizmo(bool applyScale /*= false*/)
{
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 axis[3];

	if (applyScale)
		this->GetScaledAxies(axis);
	else
		this->GetUnitAxies(axis);

	//GIZMO_MGR->Line(worldPos, worldPos + axis[2], 0xff0000ff);
	//GIZMO_MGR->Line(worldPos, worldPos + axis[1], 0xff00ff00);
	//GIZMO_MGR->Line(worldPos, worldPos + axis[0], 0xffff0000);

}


