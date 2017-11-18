#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#define RESET_POSITION 1
#define RESET_ROTATION 2
#define RESET_SCALE	   4

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2


struct TransformComponent : public Component
{
	DECLARE_META(TransformComponent);

	TransformComponent();
	~TransformComponent();

	void Reset(int resetFlag = -1);

	void AddChild(TransformComponent* pChild);

	void AttachTo(TransformComponent* pParent);

	void ReleaseParent();

	//위치를 월드 좌표계로 셋팅한다. 
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(Vector3 pos);


	//위치를 로컬 좌표계로 셋팅한다.  ( 부모가 있는 경우 무모의 상태적인 위치 )
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(Vector3 pos);


	//자신의 축기준으로 이동 시킨다.
	void MovePositionSelf(float dx, float dy, float dz);
	void MovePositionSelf(Vector3 delta);

	//월드 기준으로 이동 시킨다.
	void MovePositionWorld(float dx, float dy, float dz);
	void MovePositionWorld(Vector3 delta);

	//부모가 있는 경우 로컬 기준으로 이동 시킨다.
	void MovePositionLocal(float dx, float dy, float dz);
	void MovePositionLocal(Vector3 delta);

	//스케일 셋팅 
	void SetScale(float x, float y, float z);
	void SetScale(Vector3 scale);

	//스케일링
	void Scaling(float dx, float dy, float dz);
	void Scaling(Vector3 deltaScale);

	//월드 기준으로 회전 
	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(const Vector3 &angle);

	//자신의 축기준으로 회전
	void RotateSelf(float angleX, float angleY, float angleZ);
	void RotateSelf(const Vector3 &angle);


	//부모가 있는 경우 부모 로컬의 축기준으로 회전
	void RotateLocal(float angleX, float angleY, float angleZ);
	void RotateLocal(const Vector3 &angle);


	//사원수를 이용한 특정 회전값으로 회전량을 가져라....
	void SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	// 회전 행렬을 넣어주면 그 회전 행렬대로 회전한다.
	void SetRotateWorld(const Matrix& matWorldRotate);
	void SetRotateLocal(const Matrix& matWorldRotate);

	// 회전 사원수를 넣어주면 그 회전값 대로 회전한다.
	void SetRotateWorld(const Quaternion& matWorldRotate);
	void SetRotateLocal(const Quaternion& matWorldRotate);

	//특정 방향을 바라보게 회전해라.
	void LookDirection(const Vector3 &dir, const Vector3 &up = Vector3(0, 1, 0));

	//특정 방향을 바라보는데 angle 각만큼만 회전 해라
	void LookDirection(const Vector3 &dir, float angle);

	//특정위치를 바라보게 회전해라.
	void LookPosition(const Vector3 &pos, const Vector3 &up = Vector3(0, 1, 0));

	//특정위치를  바라보는데 angle 각만큼만 회전 해라
	void LookPosition(const Vector3 &pos, float angle);

	// 자신의 회전 값을 from 과 to 사이의 회전량만큼 회전보간(구면보간) 하여 적용
	void RotateSlerp(const TransformComponent& from, const TransformComponent& to, float t);

	// 자신의 위치 값을 from 과 to 사이의 위치만큼 선형보간 하여 적용
	void PositionLerp(const TransformComponent& from, const TransformComponent& to, float t);

	// 자신의 모든 정보를 from 과 to 사이의 정보만큼 보간 하여 적용
	void Interpolate(const TransformComponent& from, const TransformComponent& to, float t);

	/////// update/////
	//void UpdateTransform();
	//void SetDeviceWorld(LPDIRECT3DDEVICE9 pDevie);
	//void SetDeviceView(LPDIRECT3DDEVICE9 pDevice);
	//void RenderGizmo(bool applyScale = false);

	///// Get  //////

	//최종 행렬을 얻는다.
	Matrix GetFinalMatrix() const;
	Matrix GetWorldRotateMatrix() const;
	Quaternion GetWorldRotateQuaternion() const;

	//위치 값을 얻는다.
	Vector3 GetWorldPosition() const;
	Vector3 GetLocalPosition() const;

	//축을 얻는다. ( 월드 기준 )
	void GetScaledAxies(Vector3* pVecArr) const;
	void GetUnitAxies(Vector3* pVecArr) const;
	Vector3 GetScaledAxis(int axisNum) const;
	Vector3 GetUnitAxis(int axisNum) const;
	Vector3 GetForward(bool bNormalize = true) const;
	Vector3 GetUp(bool bNormalize = true) const;
	Vector3 GetRight(bool bNormalize = true) const;

	Vector3 GetScale() const { return this->_scale; }

	//맴버 변수들

	union
	{
		struct{
			Vector3	_axis[3];
		};
		struct
		{
			Vector3 _right;
			Vector3 _up	;
			Vector3 _forward;

		};
	};

	Vector3 _position;
	Vector3 _dummyPosition;
	Vector3 _scale;

	Matrix _matFinal;
	Matrix _matLocal;

	TransformComponent* _pParent;
	TransformComponent* _pFirstChild;
	TransformComponent* _pNextSibling;

	bool32 _transformDirty;

};

#endif
