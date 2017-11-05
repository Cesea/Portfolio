#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
{
	this->pParent = NULL;
	this->_pFirstChild = NULL;
	this->_pNextSibling = NULL;

	this->_bAutoUpdate = true;

	//정보 리셋
	this->Reset();

	MatrixIdentity(&_matFinal);
	MatrixIdentity(&_matLocal);
	if (this->_bAutoUpdate)
		this->UpdateTransform();
}


Transform::~Transform()
{
}

//Transform 정보를 초기화
void Transform::Reset(int resetFlag /*= -1*/)
{
	if (resetFlag & RESET_POSITION)
	{
		//위치는 0, 0, 0
		this->_position.x = 0;
		this->_position.y = 0;
		this->_position.z = 0;
	}

	if (resetFlag & RESET_ROTATION)
	{
		//회전방향은 
		//this->axis[0] = Vector3( 1, 0, 0 );
		//this->axis[1] = Vector3( 0, 1, 0 );
		//this->axis[2] = Vector3( 0, 0, 1 );

		this->right = Vector3(1, 0, 0);
		this->up = Vector3(0, 1, 0);
		this->forward = Vector3(0, 0, 1);
	}

	if (resetFlag & RESET_SCALE)
	{
		//스케일의 초기화 값
		this->_scale = Vector3(1, 1, 1);
	}

	this->UpdateTransform();
}

//특정 Child 를 내자식으로 붙인다.
void Transform::AddChild(Transform* pNewChild)
{
	//이미 내새끼면 할필요 없다
	if (pNewChild->pParent == this)
		return;

	//너이새끼 내밑으로 들어올려면 부모랑 연을 끊어라...
	pNewChild->ReleaseParent();

	//부모의 상대적인 좌표값으로 갱신하기위해 
	//부모의 final 역행렬을 구한다.
	Matrix matInvFinal;
	MatrixInverse(&matInvFinal, NULL, &this->_matFinal);

	//자식의 Position 과 Axis 및 Scale 갱신
	Vec3TransformCoord(&pNewChild->_position, &pNewChild->_position, &matInvFinal);

	//축3개 변환하고 
	for (int i = 0; i < 3; i++) {
		Vec3TransformNormal(pNewChild->axis + i, pNewChild->axis + i, &matInvFinal);
	}

	//3축에 대한 길이값을 얻는다.
	pNewChild->_scale.x = Vec3Length(&pNewChild->right);
	pNewChild->_scale.y = Vec3Length(&pNewChild->up);
	pNewChild->_scale.z = Vec3Length(&pNewChild->forward);

	//정규화
	Vec3Normalize(&pNewChild->right, &pNewChild->right);
	Vec3Normalize(&pNewChild->up, &pNewChild->up);
	Vec3Normalize(&pNewChild->forward, &pNewChild->forward);

	//새로운 놈의 부모는 내가 된다.
	pNewChild->pParent = this;

	//나의 자식놈 포인터
	Transform* pChild = this->_pFirstChild;

	//자식이 없는 쓸쓸한 독거노인이라면...
	if (pChild == NULL) {
		//안심하고 추가
		this->_pFirstChild = pNewChild;
		pNewChild->pParent = this;
	}

	//대가족에 들어간다.
	else {

		while (pChild != NULL) {

			//내가 들어갈 자리를 찾았다면...
			if (pChild->_pNextSibling == NULL)
			{
				pChild->_pNextSibling = pNewChild;
				pNewChild->pParent = this;
				break;
			}

			pChild = pChild->_pNextSibling;
		}
	}

	//업데이트
	this->UpdateTransform();

}

//특정 Transform 에 붙는다.
void Transform::AttachTo(Transform* pNewParent)
{
	pNewParent->AddChild(this);
}

//부모와 안녕
void Transform::ReleaseParent()
{
	//부모가 없니?
	if (this->pParent == NULL)
		return;

	//부모랑 연을 끊기 전에 부모부터 자식연을 끊어라...
	Transform* pChild = this->pParent->_pFirstChild;

	//내가 부모의 첫째자식이니?
	if (pChild == this) {

		//내다음 자식이 첫번째 자식이 된다.
		this->pParent->_pFirstChild = this->_pNextSibling;

		//형재들과의 연도 끊는다.
		this->_pNextSibling = NULL;
	}

	else
	{
		while (pChild != NULL) {

			//현재 자식의 다음이 나니?
			if (pChild->_pNextSibling == this) {

				pChild->_pNextSibling = this->_pNextSibling;

				//형재들과의 연도 끊는다.
				this->_pNextSibling = NULL;

				break;
			}

			//다음 자식본다.
			pChild = pChild->_pNextSibling;
		}
	}

	//부모랑 연을 끊어라...
	this->pParent = NULL;

	//자신의 현재 월드 위치에 대한 갱신이 필요하다.
	//진짜월드 위치는 matFinal 이 다 가지고 있다.

	//월드 위치 갱신
	this->_position.x = this->_matFinal._41;
	this->_position.y = this->_matFinal._42;
	this->_position.z = this->_matFinal._43;

	//3축 얻어온다.
	Vector3 forwardScaled(this->_matFinal._31, this->_matFinal._32, this->_matFinal._33);
	Vector3 upScaled(this->_matFinal._21, this->_matFinal._22, this->_matFinal._23);
	Vector3 rightScaled(this->_matFinal._11, this->_matFinal._12, this->_matFinal._13);

	//3축에서 스케일 뺀다
	float scaleX = Vec3Length(&rightScaled);
	float scaleY = Vec3Length(&upScaled);
	float scaleZ = Vec3Length(&forwardScaled);

	//정규화
	Vector3 forwardUnit;
	Vector3 upUnit;
	Vector3 rightUnit;
	Vec3Normalize(&rightUnit, &rightScaled);
	Vec3Normalize(&upUnit, &upScaled);
	Vec3Normalize(&forwardUnit, &forwardScaled);

	//정규화된 3축 대입
	this->forward = forwardUnit;
	this->right = rightUnit;
	this->up = upUnit;

	//스케일 대입
	this->_scale.x = scaleX;
	this->_scale.y = scaleY;
	this->_scale.z = scaleZ;

	//나만의 새상이 되었다...

	//부모랑 사라진 기념으로 Update 한번 콜
	this->UpdateTransform();

}


//위치를 월드 좌표계로 셋팅한다. 
void Transform::SetWorldPosition(float x, float y, float z)
{
	Vector3 pos(x, y, z);

	//부모가 있다면 부모의 상태적인 위치로 바꿔라...
	if (this->pParent != NULL)
	{
		//부모의 최종 행렬의 역행렬
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		//그 역행에 Pos 적용
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
	//부모가 있다면 부모의 상태적인 위치로 바꿔라...
	if (this->pParent != NULL)
	{
		//부모의 최종 행렬의 역행렬
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		//그 역행에 Pos 적용
		Vec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//위치를 로컬 좌표계로 셋팅한다.  ( 부모가 있는 경우 무모의 상태적인 위치 )
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

//자신의 축기준으로 이동 시킨다.
void Transform::MovePositionSelf(float dx, float dy, float dz)
{
	//이동 벡터
	Vector3 move(0, 0, 0);

	//자신의 이동 축 얻는다.
	Vector3 moveAxis[3];
	this->GetUnitAxies(moveAxis);
	move += moveAxis[0] * dx;
	move += moveAxis[1] * dy;
	move += moveAxis[2] * dz;

	//월드 이동
	Vector3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + move);
}

void Transform::MovePositionSelf(Vector3 delta)
{
	//이동 벡터
	Vector3 move(0, 0, 0);

	//자신의 이동 축 얻는다.
	Vector3 moveAxis[3];
	this->GetUnitAxies(moveAxis);
	move += moveAxis[0] * delta.x;
	move += moveAxis[1] * delta.y;
	move += moveAxis[2] * delta.z;

	//월드 이동
	Vector3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + move);
}

//월드 기준으로 이동 시킨다.
void Transform::MovePositionWorld(float dx, float dy, float dz)
{
	//이동 벡터
	Vector3 move(dx, dy, dz);

	//월드 이동
	Vector3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + move);
}

void Transform::MovePositionWorld(Vector3 delta)
{
	//월드 이동
	Vector3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + delta);
}


//부모가 있는 경우 로컬 기준으로 이동 시킨다.
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


//스케일 셋팅 
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

//스케일링
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

//월드 기준으로 회전 
void Transform::RotateWorld(float angleX, float angleY, float angleZ)
{
	//부모가 있는 경우
	if (this->pParent)
	{
		//진짜로월드 축
		Vector3 worldAxis[3];
		this->GetUnitAxies(worldAxis);

		//각 축에 대한 회전 행렬
		Matrix matRotateX;
		MatrixRotationX(&matRotateX, angleX);

		Matrix matRotateY;
		MatrixRotationY(&matRotateY, angleY);

		Matrix matRotateZ;
		MatrixRotationZ(&matRotateZ, angleZ);

		//최종 회전 행렬
		Matrix matRotate = matRotateY * matRotateX * matRotateZ;

		//부모의 역행렬로 다시 회전
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		//최종 회전 행렬 대로 회전 시킨다.
		for (int i = 0; i < 3; i++)
			Vec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);


		if (this->_bAutoUpdate)
			this->UpdateTransform();
	}


	//부모가 없는 경우
	else
	{
		RotateLocal(angleX, angleY, angleZ);
	}


}

void Transform::RotateWorld(Vector3 angle)
{
	//부모가 있는 경우
	if (this->pParent)
	{
		//진짜로월드 축
		Vector3 worldAxis[3];
		this->GetUnitAxies(worldAxis);

		//각 축에 대한 회전 행렬
		Matrix matRotateX;
		MatrixRotationX(&matRotateX, angle.x);

		Matrix matRotateY;
		MatrixRotationY(&matRotateY, angle.y);

		Matrix matRotateZ;
		MatrixRotationZ(&matRotateZ, angle.z);


		//최종 회전 행렬만들때 미리 역행렬성분을 곱해놓으면 1 번만 회전시키면 된다.

		//최종 회전 행렬
		Matrix matRotate = matRotateY * matRotateX * matRotateZ;

		//부모의 역행렬로 다시 회전
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		//최종 회전 행렬 대로 회전 시킨다.
		for (int i = 0; i < 3; i++)
			Vec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);


		if (this->_bAutoUpdate)
			this->UpdateTransform();
	}

	//부모가 없는 경우
	else
	{
		RotateLocal(angle.x, angle.y, angle.z);
	}
}

//자신의 축기준으로 회전
void Transform::RotateSelf(float angleX, float angleY, float angleZ)
{
	//각 축에 대한 회전 행렬
	Matrix matRotateX;
	MatrixRotationAxis(&matRotateX, &this->GetRight(), angleX);

	Matrix matRotateY;
	MatrixRotationAxis(&matRotateY, &this->GetUp(), angleY);

	Matrix matRotateZ;
	MatrixRotationAxis(&matRotateZ, &this->GetForward(), angleZ);


	//최종 회전 행렬
	Matrix matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::RotateSelf(Vector3 angle)
{
	//각 축에 대한 회전 행렬
	Matrix matRotateX;
	MatrixRotationAxis(&matRotateX, &this->GetRight(), angle.x);

	Matrix matRotateY;
	MatrixRotationAxis(&matRotateY, &this->GetUp(), angle.y);

	Matrix matRotateZ;
	MatrixRotationAxis(&matRotateZ, &this->GetForward(), angle.z);


	//최종 회전 행렬
	Matrix matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//부모가 있는 경우 부모 로컬의 축기준으로 회전
void Transform::RotateLocal(float angleX, float angleY, float angleZ)
{
	//각 축에 대한 회전 행렬
	Matrix matRotateX;
	MatrixRotationX(&matRotateX, angleX);

	Matrix matRotateY;
	MatrixRotationY(&matRotateY, angleY);

	Matrix matRotateZ;
	MatrixRotationZ(&matRotateZ, angleZ);

	//최종 회전 행렬
	Matrix matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::RotateLocal(Vector3 angle)
{
	//각 축에 대한 회전 행렬
	Matrix matRotateX;
	MatrixRotationX(&matRotateX, angle.x);

	Matrix matRotateY;
	MatrixRotationY(&matRotateY, angle.y);

	Matrix matRotateZ;
	MatrixRotationZ(&matRotateZ, angle.z);

	//최종 회전 행렬
	Matrix matRotate = matRotateY * matRotateX * matRotateZ;

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}


//특정 방향을 바라보게 회전해라.
void Transform::LookDirection(Vector3 dir, Vector3 up /*= Vector3(0, 1, 0)*/)
{
	//정면 벡터
	Vector3 newForward = dir;

	//오른쪽벡터 ( 매개변수로 들어온 Up 을 가지고 외적 )
	Vector3 newRight;
	Vec3Cross(&newRight, &up, &newForward);
	Vec3Normalize(&newRight, &newRight);

	//업 
	Vector3 newUp;
	Vec3Cross(&newUp, &newForward, &newRight);
	Vec3Normalize(&newUp, &newUp);

	//만약 부모가 있다면...
	if (this->pParent)
	{
		//새로운 축 성분에 부모 역행렬 곱해....
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

//특정 방향을 바라보는데 angle 각만큼만 회전 해라
void Transform::LookDirection(Vector3 dir, float angle)
{
	//진짜로월드 축
	Vector3 worldAxis[3];
	this->GetUnitAxies(worldAxis);

	//정면 벡터와 바라볼 방향의 각도차를 얻자...
	float distRadian = acos(
		ClampMinusOnePlusOne(Vec3Dot(worldAxis + AXIS_Z, &dir)));

	//각도차가 거의 없다면 하지마...
	if (FloatZero(distRadian)) return;

	//외적 ( 내정면과 타겟까지의 방향을 외적 )
	Vector3 cross;
	Vec3Cross(&cross, worldAxis + AXIS_Z, &dir);
	Vec3Normalize(&cross, &cross);

	//외적축으로 각차만큼 회전 시키는 행렬
	Matrix matRotate;
	MatrixRotationAxis(&matRotate, &cross, min(angle, distRadian));

	//만약 부모가 있다면...
	if (this->pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//적용
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

//특정위치를 바라보게 회전해라.
void Transform::LookPosition(Vector3 pos, Vector3 up /*= Vector3(0, 1, 0)*/)
{
	//위치에 대한 방향벡터를 얻는다.
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 dir = pos - worldPos;

	Vec3Normalize(&dir, &dir);
	this->LookDirection(dir, up);
}

//특정위치를  바라보는데 angle 각만큼만 회전 해라
void Transform::LookPosition(Vector3 pos, float angle)
{
	//위치에 대한 방향벡터를 얻는다.
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 dir = pos - worldPos;

	Vec3Normalize(&dir, &dir);
	this->LookDirection(dir, angle);
}

//사원수를 이용한 특정 회전값으로 회전량을 가져라....
void Transform::SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ)
{
	//사원수 준비
	Quaternion quatRot;
	QuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//사원수에 의한 행렬준비
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

															//만약 부모가 있다면...
	if (this->pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ)
{
	//사원수 준비
	Quaternion quatRot;
	QuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//사원수에 의한 행렬준비
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

															//축리셋
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

// 회전 행렬을 넣어주면 그 회전 행렬대로 회전한다.
void Transform::SetRotateWorld(const Matrix& matWorldRotate)
{
	Matrix matRotate = matWorldRotate;

	//만약 부모가 있다면...
	if (this->pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetRotateLocal(const Matrix& matWorldRotate)
{
	Matrix matRotate = matWorldRotate;

	//축리셋
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

// 회전 사원수를 넣어주면 그 회전값 대로 회전한다.
void Transform::SetRotateWorld(const Quaternion& matWorldRotate)
{
	//사원수 준비
	Quaternion quatRot = matWorldRotate;

	//사원수에 의한 행렬준비
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

															//만약 부모가 있다면...
	if (this->pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetRotateLocal(const Quaternion& matWorldRotate)
{
	//사원수 준비
	Quaternion quatRot = matWorldRotate;

	//사원수에 의한 행렬준비
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

															//축리셋
	this->right = Vector3(1, 0, 0);
	this->up = Vector3(0, 1, 0);
	this->forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}


// 자신의 회전 값을 from 과 to 사이의 회전량만큼 회전보간(구면보간) 하여 적용
void Transform::RotateSlerp(const Transform& from, const Transform& to, float t)
{
	t = Clamp01(t);

	Quaternion fromQuat = from.GetWorldRotateQuaternion();
	Quaternion toQuat = to.GetWorldRotateQuaternion();

	//t 가 0 과 같다면...
	if (FloatZero(t))
	{
		this->SetRotateWorld(fromQuat);
	}

	//t 가 1 과 같다면...
	else if (FloatEqual(t, 1.0f))
	{
		this->SetRotateWorld(toQuat);
	}


	//0 과 1 사이의 값일때만 보간
	else
	{
		Quaternion result;

		//from 과 to 사원수간의 t 만큼의 회전보간을 하여
		//result 사원수에 대입
		QuaternionSlerp(&result, &fromQuat, &toQuat, t);

		//보간된 사원수 정보로 회전 셋팅
		this->SetRotateWorld(result);
	}
}

// 자신의 위치 값을 from 과 to 사이의 위치만큼 선형보간 하여 적용
void Transform::PositionLerp(const Transform& from, const Transform& to, float t)
{
	t = Clamp01(t);

	Vector3 fromWorldPos = from.GetWorldPosition();
	Vector3 toWorldPos = to.GetWorldPosition();

	//t 가 0 과 같다면...
	if (FloatZero(t))
	{
		this->SetWorldPosition(fromWorldPos);
	}

	//t 가 1 과 같다면...
	else if (FloatEqual(t, 1.0f))
	{
		this->SetWorldPosition(toWorldPos);
	}


	//0 과 1 사이의 값일때만 보간
	else
	{
		Vector3 result;
		Vec3Lerp(&result, &fromWorldPos, &toWorldPos, t);
		this->SetWorldPosition(result);
	}
}

// 자신의 모든 정보를 from 과 to 사이의 정보만큼 보간 하여 적용
void Transform::Interpolate(const Transform& from, const Transform& to, float t)
{
	t = Clamp01(t);

	Vector3 resultScale;
	Vector3 resultPosition;
	Quaternion resultRotate;

	//t 가 0 과 같다면...
	if (FloatZero(t))
	{
		resultScale = from._scale;
		resultPosition = from.GetWorldPosition();
		resultRotate = from.GetWorldRotateQuaternion();
	}

	//t 가 1 과 같다면...
	else if (FloatEqual(t, 1.0f))
	{
		resultScale = to._scale;
		resultPosition = to.GetWorldPosition();
		resultRotate = to.GetWorldRotateQuaternion();
	}


	//0 과 1 사이의 값일때만 보간
	else
	{
		Vector3 fromScale = from._scale;
		Vector3 toScale = to._scale;

		Vector3 fromPosition = from.GetWorldPosition();
		Vector3 toPosition = to.GetWorldPosition();

		Quaternion fromQuat = from.GetWorldRotateQuaternion();
		Quaternion toQuat = to.GetWorldRotateQuaternion();


		//스케일 보간
		Vec3Lerp(&resultScale, &fromScale, &toScale, t);

		//위치 보간
		Vec3Lerp(&resultPosition, &fromPosition, &toPosition, t);

		//회전 보간
		QuaternionSlerp(&resultRotate, &fromQuat, &toQuat, t);


	}

	//너가 기존에 어떤 값을 지니고 있었니?
	bool bPrevAutoUpdate = this->_bAutoUpdate;

	//일단 오토업데이트 막는다
	this->_bAutoUpdate = false;

	this->SetScale(resultScale);
	this->SetWorldPosition(resultPosition);
	this->SetRotateWorld(resultRotate);

	//돌려
	this->_bAutoUpdate = bPrevAutoUpdate;

	if (this->_bAutoUpdate)
		this->UpdateTransform();
}



void Transform::UpdateTransform()
{
	//자신의 정보로 matLocal 행렬을 갱신한다.
	MatrixIdentity(&this->_matLocal);

	//스케일 먹은 축정보
	Vector3 scaledRight = this->right * this->_scale.x;
	Vector3 scaledUp = this->up * this->_scale.y;
	Vector3 scaledForward = this->forward * this->_scale.z;

	memcpy(&this->_matLocal._11, &scaledRight, sizeof(Vector3));
	memcpy(&this->_matLocal._21, &scaledUp, sizeof(Vector3));
	memcpy(&this->_matLocal._31, &scaledForward, sizeof(Vector3));
	memcpy(&this->_matLocal._41, &this->_position, sizeof(Vector3));

	//나의 최종 행렬
	if (this->pParent == NULL)
	{
		this->_matFinal = _matLocal;
	}

	//내가 부모가 있다면...
	else 
	{
		this->_matFinal = _matLocal * this->pParent->_matFinal;
	}

	//자식 업데이트
	Transform* pChild = this->_pFirstChild;

	//자식들아 너희들도 업데이트
	while (pChild != NULL)
	{
		pChild->UpdateTransform();
		pChild = pChild->_pNextSibling;
	}
}


//Device 에 자신의 Tansform 을 월드로 셋팅
void Transform::SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &_matFinal);
}

//Device 에 자신의 Tansform 을 뷰로 셋팅
void Transform::SetDeviceView(LPDIRECT3DDEVICE9 pDevice)
{
	Matrix matView;
	MatrixInverse(&matView, NULL, &_matFinal);

	pDevice->SetTransform(D3DTS_VIEW, &matView);
}


//
// Get 들..................
//
//최종 행렬을 얻는다.
Matrix Transform::GetFinalMatrix() const
{
	return this->_matFinal;
}

Matrix Transform::GetWorldRotateMatrix() const
{
	//자신의 축으로 회전 행렬을 만들어 재낀다
	Matrix matRotate;
	MatrixIdentity(&matRotate);

	//3축을 얻는다.
	Vector3 axis[3];
	this->GetUnitAxies(axis);

	//행렬에 적용
	memcpy(&matRotate._11, axis + 0, sizeof(Vector3));
	memcpy(&matRotate._21, axis + 1, sizeof(Vector3));
	memcpy(&matRotate._31, axis + 2, sizeof(Vector3));

	return matRotate;
}

Quaternion Transform::GetWorldRotateQuaternion() const
{
	Quaternion quat;

	Matrix matRotate = this->GetWorldRotateMatrix();

	//회전 행렬로 사원수를 만든다.
	QuaternionRotationMatrix(&quat, &matRotate);

	return quat;
}

//위치 값을 얻는다.
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

//축을 얻는다. ( 월드 기준 )
void Transform::GetScaledAxies(Vector3* pVecArr) const
{
	for (int i = 0; i < 3; i++) {
		pVecArr[i] = this->axis[i];
	}

	//부모가 있다면..
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

	//부모가 있다면..
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

	//부모가 있다면..
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

	//부모가 있다면..
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

////디폴트 컨트롤 을해준다.
//void Transform::DefaultControl(float timeDelta)
//{
//	//디폴트 컨트롤을 위한 카메라 Angle 값
//	static float nowAngleH = 0.0f;			//수평앵글
//	static float nowAngleV = 0.0f;			//수직앵글
//	static float maxAngleV = 85.0f;			//수직 최대 앵글
//	static float minAngleV = -85.0f;			//수직 최저 앵글
//	static float sensitivityH = 1.0f;					//가로 민감도
//	static float sensitivityV = 1.0f;					//세로 민감도 ( 이값이 음수면 Invert Mouse )
//	static Vector3 nowVelocity(0, 0, 0);			//현제 방향과 속도를 가진 벡터
//
//	static float accelate = 30.0f;						//초당 이동 증가값
//	static float nowSpeed = 3.0f;						//현재 속도
//	static float maxSpeed = 10.0f;						//최고 속도 
//
//
//														//최초 누를때는 마우스 위치를 가운데로 놓고 시작
//	if (KEY_MGR->IsOnceDown(VK_RBUTTON)) {
//
//		//화면의 중심위치
//		int screenCenterX = WINSIZE_X / 2;
//		int screenCenterY = WINSIZE_Y / 2;
//
//		//다시 마우스 위치를 센터로...
//		SetMousePos(screenCenterX, screenCenterY);
//	}
//
//	//우클릭을 할때만 Default Control 을 한다
//	else if (KEY_MGR->IsStayDown(VK_RBUTTON)) {
//
//		//
//		// 이동 처리
//		//
//
//		//입력 방향벡터
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
//		//제로 벡터가 아닐때
//		if (VECTORZERO(inputVector) == false)
//		{
//			//정규화
//			Vec3Normalize(&inputVector, &inputVector);
//		}
//
//		//이동 해라
//		//타겟벡터 
//		Vector3 target = inputVector * maxSpeed;
//		this->MovePositionSelf(target * timeDelta);
//
//		//
//		// 회전 처리
//		// 
//		//화면의 중심위치
//		int screenCenterX = WINSIZE_X / 2;
//		int screenCenterY = WINSIZE_Y / 2;
//
//		//현재 마우스 위치
//		POINT mousePos = GetMousePos();
//
//		//이동량 ( 중앙에서 멀어진 량 )
//		float deltaX = mousePos.x - screenCenterX;
//		float deltaY = mousePos.y - screenCenterY;
//
//		//앵글 추가
//		nowAngleH += deltaX * sensitivityH;
//		nowAngleV += deltaY * sensitivityV;
//
//		//앵글값을 min max 범위 안으로
//		nowAngleV = Clamp(nowAngleV, minAngleV, maxAngleV);
//
//		//다시 마우스 위치를 센터로...
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


//Transform 에 대한 기즈모를 그린다.
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


