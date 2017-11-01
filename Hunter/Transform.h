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

	//��ġ
	D3DXVECTOR3 _position;

	//������
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

	//��ġ�� ���� ��ǥ��� �����Ѵ�. 
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(D3DXVECTOR3 pos);


	//��ġ�� ���� ��ǥ��� �����Ѵ�.  ( �θ� �ִ� ��� ������ �������� ��ġ )
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(D3DXVECTOR3 pos);


	//�ڽ��� ��������� �̵� ��Ų��.
	void MovePositionSelf(float dx, float dy, float dz);
	void MovePositionSelf(D3DXVECTOR3 delta);

	//���� �������� �̵� ��Ų��.
	void MovePositionWorld(float dx, float dy, float dz);
	void MovePositionWorld(D3DXVECTOR3 delta);

	//�θ� �ִ� ��� ���� �������� �̵� ��Ų��.
	void MovePositionLocal(float dx, float dy, float dz);
	void MovePositionLocal(D3DXVECTOR3 delta);



	//������ ���� 
	void SetScale(float x, float y, float z);
	void SetScale(D3DXVECTOR3 scale);

	//�����ϸ�
	void Scaling(float dx, float dy, float dz);
	void Scaling(D3DXVECTOR3 deltaScale);

	//���� �������� ȸ�� 
	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(D3DXVECTOR3 angle);

	//�ڽ��� ��������� ȸ��
	void RotateSelf(float angleX, float angleY, float angleZ);
	void RotateSelf(D3DXVECTOR3 angle);


	//�θ� �ִ� ��� �θ� ������ ��������� ȸ��
	void RotateLocal(float angleX, float angleY, float angleZ);
	void RotateLocal(D3DXVECTOR3 angle);


	//������� �̿��� Ư�� ȸ�������� ȸ������ ������....
	void SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	// ȸ�� ����� �־��ָ� �� ȸ�� ��Ĵ�� ȸ���Ѵ�.
	void SetRotateWorld(const D3DXMATRIXA16& matWorldRotate);
	void SetRotateLocal(const D3DXMATRIXA16& matWorldRotate);

	// ȸ�� ������� �־��ָ� �� ȸ���� ��� ȸ���Ѵ�.
	void SetRotateWorld(const D3DXQUATERNION& matWorldRotate);
	void SetRotateLocal(const D3DXQUATERNION& matWorldRotate);

	//Ư�� ������ �ٶ󺸰� ȸ���ض�.
	void LookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	//Ư�� ������ �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
	void LookDirection(D3DXVECTOR3 dir, float angle);

	//Ư����ġ�� �ٶ󺸰� ȸ���ض�.
	void LookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	//Ư����ġ��  �ٶ󺸴µ� angle ����ŭ�� ȸ�� �ض�
	void LookPosition(D3DXVECTOR3 pos, float angle);

	// �ڽ��� ȸ�� ���� from �� to ������ ȸ������ŭ ȸ������(���麸��) �Ͽ� ����
	void RotateSlerp(const Transform& from, const Transform& to, float t);

	// �ڽ��� ��ġ ���� from �� to ������ ��ġ��ŭ �������� �Ͽ� ����
	void PositionLerp(const Transform& from, const Transform& to, float t);

	// �ڽ��� ��� ������ from �� to ������ ������ŭ ���� �Ͽ� ����
	void Interpolate(const Transform& from, const Transform& to, float t);

	///// update/////

	void UpdateTransform();

	void SetDeviceWorld(LPDIRECT3DDEVICE9 pDevie);

	void SetDeviceView(LPDIRECT3DDEVICE9 pDevice);

	void RenderGizmo(bool applyScale = false);

	//void DefaultControl(float timeDelta);
	//void DefaultControl2(float timeDelta);


	///// Get  //////

	//���� ����� ��´�.
	D3DXMATRIXA16 GetFinalMatrix() const;
	D3DXMATRIXA16 GetWorldRotateMatrix() const;
	D3DXQUATERNION GetWorldRotateQuaternion() const;

	//��ġ ���� ��´�.
	D3DXVECTOR3 GetWorldPosition() const;
	D3DXVECTOR3 GetLocalPosition() const;

	//���� ��´�. ( ���� ���� )
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
