#ifndef TRANSFORM_H
#define TRANSFORM_H

#define RESET_POSITION 1
#define RESET_ROTATION 2
#define RESET_SCALE	   4

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

class Transform
{
protected:
	union
	{
		struct{
			D3DXVECTOR3	axis[3];
		};
		struct
		{
			D3DXVECTOR3 right;
			D3DXVECTOR3 up	;
			D3DXVECTOR3 forward;

		};
	};

	//위치
	D3DXVECTOR3 _position;

	//스케일
	D3DXVECTOR3 _scale;

	D3DXMATRIXA16 _matFinal;

	bool _bAutoUpdate;

	Transform* pParent;

	Transform* _pFirstChild;

	Transform* _pNextSibling;

	D3DXMATRIXA16 _matLocal;

public:
	Transform();
	~Transform();

	void SetAutoUpdate(bool bauto)
	{
		this->_bAutoUpdate = bauto;
	}

	void Reset(int resetFlag = -1);

	void AddChild(Transform* pChild);

	void AttachTo(Transform* pParent);

	void ReleaseParent();

	//위치를 월드 좌표계로 셋팅한다. 
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(D3DXVECTOR3 pos);


	//위치를 로컬 좌표계로 셋팅한다.  ( 부모가 있는 경우 무모의 상태적인 위치 )
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(D3DXVECTOR3 pos);


	//자신의 축기준으로 이동 시킨다.
	void MovePositionSelf(float dx, float dy, float dz);
	void MovePositionSelf(D3DXVECTOR3 delta);

	//월드 기준으로 이동 시킨다.
	void MovePositionWorld(float dx, float dy, float dz);
	void MovePositionWorld(D3DXVECTOR3 delta);

	//부모가 있는 경우 로컬 기준으로 이동 시킨다.
	void MovePositionLocal(float dx, float dy, float dz);
	void MovePositionLocal(D3DXVECTOR3 delta);



	//스케일 셋팅 
	void SetScale(float x, float y, float z);
	void SetScale(D3DXVECTOR3 scale);

	//스케일링
	void Scaling(float dx, float dy, float dz);
	void Scaling(D3DXVECTOR3 deltaScale);

	//월드 기준으로 회전 
	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(D3DXVECTOR3 angle);

	//자신의 축기준으로 회전
	void RotateSelf(float angleX, float angleY, float angleZ);
	void RotateSelf(D3DXVECTOR3 angle);


	//부모가 있는 경우 부모 로컬의 축기준으로 회전
	void RotateLocal(float angleX, float angleY, float angleZ);
	void RotateLocal(D3DXVECTOR3 angle);


	//사원수를 이용한 특정 회전값으로 회전량을 가져라....
	void SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	// 회전 행렬을 넣어주면 그 회전 행렬대로 회전한다.
	void SetRotateWorld(const D3DXMATRIXA16& matWorldRotate);
	void SetRotateLocal(const D3DXMATRIXA16& matWorldRotate);

	// 회전 사원수를 넣어주면 그 회전값 대로 회전한다.
	void SetRotateWorld(const D3DXQUATERNION& matWorldRotate);
	void SetRotateLocal(const D3DXQUATERNION& matWorldRotate);

	//특정 방향을 바라보게 회전해라.
	void LookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	//특정 방향을 바라보는데 angle 각만큼만 회전 해라
	void LookDirection(D3DXVECTOR3 dir, float angle);

	//특정위치를 바라보게 회전해라.
	void LookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	//특정위치를  바라보는데 angle 각만큼만 회전 해라
	void LookPosition(D3DXVECTOR3 pos, float angle);

	// 자신의 회전 값을 from 과 to 사이의 회전량만큼 회전보간(구면보간) 하여 적용
	void RotateSlerp(const Transform& from, const Transform& to, float t);

	// 자신의 위치 값을 from 과 to 사이의 위치만큼 선형보간 하여 적용
	void PositionLerp(const Transform& from, const Transform& to, float t);

	// 자신의 모든 정보를 from 과 to 사이의 정보만큼 보간 하여 적용
	void Interpolate(const Transform& from, const Transform& to, float t);

	///// update/////

	void UpdateTransform();

	void SetDeviceWorld(LPDIRECT3DDEVICE9 pDevie);

	void SetDeviceView(LPDIRECT3DDEVICE9 pDevice);

	void RenderGizmo(bool applyScale = false);

	//void DefaultControl(float timeDelta);
	//void DefaultControl2(float timeDelta);


	///// Get  //////

	//최종 행렬을 얻는다.
	D3DXMATRIXA16 GetFinalMatrix() const;
	D3DXMATRIXA16 GetWorldRotateMatrix() const;
	D3DXQUATERNION GetWorldRotateQuaternion() const;

	//위치 값을 얻는다.
	D3DXVECTOR3 GetWorldPosition() const;
	D3DXVECTOR3 GetLocalPosition() const;

	//축을 얻는다. ( 월드 기준 )
	void GetScaledAxies(D3DXVECTOR3* pVecArr) const;
	void GetUnitAxies(D3DXVECTOR3* pVecArr) const;
	D3DXVECTOR3 GetScaledAxis(int axisNum) const;
	D3DXVECTOR3 GetUnitAxis(int axisNum) const;
	D3DXVECTOR3 GetForward(bool bNormalize = true) const;
	D3DXVECTOR3 GetUp(bool bNormalize = true) const;
	D3DXVECTOR3 GetRight(bool bNormalize = true) const;

	D3DXVECTOR3 GetScale() const {
		return this->_scale;
	}
};

#endif
