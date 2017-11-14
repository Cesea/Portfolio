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

	//정보 리셋
	this->Reset();

	MatrixIdentity(&_matFinal);
	MatrixIdentity(&_matLocal);
}


TransformComponent::~TransformComponent()
{
}

//TransformComponent 정보를 초기화
void TransformComponent::Reset(int resetFlag /*= -1*/)
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
		//this->_axis[0] = Vector3( 1, 0, 0 );
		//this->_axis[1] = Vector3( 0, 1, 0 );
		//this->_axis[2] = Vector3( 0, 0, 1 );

		this->_right = Vector3(1, 0, 0);
		this->_up = Vector3(0, 1, 0);
		this->_forward = Vector3(0, 0, 1);
	}

	if (resetFlag & RESET_SCALE)
	{
		//스케일의 초기화 값
		this->_scale = Vector3(1, 1, 1);
	}

	_transformDirty = true;
}

//특정 Child 를 내자식으로 붙인다.
void TransformComponent::AddChild(TransformComponent* pNewChild)
{
	//이미 내새끼면 할필요 없다
	if (pNewChild->_pParent == this)
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
	for (int32 i = 0; i < 3; i++) 
	{
		Vec3TransformNormal(pNewChild->_axis + i, pNewChild->_axis + i, &matInvFinal);
	}

	//3축에 대한 길이값을 얻는다.
	pNewChild->_scale.x = Vec3Length(&pNewChild->_right);
	pNewChild->_scale.y = Vec3Length(&pNewChild->_up);
	pNewChild->_scale.z = Vec3Length(&pNewChild->_forward);

	//정규화
	Vec3Normalize(&pNewChild->_right, &pNewChild->_right);
	Vec3Normalize(&pNewChild->_up, &pNewChild->_up);
	Vec3Normalize(&pNewChild->_forward, &pNewChild->_forward);

	//새로운 놈의 부모는 내가 된다.
	pNewChild->_pParent = this;

	//나의 자식놈 포인터
	TransformComponent* pChild = this->_pFirstChild;

	//자식이 없는 쓸쓸한 독거노인이라면...
	if (nullptr == pChild ) 
	{
		//안심하고 추가
		this->_pFirstChild = pNewChild;
		pNewChild->_pParent = this;
	}

	//대가족에 들어간다.
	else 
	{
		while (nullptr != pChild) 
		{
			//내가 들어갈 자리를 찾았다면...
			if (nullptr == pChild->_pNextSibling)
			{
				pChild->_pNextSibling = pNewChild;
				pNewChild->_pParent = this;
				break;
			}

			pChild = pChild->_pNextSibling;
		}
	}

	//업데이트
	_transformDirty = true;
}

//특정 TransformComponent 에 붙는다.
void TransformComponent::AttachTo(TransformComponent* pNewParent)
{
	pNewParent->AddChild(this);
}

//부모와 안녕
void TransformComponent::ReleaseParent()
{
	//부모가 없니?
	if (nullptr == _pParent)
	{
		return;
	}

	//부모랑 연을 끊기 전에 부모부터 자식연을 끊어라...
	TransformComponent* pChild = _pParent->_pFirstChild;

	//내가 부모의 첫째자식이니?
	if (this == pChild ) 
	{
		//내다음 자식이 첫번째 자식이 된다.
		this->_pParent->_pFirstChild = this->_pNextSibling;

		//형재들과의 연도 끊는다.
		this->_pNextSibling = NULL;
	}

	else
	{
		while (nullptr != pChild) 
		{
			//현재 자식의 다음이 나니?
			if (this == pChild->_pNextSibling) 
			{
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
	this->_pParent = nullptr;

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
	this->_forward = forwardUnit;
	this->_right = rightUnit;
	this->_up = upUnit;

	//스케일 대입
	this->_scale.x = scaleX;
	this->_scale.y = scaleY;
	this->_scale.z = scaleZ;

	//나만의 새상이 되었다...

	//부모랑 사라진 기념으로 Update 한번 콜
	_transformDirty = true;
}


//위치를 월드 좌표계로 셋팅한다. 
void TransformComponent::SetWorldPosition(float x, float y, float z)
{
	Vector3 pos(x, y, z);

	//부모가 있다면 부모의 상태적인 위치로 바꿔라...
	if (nullptr != this->_pParent )
	{
		//부모의 최종 행렬의 역행렬
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		//그 역행에 Pos 적용
		Vec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	_transformDirty = true;
}

void TransformComponent::SetWorldPosition(Vector3 pos)
{
	//부모가 있다면 부모의 상태적인 위치로 바꿔라...
	if (this->_pParent != NULL)
	{
		//부모의 최종 행렬의 역행렬
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		//그 역행에 Pos 적용
		Vec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;

	_transformDirty = true;
}

//위치를 로컬 좌표계로 셋팅한다.  ( 부모가 있는 경우 무모의 상태적인 위치 )
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

//자신의 축기준으로 이동 시킨다.
void TransformComponent::MovePositionSelf(float dx, float dy, float dz)
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

void TransformComponent::MovePositionSelf(Vector3 delta)
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
void TransformComponent::MovePositionWorld(float dx, float dy, float dz)
{
	//이동 벡터
	Vector3 move(dx, dy, dz);

	//월드 이동
	Vector3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + move);
}

void TransformComponent::MovePositionWorld(Vector3 delta)
{
	//월드 이동
	Vector3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + delta);
}


//부모가 있는 경우 로컬 기준으로 이동 시킨다.
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


//스케일 셋팅 
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

//스케일링
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

//월드 기준으로 회전 
void TransformComponent::RotateWorld(float angleX, float angleY, float angleZ)
{
	//부모가 있는 경우
	if (this->_pParent)
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
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		//최종 회전 행렬 대로 회전 시킨다.
		for (int i = 0; i < 3; i++)
			Vec3TransformNormal(this->_axis + i, worldAxis + i, &matRotate);

	_transformDirty = true;
	}


	//부모가 없는 경우
	else
	{
		RotateLocal(angleX, angleY, angleZ);
	}


}

void TransformComponent::RotateWorld(const Vector3 &angle)
{
	//부모가 있는 경우
	if (this->_pParent)
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
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		//최종 회전 행렬 대로 회전 시킨다.
		for (int i = 0; i < 3; i++)
			Vec3TransformNormal(this->_axis + i, worldAxis + i, &matRotate);


	
	_transformDirty = true;
	}

	//부모가 없는 경우
	else
	{
		RotateLocal(angle.x, angle.y, angle.z);
	}
}

//자신의 축기준으로 회전
void TransformComponent::RotateSelf(float angleX, float angleY, float angleZ)
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
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);


	
	_transformDirty = true;
}

void TransformComponent::RotateSelf(const Vector3 &angle)
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
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);


	
	_transformDirty = true;
}

//부모가 있는 경우 부모 로컬의 축기준으로 회전
void TransformComponent::RotateLocal(float angleX, float angleY, float angleZ)
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
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);


	
	_transformDirty = true;
}

void TransformComponent::RotateLocal(const Vector3 &angle)
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
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	
	_transformDirty = true;
}


//특정 방향을 바라보게 회전해라.
void TransformComponent::LookDirection(const Vector3 &dir, const Vector3 &_up /*= Vector3(0, 1, 0)*/)
{
	//정면 벡터
	Vector3 newForward = dir;

	//오른쪽벡터 ( 매개변수로 들어온 Up 을 가지고 외적 )
	Vector3 newRight;
	Vec3Cross(&newRight, &_up, &newForward);
	Vec3Normalize(&newRight, &newRight);

	//업 
	Vector3 newUp;
	Vec3Cross(&newUp, &newForward, &newRight);
	Vec3Normalize(&newUp, &newUp);

	//만약 부모가 있다면...
	if (this->_pParent)
	{
		//새로운 축 성분에 부모 역행렬 곱해....
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

//특정 방향을 바라보는데 angle 각만큼만 회전 해라
void TransformComponent::LookDirection(const Vector3 &dir, float angle)
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
	if (this->_pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//적용
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(this->_axis + i, worldAxis + i, &matRotate);

	
	_transformDirty = true;
}

//특정위치를 바라보게 회전해라.
void TransformComponent::LookPosition(const Vector3 &pos, const Vector3 &_up /*= Vector3(0, 1, 0)*/)
{
	//위치에 대한 방향벡터를 얻는다.
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 dir = pos - worldPos;

	Vec3Normalize(&dir, &dir);
	this->LookDirection(dir, _up);
}

//특정위치를  바라보는데 angle 각만큼만 회전 해라
void TransformComponent::LookPosition(const Vector3 &pos, float angle)
{
	//위치에 대한 방향벡터를 얻는다.
	Vector3 worldPos = this->GetWorldPosition();
	Vector3 dir = pos - worldPos;

	Vec3Normalize(&dir, &dir);
	this->LookDirection(dir, angle);
}

//사원수를 이용한 특정 회전값으로 회전량을 가져라....
void TransformComponent::SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ)
{
	//사원수 준비
	Quaternion quatRot;
	QuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//사원수에 의한 행렬준비
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

															//만약 부모가 있다면...
	if (this->_pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

void TransformComponent::SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ)
{
	//사원수 준비
	Quaternion quatRot;
	QuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//사원수에 의한 행렬준비
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

															//축리셋
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

// 회전 행렬을 넣어주면 그 회전 행렬대로 회전한다.
void TransformComponent::SetRotateWorld(const Matrix& matWorldRotate)
{
	Matrix matRotate = matWorldRotate;

	//만약 부모가 있다면...
	if (this->_pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

void TransformComponent::SetRotateLocal(const Matrix& matWorldRotate)
{
	Matrix matRotate = matWorldRotate;

	//축리셋
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

// 회전 사원수를 넣어주면 그 회전값 대로 회전한다.
void TransformComponent::SetRotateWorld(const Quaternion& matWorldRotate)
{
	//사원수 준비
	Quaternion quatRot = matWorldRotate;

	//사원수에 의한 행렬준비
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

															//만약 부모가 있다면...
	if (this->_pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		Matrix matInvParentFinal;
		MatrixInverse(&matInvParentFinal, NULL, &this->_pParent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}

void TransformComponent::SetRotateLocal(const Quaternion& matWorldRotate)
{
	//사원수 준비
	Quaternion quatRot = matWorldRotate;

	//사원수에 의한 행렬준비
	Matrix matRotate;
	MatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

															//축리셋
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_forward = Vector3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		Vec3TransformNormal(&this->_axis[i], &this->_axis[i], &matRotate);

	_transformDirty = true;
}


// 자신의 회전 값을 from 과 to 사이의 회전량만큼 회전보간(구면보간) 하여 적용
void TransformComponent::RotateSlerp(const TransformComponent& from, const TransformComponent& to, float t)
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
void TransformComponent::PositionLerp(const TransformComponent& from, const TransformComponent& to, float t)
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
void TransformComponent::Interpolate(const TransformComponent& from, const TransformComponent& to, float t)
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

	

	//TODO : Interpolation을 어떻게 해야할까????
	//너가 기존에 어떤 값을 지니고 있었니?
	//bool bPrevAutoUpdate = this->_autoUpdate;

	////일단 오토업데이트 막는다
	//this->_autoUpdate = false;

	//this->SetScale(resultScale);
	//this->SetWorldPosition(resultPosition);
	//this->SetRotateWorld(resultRotate);

	////돌려
	//this->_autoUpdate = bPrevAutoUpdate;

	//if (this->_autoUpdate)
	//	this->UpdateTransform();
}

// Get 들..................
//
//최종 행렬을 얻는다.
Matrix TransformComponent::GetFinalMatrix() const
{
	return this->_matFinal;
}

Matrix TransformComponent::GetWorldRotateMatrix() const
{
	//자신의 축으로 회전 행렬을 만들어 재낀다
	Matrix matRotate;
	MatrixIdentity(&matRotate);

	//3축을 얻는다.
	Vector3 _axis[3];
	this->GetUnitAxies(_axis);

	//행렬에 적용
	memcpy(&matRotate._11, _axis + 0, sizeof(Vector3));
	memcpy(&matRotate._21, _axis + 1, sizeof(Vector3));
	memcpy(&matRotate._31, _axis + 2, sizeof(Vector3));

	return matRotate;
}

Quaternion TransformComponent::GetWorldRotateQuaternion() const
{
	Quaternion quat;

	Matrix matRotate = this->GetWorldRotateMatrix();

	//회전 행렬로 사원수를 만든다.
	QuaternionRotationMatrix(&quat, &matRotate);

	return quat;
}

//위치 값을 얻는다.
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

//축을 얻는다. ( 월드 기준 )
void TransformComponent::GetScaledAxies(Vector3* pVecArr) const
{
	for (int i = 0; i < 3; i++) {
		pVecArr[i] = this->_axis[i];
	}

	//부모가 있다면..
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

	//부모가 있다면..
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

	//부모가 있다면..
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

	//부모가 있다면..
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

//TransformComponent 에 대한 기즈모를 그린다.
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
