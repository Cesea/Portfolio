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

	//��ġ�� ���� ��ǥ��� �����Ѵ�. 
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(Vector3 pos);


	//��ġ�� ���� ��ǥ��� �����Ѵ�.  ( �θ� �ִ� ��� ������ �������� ��ġ )
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(Vector3 pos);


	//�ڽ��� ��������� �̵� ��Ų��.
	void MovePositionSelf(float dx, float dy, float dz);
	void MovePositionSelf(Vector3 delta);

	//���� �������� �̵� ��Ų��.
	void MovePositionWorld(float dx, float dy, float dz);
	void MovePositionWorld(Vector3 delta);

	//�θ� �ִ� ��� ���� �������� �̵� ��Ų��.
	void MovePositionLocal(float dx, float dy, float dz);
	void MovePositionLocal(Vector3 delta);

	//������ ���� 
	void SetScale(float x, float y, float z);
	void SetScale(Vector3 scale);

	//�����ϸ�
	void Scaling(float dx, float dy, float dz);
	void Scaling(Vector3 deltaScale);

	//���� �������� ȸ�� 
	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(const Vector3 &angle);

	//�ڽ��� ��������� ȸ��
	void RotateSelf(float angleX, float angleY, float angleZ);
	void RotateSelf(const Vector3 &angle);


	//�θ� �ִ� ��� �θ� ������ ��������� ȸ��
	void RotateLocal(float angleX, float angleY, float angleZ);
	void RotateLocal(const Vector3 &angle);


	//������� �̿��� Ư�� ȸ�������� ȸ������ ������....
	void SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	// ȸ�� ����� �־��ָ� �� ȸ�� ��Ĵ�� ȸ���Ѵ�.
	void SetRotateWorld(const Matrix& matWorldRotate);
	void SetRotateLocal(const Matrix& matWorldRotate);

	// ȸ�� ������� �־��ָ� �� ȸ���� ��� ȸ���Ѵ�.
	void SetRotateWorld(const Quaternion& matWorldRotate);
	void SetRotateLocal(const Quaternion& matWorldRotate);

	//Ư�� ������ �ٶ󺸰� ȸ���ض�.
	void LookDirection(const Vector3 &dir, const Vector3 &up = Vector3(0, 1, 0));

	//Ư�� ������ �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
	void LookDirection(const Vector3 &dir, float angle);

	//Ư����ġ�� �ٶ󺸰� ȸ���ض�.
	void LookPosition(const Vector3 &pos, const Vector3 &up = Vector3(0, 1, 0));

	//Ư����ġ��  �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
	void LookPosition(const Vector3 &pos, float angle);

	// �ڽ��� ȸ�� ���� from �� to ������ ȸ������ŭ ȸ������(���麸��) �Ͽ� ����
	void RotateSlerp(const TransformComponent& from, const TransformComponent& to, float t);

	// �ڽ��� ��ġ ���� from �� to ������ ��ġ��ŭ �������� �Ͽ� ����
	void PositionLerp(const TransformComponent& from, const TransformComponent& to, float t);

	// �ڽ��� ��� ������ from �� to ������ ������ŭ ���� �Ͽ� ����
	void Interpolate(const TransformComponent& from, const TransformComponent& to, float t);

	/////// update/////
	//void UpdateTransform();
	//void SetDeviceWorld(LPDIRECT3DDEVICE9 pDevie);
	//void SetDeviceView(LPDIRECT3DDEVICE9 pDevice);
	//void RenderGizmo(bool applyScale = false);

	///// Get  //////

	//���� ����� ��´�.
	Matrix GetFinalMatrix() const;
	Matrix GetWorldRotateMatrix() const;
	Quaternion GetWorldRotateQuaternion() const;

	//��ġ ���� ��´�.
	Vector3 GetWorldPosition() const;
	Vector3 GetLocalPosition() const;

	//���� ��´�. ( ���� ���� )
	void GetScaledAxies(Vector3* pVecArr) const;
	void GetUnitAxies(Vector3* pVecArr) const;
	Vector3 GetScaledAxis(int axisNum) const;
	Vector3 GetUnitAxis(int axisNum) const;
	Vector3 GetForward(bool bNormalize = true) const;
	Vector3 GetUp(bool bNormalize = true) const;
	Vector3 GetRight(bool bNormalize = true) const;

	Vector3 GetScale() const { return this->_scale; }

	//�ɹ� ������

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
