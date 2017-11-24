#include "stdafx.h"
#include "CollisionUtils.h"


#define EPSILON 0.001f
#define FLOATEQUAL(a,b) ((abs((a)-(b)) )< EPSILON)

bool Time_RayToOBB(Vector3 rayPos, Vector3 rayDir, Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec,
	float xVecSize, float yVecSize, float zVecSize)
{

	//���̿��� OBB�ڽ� �߽����� ���� ���͸� ���Ѵ�
	Vector3 vec1 = OBBPos - rayPos;

	//OBB�ڽ��� ���࿡ ������ ���⺤�͸� �������� float3�� �����Ѵ�
	Vector3 vec2 = Vector3(
		Vec3Dot(&xVec, &rayDir),
		Vec3Dot(&yVec, &rayDir),
		Vec3Dot(&zVec, &rayDir));

	//vec1�� ���������� �������� �������� �����Ѵ�
	Vector3 vec3 = Vector3
	(
		Vec3Dot(&xVec, &vec1),
		Vec3Dot(&yVec, &vec1),
		Vec3Dot(&zVec, &vec1));

	//x���� �ִ�� �ּҸ� ã�´�!!
	float xMax = 0.0f;
	float xMin = 0.0f;
	//���� ������ ������ ��� �����ϰ�(�����Ѱ��� 0�� ���Դ�), OBB�ڽ��� ������� Pos�� x�Ÿ��� ũ�ٸ� �浹�� �� ����.
	if (absFloat(vec2.x) < EPSILON)
	{
		if (vec3.x - xVecSize > 0 || vec3.x + xVecSize < 0)
		{
			return -1;
		}
		//�������� 0�̸� ���������Ƿ�~ �ּҰ��� ��
		vec2.x = EPSILON;
	}
	xMin = (vec3.x + xVecSize) / vec2.x;
	xMax = (vec3.x - xVecSize) / vec2.x;


	//y���� �ִ�� �ּҸ� ã�´�!!
	float yMax = 0.0f;
	float yMin = 0.0f;
	//���� ������ ������ ��� �����ϰ�(�����Ѱ��� 0�� ���Դ�), OBB�ڽ��� ������� Pos�� x�Ÿ��� ũ�ٸ� �浹�� �� ����.
	if (absFloat(vec2.y) < EPSILON)
	{
		if (vec3.y - yVecSize > 0 || vec3.y + yVecSize < 0)
		{
			return -1;
		}
		//�������� 0�̸� ���������Ƿ�~ �ּҰ��� ��
		vec2.y = EPSILON;
	}
	yMin = (vec3.y + yVecSize) / vec2.y;
	yMax = (vec3.y - yVecSize) / vec2.y;


	//z���� �ִ�� �ּҸ� ã�´�!!
	float zMax = 0.0f;
	float zMin = 0.0f;
	//���� ������ ������ ��� �����ϰ�(�����Ѱ��� 0�� ���Դ�), OBB�ڽ��� ������� Pos�� x�Ÿ��� ũ�ٸ� �浹�� �� ����.
	if (absFloat(vec2.z) < EPSILON)
	{
		if (vec3.z - zVecSize > 0 || vec3.z + zVecSize < 0)
		{
			return -1;
		}
		//�������� 0�̸� ���������Ƿ�~ �ּҰ��� ��
		vec2.z = EPSILON;
	}
	zMin = (vec3.z + zVecSize) / vec2.z;
	zMax = (vec3.z - zVecSize) / vec2.z;

	//���³��� �ּ�/�ִ밪���� ���� �ּ�/�ִ밪���� ���Ѵ�
	float FinalMax = 0.0f;
	float FinalMin = 0.0f;

	FinalMin = fminf(
		fminf(
			fminf(xMin, xMax),
			fmin(yMin, yMax)
		)
		,
		fminf(zMin, zMax)
	);

	FinalMax = fmaxf(
		fmaxf(
			fmaxf(xMin, xMax),
			fmaxf(yMin, yMax)
		)
		,
		fmaxf(zMin, zMax)
	);

	//���� ���������� 0���� �۴ٸ�, ������ �ƿ� �ݴ�������� ������� �����Ƿ� �浹�Ұ���
	if (FinalMax < 0) return -1;

	//�ּҰ� �ִ뺸�� ũ�ٸ�? �浹���� �ʾҴ�
	if (FinalMin > FinalMax)return -1;


	//�ּҰ��� 0���� �۴ٸ� ������ �ڽ��ȿ��� ����Ѱ� -> �浹�ѽð��� �ִ밪
	if (FinalMin < 0.0f)
		return FinalMax;

	return FinalMin;
}

bool Time_RayToOBB(const Vector3 & rayPos, Vector3 & rayDir, Vector3 & OBBPos, Vector3 & xVec, Vector3 & yVec, Vector3 & zVec, float xVecSize, float yVecSize, float zVecSize)
{
	return false;
}

bool IsBlocking(TransformComponent * pTransA, CollisionComponent::BoundingBox * pBoundA, TransformComponent * pTransB, CollisionComponent::BoundingBox * pBoundB, float moveFactor)
{
	//���� �浹���� �ʾ����� ���ʿ����
	if (Collision_AABBToAABB(pBoundA->_localMinPos, pBoundA->_localMaxPos,pBoundB->_localMinPos,pBoundB->_localMaxPos) == false)
		return false;

	moveFactor = Clamp01(moveFactor);

	//���� �ε��ƽ��� �о���Ѵ�...

	//A�� Min Max
	Vector3 minA = pBoundA->_localMinPos;
	Vector3 maxA = pBoundA->_localMaxPos;

	//B�� Min Max
	Vector3 minB = pBoundB->_localMinPos;
	Vector3 maxB = pBoundB->_localMaxPos;

	//B �� ���� �簢 8 ��

	//       5-------6 Max
	//      /|      /|
	//     1-------2 |
	//     | 4-----|-7
	//     |/      |/
	// Min 0-------3

	Vector3 pos[8];
	pos[0] = Vector3(minB.x, minB.y, minB.z);
	pos[1] = Vector3(minB.x, maxB.y, minB.z);
	pos[2] = Vector3(maxB.x, maxB.y, minB.z);
	pos[3] = Vector3(maxB.x, minB.y, minB.z);
	pos[4] = Vector3(minB.x, minB.y, maxB.z);
	pos[5] = Vector3(minB.x, maxB.y, maxB.z);
	pos[6] = Vector3(maxB.x, maxB.y, maxB.z);
	pos[7] = Vector3(maxB.x, minB.y, maxB.z);


	//A �� ���� �����
	Matrix matWorldAInv;
	MatrixInverse(&matWorldAInv, NULL, &pTransA->GetFinalMatrix());

	//B �� ���� ���
	Matrix matWorldB = pTransB->GetFinalMatrix();

	//B ���� ��ŭ ���� A �� ������ �ٽ� ������ ���
	Matrix mat = matWorldB * matWorldAInv;

	//B pos �� ����
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	//������ Pos ���� A ���� B �� ������� ��ġ�����̵ȴ�.


	//���������� ����� B pos �� ������ min max �� ���� ����
	minB = pos[0];
	maxB = pos[0];
	for (int i = 1; i < 8; i++) {
		if (pos[i].x < minB.x) minB.x = pos[i].x;
		if (pos[i].y < minB.y) minB.y = pos[i].y;
		if (pos[i].z < minB.z) minB.z = pos[i].z;
		if (pos[i].x > maxB.x) maxB.x = pos[i].x;
		if (pos[i].y > maxB.y) maxB.y = pos[i].y;
		if (pos[i].z > maxB.z) maxB.z = pos[i].z;
	}

	//�簢 �� ����ü
	struct fRect {
		float left;
		float right;
		float bottom;
		float top;
		float back;
		float front;
	};

	fRect rcA = { minA.x, maxA.x, minA.y, maxA.y, minA.z, maxA.z };
	fRect rcB = { minB.x, maxB.x, minB.y, maxB.y, minB.z, maxB.z };

	//��Ĩ���� �簢��
	fRect rcInter;
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);


	//������ ��Ĩ���� ���ϰ� ���� ���� ���� ������ B �̵���Ų��.
	float interX = rcInter.right - rcInter.left;
	float interY = rcInter.top - rcInter.bottom;
	float interZ = rcInter.front - rcInter.back;
	float minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	//�̴� ����
	D3DXVECTOR3 moveDirA(0, 0, 0);

	//�з�
	float moveLengthA = minInter;

	//X ���� ��Ĩ���� ���� �۴ٸ�..
	if (minInter == interX)
	{
		//A �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.left, rcA.left))
			moveDirA = -pTransA->GetRight();

		//A �� ���������� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.right, rcA.right))
			moveDirA = pTransA->GetRight();
	}
	//Y ���� ��Ĩ���� ���� �۴ٸ�..
	else if (minInter == interY)
	{
		//A �� ������ �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.top, rcA.top))
			moveDirA = pTransA->GetUp();

		//A �� �Ʒ����� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.bottom, rcA.bottom))
			moveDirA = -pTransA->GetUp();

	}

	//Z ���� ��ħ���� ���� �۴ٸ�..
	else if (minInter == interZ)
	{
		//A �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.front, rcA.front))
			moveDirA = pTransA->GetForward();

		//A �� ������ �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.back, rcA.back))
			moveDirA = -pTransA->GetForward();
	}

	//��������´ٸ� �з��� �̴� ������ ���ȴ�.
	//moveDirA
	//moveLengthA
	//���� �ΰ��� A �� ������ �ΰ� B �� A ������ ����� ���̵ȴ�.

	//
	// �Ʒ��� ������ B �� ������ �ΰ� A �� B ������ ����� ���̵ȴ�.
	//
	D3DXVECTOR3 moveDirB(0, 0, 0);
	float moveLengthB = 0.0f;

	//A�� Min Max
	minA = pBoundA->_localMinPos;
	maxA = pBoundA->_localMaxPos;

	//B�� Min Max
	minB = pBoundB->_localMinPos;
	maxB = pBoundB->_localMaxPos;

	//B �� ���� �簢 8 ��

	//       5-------6 Max
	//      /|      /|
	//     1-------2 |
	//     | 4-----|-7
	//     |/      |/
	// Min 0-------3

	pos[0] = Vector3(minA.x, minA.y, minA.z);
	pos[1] = Vector3(minA.x, maxA.y, minA.z);
	pos[2] = Vector3(maxA.x, maxA.y, minA.z);
	pos[3] = Vector3(maxA.x, minA.y, minA.z);
	pos[4] = Vector3(minA.x, minA.y, maxA.z);
	pos[5] = Vector3(minA.x, maxA.y, maxA.z);
	pos[6] = Vector3(maxA.x, maxA.y, maxA.z);
	pos[7] = Vector3(maxA.x, minA.y, maxA.z);

	//B �� ���� �����
	Matrix matWorldBInv;
	D3DXMatrixInverse(&matWorldBInv, NULL, &pTransB->GetFinalMatrix());

	//A �� ���� ���
	D3DXMATRIXA16 matWorldA = pTransA->GetFinalMatrix();

	//A ���� ��ŭ ���� B �� ������ �ٽ� ������ ���
	mat = matWorldA * matWorldBInv;

	//A pos �� ����
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	//���������� ����� A pos �� ������ min max �� ���� ����
	minA = pos[0];
	maxA = pos[0];
	for (int i = 1; i < 8; i++) {
		if (pos[i].x < minA.x) minA.x = pos[i].x;
		if (pos[i].y < minA.y) minA.y = pos[i].y;
		if (pos[i].z < minA.z) minA.z = pos[i].z;
		if (pos[i].x > maxA.x) maxA.x = pos[i].x;
		if (pos[i].y > maxA.y) maxA.y = pos[i].y;
		if (pos[i].z > maxA.z) maxA.z = pos[i].z;
	}
	//rcA = { minA.x, maxA.x, minA.y, maxA.y, minA.z, maxA.z };
	//rcB = { minB.x, maxB.x, minB.y, maxB.y, minB.z, maxB.z };
	rcA.left = minA.x;		rcA.right = maxA.x;			rcA.bottom = minA.y;			rcA.top = maxA.y;			rcA.back = minA.z;			rcA.front = maxA.z;
	rcB.left = minB.x;		rcB.right = maxB.x;			rcB.bottom = minB.y;			rcB.top = maxB.y;			rcB.back = minB.z;			rcB.front = maxB.z;

	//��Ĩ���� �簢��
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);

	//������ ��Ĩ���� ���ϰ� ���� ���� ���� ������ B �̵���Ų��.
	interX = rcInter.right - rcInter.left;
	interY = rcInter.top - rcInter.bottom;
	interZ = rcInter.front - rcInter.back;
	minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	moveLengthB = minInter;

	//X ���� ��Ĩ���� ���� �۴ٸ�..
	if (minInter == interX)
	{
		//B �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.left, rcB.left))
			moveDirB = -pTransB->GetRight();

		//B �� ���������� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.right, rcB.right))
			moveDirB = pTransB->GetRight();
	}
	//Y ���� ��Ĩ���� ���� �۴ٸ�..
	else if (minInter == interY)
	{
		//B �� ������ �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.top, rcB.top))
			moveDirB = pTransB->GetUp();

		//B �� �Ʒ����� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.bottom, rcB.bottom))
			moveDirB = -pTransB->GetUp();

	}

	//Z ���� ��ħ���� ���� �۴ٸ�..
	else if (minInter == interZ)
	{
		//B �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.front, rcB.front))
			moveDirB = pTransB->GetForward();

		//B �� ������ �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.back, rcB.back))
			moveDirB = -pTransB->GetForward();
	}


	//��������´ٸ� �Ʒ��� 4 ���� ������ ���� ���̴�....
	//D3DXVECTOR3 moveDirA( 0, 0, 0 );
	//float moveLengthA = minInter;
	//D3DXVECTOR3 moveDirB( 0, 0, 0 );
	//float moveLengthB = 0.0f;


	//�з��� ����������...
	if (moveLengthB > moveLengthA)
	{
		//A �� B �� ������ ����
		//���� A �� ������ �ΰ� B �� �ǵ��ΰŴ�
		//���� A �� �������� 2 ���� B �� ������ �� 1 �̿��ٰ� �����Ͽ��� ��
		//A �� ����ķ� ���Ǿ� A �� �������� 1 B �� �������� 0.5 �� ���� �����̴�
		//�׷��Ƿ� �� ���� ���̴�
		//Ascale ���� ���Ͽ� ����ϴ� ���� �ùٸ���.
		D3DXVECTOR3 scaleA = pTransA->GetScale();
		D3DXVECTOR3 scaleB = pTransB->GetScale();

		pTransB->MovePositionWorld(
			moveDirA.x * moveLengthA * (1.0 - moveFactor) * scaleA.x,
			moveDirA.y * moveLengthA * (1.0 - moveFactor) * scaleA.y,
			moveDirA.z * moveLengthA * (1.0 - moveFactor) * scaleA.z);


		pTransA->MovePositionWorld(
			-moveDirA.x * moveLengthA * moveFactor * scaleA.x,
			-moveDirA.y * moveLengthA * moveFactor * scaleA.y,
			-moveDirA.z * moveLengthA * moveFactor * scaleA.z);


	}


	else
	{
		//A �� B �� ������ ����
		//���� B �� ������ �ΰ� A �� �ǵ��ΰŴ� 
		//���� A �� �������� 2 ���� B �� ������ �� 1 �̿��ٰ� �����Ͽ�����
		//B �� ����ķ� ���Ǿ� A �� �������� 2 B �� �������� 1 �� ���� �����̴�
		//�׷��Ƿ� �� ���� ���̴� BScale ���� ���Ͽ� ����ϴ� ���� �ùٸ���.

		D3DXVECTOR3 scaleA = pTransA->GetScale();
		D3DXVECTOR3 scaleB = pTransB->GetScale();

		pTransA->MovePositionWorld(
			moveDirB.x * moveLengthB * moveFactor  * scaleB.x,
			moveDirB.y * moveLengthB * moveFactor  * scaleB.y,
			moveDirB.z * moveLengthB * moveFactor  * scaleB.z);

		pTransB->MovePositionWorld(
			-moveDirB.x * moveLengthB * (1.0 - moveFactor) *scaleB.x,
			-moveDirB.y * moveLengthB * (1.0 - moveFactor) *scaleB.y,
			-moveDirB.z * moveLengthB * (1.0 - moveFactor) *scaleB.z);
	}


	return true;

}

bool IsRayHitBound(const Ray & ray, const CollisionComponent::BoundingSphere & boundingSphere, 
	const TransformComponent & transform, Vector3 * pOutHitPos, Vector3 * pOutHitNormal)
{
	Matrix matWorld = transform.GetFinalMatrix();

	Vector3 scale = transform.GetScale();

	Vector3 center;
	float radius;
	Vector3 halfSize;

	//��ȯ�� ��ġ
	Vec3TransformCoord(&center, &boundingSphere._localCenter, &matWorld);

	//������...
	halfSize.x = boundingSphere._radius  * scale.x;
	halfSize.y = boundingSphere._radius * scale.y;
	halfSize.z = boundingSphere._radius * scale.z;
	radius = Vec3Length(&halfSize);

	//������ ���������� ���� ���� ���ͱ����� ���⺤��
	Vector3 dirToCenter = center - ray.origin;

	//������ ����
	float lengthSq = Vec3LengthSq(&dirToCenter);

	//�������� ����
	float r2 = radius * radius;

	//���� ������ ���ȿ� �ִٸ�..
	if (r2 > lengthSq)
	{
		//������ �ȿ��� ������ ���� üũ �ȵȴ�.
		return false;
	}

	//����������� �������� ���ۿ� �ִٴ¿���
	//�����ͱ����� ���⺤�Ϳ� ������ ���⺤�Ͱ� ������ ������ 
	//�а��̶�� �׾��ٱ���� �浹���Ͼ���
	float dot = Vec3Dot(&dirToCenter, &ray.direction);
	if (dot <= 0.0f)
	{
		return false;
	}

	// ��Ÿ����� ������ �ϱ����� ���� �ﰢ�� ��������
	// d2 = x2 + y2;
	// d = sqrt( x2 + y2 );

	float x2 = dot * dot;
	float d2 = lengthSq;
	//d2 - x2 = y2;
	float y2 = d2 - x2;

	//������ ������ �����.
	if (y2 > r2) 
	{
		return false;
	}

	//��������´ٸ� �ϴ��� ��Ʈ
	//���� ��ư��߾� HitPoint ���ִٸ�..
	if (pOutHitPos != nullptr)
	{
		//d�� raius ����
		d2 = r2;
		//d2 = y2 + x2
		//float x2 = d2 - y2;
		float x = sqrt(d2 - y2);

		//
		*pOutHitPos = ray.origin + (ray.direction * (dot - x));

		//Hit �� ��ġ�� �븻�� ��ڴٸ�..
		if (pOutHitNormal)
		{
			*pOutHitNormal = *pOutHitPos - center;
			Vec3Normalize(pOutHitNormal, pOutHitNormal);
		}
	}

	return true;
}

Vector3 GetSphereClosetVector(const Vector3 & SpherePos, const Vector3 & point, float radius)
{
	Vector3 toPoint;
	toPoint = point - SpherePos;
	Vec3Normalize(&toPoint, &toPoint);
	Vector3 closetVec = toPoint*radius;
	return (closetVec + SpherePos);
}

Vector3 GetAABBClosetVector(const Vector3 & MinPoint, const Vector3 & MaxPoint, const Vector3 & point)
{
	Vector3 closetVec = point;

	(point.x > MinPoint.x) ? closetVec.x = point.x : closetVec.x = MinPoint.x;
	(point.y > MinPoint.y) ? closetVec.y = point.y : closetVec.y = MinPoint.y;
	(point.z > MinPoint.z) ? closetVec.z = point.z : closetVec.z = MinPoint.z;

	(point.x < MaxPoint.x) ? closetVec.x = point.x : closetVec.x = MaxPoint.x;
	(point.y < MaxPoint.y) ? closetVec.y = point.y : closetVec.y = MaxPoint.y;
	(point.z < MaxPoint.z) ? closetVec.z = point.z : closetVec.z = MaxPoint.z;

	return closetVec;
}

Vector3 GetOBBClosetVector(const Vector3 & OBBPos, const Vector3 & xVec, const Vector3 & yVec, const Vector3 & zVec, float xVecSize, float yVecSize, float zVecSize, const Vector3 & point)
{
		Vector3 dir = point - OBBPos;
	Vector3 result = OBBPos;
	//x�� ����
	float distance = Vec3Dot(&dir, &xVec);

	if (distance > xVecSize)
	{
		distance = xVecSize;
	}
	if (distance < -xVecSize)
	{
		distance = -xVecSize;
	}

	result = result + (xVec*distance);

	//y�� ����
	distance = Vec3Dot(&dir, &yVec);

	if (distance > yVecSize)
	{
		distance = yVecSize;
	}
	if (distance < -yVecSize)
	{
		distance = -yVecSize;
	}

	result = result + (yVec*distance);

	//z�� ����
	distance = Vec3Dot(&dir, &zVec);

	if (distance > zVecSize)
	{
		distance = zVecSize;
	}
	if (distance < -zVecSize)
	{
		distance = -zVecSize;
	}

	result = result + (zVec*distance);

	return result;
}

Vector3 GetPlaneClosetVector(const Vector3 & planeNormal, float planeDistance, const Vector3 & point)
{
	float dot = Vec3Dot(&planeNormal, &point);
	float distance = dot - planeDistance;
	return point - planeNormal*distance;
}

Vector3 GetLineClosetVector(const Vector3 & startPoint, const Vector3 & endPoint, const Vector3 & point)
{
	Vector3 lineVec = endPoint - startPoint;
	Vector3 lineToPoint = point - startPoint;
	float t = Vec3Dot(&lineVec, &lineToPoint) / Vec3Dot(&lineVec, &lineVec);
	if (t < 0) t = 0;
	if (t > 1)t = 1;
	return startPoint + lineVec*t;
}

Vector3 GetRayClosetVector(const Vector3 & rayOrigin, const Vector3 & rayDirection, const Vector3 & point)
{
	Vector3 rayToPoint = point - rayOrigin;
	float t = Vec3Dot(&rayToPoint, &rayDirection);
	if (t < 0) t = 0;
	return rayOrigin + rayDirection*t;
}

bool Collision_SphereToSphere(const Vector3 & spherePos1, const Vector3 & spherePos2, float radius1, float radius2)
{
	Vector3 dis = spherePos2 - spherePos1;
	float distance = Vec3Length(&dis);
	return (distance < radius1 + radius2);
}

bool Collision_SphereToAABB(const Vector3 & MinPoint, const Vector3 & MaxPoint, const Vector3 & spherePos, float radius)
{
	Vector3 closetVec = GetAABBClosetVector(MinPoint, MaxPoint, spherePos);

	Vector3 dis = closetVec - spherePos;

	float distance = Vec3Length(&dis);

	return(distance < radius);
}

bool Collision_SphereToOBB(const Vector3 & OBBPos, const Vector3 & xVec, const Vector3 & yVec, const Vector3 & zVec, float xVecSize, float yVecSize, float zVecSize, const Vector3 & spherePos, float radius)
{
	Vector3 closetVec = GetOBBClosetVector(OBBPos, xVec, yVec, zVec, xVecSize, yVecSize, zVecSize, spherePos);

	Vector3 dis = closetVec - spherePos;

	float distance = Vec3Length(&dis);

	return (distance < radius);
}

bool Collision_SphereToPlane(const Vector3 & planeNormal, float planeDistance, const Vector3 & spherePos, float radius)
{
	Vector3 closetVec = GetPlaneClosetVector(planeNormal, planeDistance, spherePos);

	Vector3 dis = closetVec - spherePos;

	float distance = Vec3Length(&dis);

	return (distance < radius);

}

bool Collision_AABBToAABB(const Vector3 & minPoint1, const Vector3 & maxPoint1, const Vector3 & minPoint2, const Vector3 & maxPoint2)
{
	if ((minPoint1.x <= maxPoint2.x&&maxPoint1.x >= minPoint2.x) && (minPoint1.y <= maxPoint2.y&&maxPoint1.y >= minPoint2.y) &&
		(minPoint1.z <= maxPoint2.z&&maxPoint1.z >= minPoint2.z))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision_AABBToOBB(const Vector3 & minPoint, const Vector3 & maxPoint, const Vector3 & OBBPos, const Vector3 & xVec, const Vector3 & yVec, const Vector3 & zVec, float xVecSize, float yVecSize, float zVecSize)
{
	
	// 1. 15���� �ʿ��� ���� ���Ѵ�
	Vector3 axis[15] =
	{
		Vector3(1,0,0),
		Vector3(0,1,0),
		Vector3(0,1,1),
		xVec,
		yVec,
		zVec
	};
	Vec3Cross(&axis[6], &xVec, &axis[0]);
	Vec3Cross(&axis[7], &yVec, &axis[0]);
	Vec3Cross(&axis[8], &zVec, &axis[0]);

	Vec3Cross(&axis[9], &xVec, &axis[1]);
	Vec3Cross(&axis[10], &yVec, &axis[1]);
	Vec3Cross(&axis[11], &zVec, &axis[1]);

	Vec3Cross(&axis[12], &xVec, &axis[2]);
	Vec3Cross(&axis[13], &yVec, &axis[2]);
	Vec3Cross(&axis[14], &zVec, &axis[2]);

	for (int j = 0; j < 15; j++)
	{

		// 2 . AABB�ڽ��� 8���� vertex ���Ͱ� �࿡ projection ���� �� �ּұ��̿� �ִ���̸� ���Ѵ�
		Vector3 vertex[8] =
		{
			Vector3(minPoint.x,minPoint.y,minPoint.z),
			Vector3(maxPoint.x,minPoint.y,minPoint.z),
			Vector3(minPoint.x,maxPoint.y,minPoint.z),
			Vector3(minPoint.x,minPoint.y,maxPoint.z),
			Vector3(maxPoint.x,maxPoint.y,minPoint.z),
			Vector3(maxPoint.x,minPoint.y,maxPoint.z),
			Vector3(minPoint.x,maxPoint.y,maxPoint.z),
			Vector3(maxPoint.x,maxPoint.y,maxPoint.z)
		};
		float min, max;
		min = max = Vec3Dot(&vertex[0], &axis[j]);
		for (int i = 1; i < 8; i++)
		{
			float project = Vec3Dot(&vertex[i], &axis[j]);
			min = (project < min) ? project : min;
			max = (project > max) ? project : max;
		}

		// 3. OBB�ڽ��� 8���� vertex ���Ͱ� �࿡ projecttion ���� �� �ּұ��̿� �ִ���̸� ���Ѵ�
		Vector3 vertex2[8] =
		{
			OBBPos + xVec*xVecSize + yVec*yVecSize + zVec*zVecSize,
			OBBPos - xVec*xVecSize + yVec*yVecSize + zVec*zVecSize,
			OBBPos + xVec*xVecSize - yVec*yVecSize + zVec*zVecSize,
			OBBPos + xVec*xVecSize + yVec*yVecSize - zVec*zVecSize,
			OBBPos - xVec*xVecSize - yVec*yVecSize + zVec*zVecSize,
			OBBPos - xVec*xVecSize + yVec*yVecSize - zVec*zVecSize,
			OBBPos + xVec*xVecSize - yVec*yVecSize - zVec*zVecSize,
			OBBPos - xVec*xVecSize - yVec*yVecSize - zVec*zVecSize
		};
		float min2, max2;
		min2 = max2 = Vec3Dot(&vertex2[0], &axis[j]);
		for (int i = 1; i < 8; i++)
		{
			float project = Vec3Dot(&vertex2[i], &axis[j]);
			min2 = (project < min2) ? project : min2;
			max2 = (project > max2) ? project : max2;
		}
		// 4. AABB�ڽ��� �ּ�~�ִ� �� OBB�ڽ��� �ּ�~�ִ븦 ���ؼ� ���� ���ϴ� ���������� ã�´�
		if (max >= min2&&min <= max2)
		{
			continue;
		}
		else
		{
			return false;
		}
		// 5. 15�� �ݺ��ؼ� ���� ���Ѵٸ� �浹�̴�
	}
	return true;
}

bool Collision_AABBtoPlane(const Vector3 & minPoint, const Vector3 & maxPoint, const Vector3 & planeNormal, float planeDistance)
{
	float pLen =
		(maxPoint.x - minPoint.x)*(planeNormal.x) + (maxPoint.y - minPoint.y)*absFloat(planeNormal.y) + (maxPoint.z - minPoint.z)*absFloat(planeNormal.z);

	Vector3 AABBPos = Vector3((minPoint.x + maxPoint.x) / 2, (minPoint.y + maxPoint.y) / 2, (minPoint.z + maxPoint.z) / 2);

	float dot = Vec3Dot(&planeNormal, &AABBPos);

	float dist = dot - planeDistance;

	return (absFloat(dist) <= pLen);
}

bool Collision_OBBToOBB(const Vector3 & OBBPos1, const Vector3 & xVec1, const Vector3 & yVec1, const Vector3 & zVec1, float xVecSize1, float yVecSize1, float zVecSize1, const Vector3 & OBBPos2, const Vector3 & xVec2, const Vector3 & yVec2, const Vector3 & zVec2, float xVecSize2, float yVecSize2, float zVecSize2)
{
	// 1. 15���� �ʿ��� ���� ���Ѵ�
	Vector3 axis[15] =
	{
		xVec1,
		yVec1,
		zVec1,
		xVec2,
		yVec2,
		zVec2
	};
	Vec3Cross(&axis[6], &xVec2, &axis[0]);
	Vec3Cross(&axis[7], &yVec2, &axis[0]);
	Vec3Cross(&axis[8], &zVec2, &axis[0]);

	Vec3Cross(&axis[9], &xVec2, &axis[1]);
	Vec3Cross(&axis[10], &yVec2, &axis[1]);
	Vec3Cross(&axis[11], &zVec2, &axis[1]);

	Vec3Cross(&axis[12], &xVec2, &axis[2]);
	Vec3Cross(&axis[13], &yVec2, &axis[2]);
	Vec3Cross(&axis[14], &zVec2, &axis[2]);

	for (int j = 0; j < 15; j++)
	{

		// 2 . AABB�ڽ��� 8���� vertex ���Ͱ� �࿡ projection ���� �� �ּұ��̿� �ִ���̸� ���Ѵ�
		Vector3 vertex[8] =
		{
			OBBPos1 + xVec1*xVecSize1 + yVec1*yVecSize1 + zVec1*zVecSize1,
			OBBPos1 - xVec1*xVecSize1 + yVec1*yVecSize1 + zVec1*zVecSize1,
			OBBPos1 + xVec1*xVecSize1 - yVec1*yVecSize1 + zVec1*zVecSize1,
			OBBPos1 + xVec1*xVecSize1 + yVec1*yVecSize1 - zVec1*zVecSize1,
			OBBPos1 - xVec1*xVecSize1 - yVec1*yVecSize1 + zVec1*zVecSize1,
			OBBPos1 - xVec1*xVecSize1 + yVec1*yVecSize1 - zVec1*zVecSize1,
			OBBPos1 + xVec1*xVecSize1 - yVec1*yVecSize1 - zVec1*zVecSize1,
			OBBPos1 - xVec1*xVecSize1 - yVec1*yVecSize1 - zVec1*zVecSize1
		};
		float min, max;
		min = max = Vec3Dot(&vertex[0], &axis[j]);
		for (int i = 1; i < 8; i++)
		{
			float project = Vec3Dot(&vertex[i], &axis[j]);
			min = (project < min) ? project : min;
			max = (project > max) ? project : max;
		}

		// 3. OBB�ڽ��� 8���� vertex ���Ͱ� �࿡ projecttion ���� �� �ּұ��̿� �ִ���̸� ���Ѵ�
		Vector3 vertex2[8] =
		{
			OBBPos2 + xVec2*xVecSize2 + yVec2*yVecSize2 + zVec2*zVecSize2,
			OBBPos2 - xVec2*xVecSize2 + yVec2*yVecSize2 + zVec2*zVecSize2,
			OBBPos2 + xVec2*xVecSize2 - yVec2*yVecSize2 + zVec2*zVecSize2,
			OBBPos2 + xVec2*xVecSize2 + yVec2*yVecSize2 - zVec2*zVecSize2,
			OBBPos2 - xVec2*xVecSize2 - yVec2*yVecSize2 + zVec2*zVecSize2,
			OBBPos2 - xVec2*xVecSize2 + yVec2*yVecSize2 - zVec2*zVecSize2,
			OBBPos2 + xVec2*xVecSize2 - yVec2*yVecSize2 - zVec2*zVecSize2,
			OBBPos2 - xVec2*xVecSize2 - yVec2*yVecSize2 - zVec2*zVecSize2
		};
		float min2, max2;
		min2 = max2 = Vec3Dot(&vertex2[0], &axis[j]);
		for (int i = 1; i < 8; i++)
		{
			float project = Vec3Dot(&vertex2[i], &axis[j]);
			min2 = (project < min2) ? project : min2;
			max2 = (project > max2) ? project : max2;
		}
		// 4. AABB�ڽ��� �ּ�~�ִ� �� OBB�ڽ��� �ּ�~�ִ븦 ���ؼ� ���� ���ϴ� ���������� ã�´�
		if (max >= min2&&min <= max2)
		{
			continue;
		}
		else
		{
			return false;
		}
		// 5. 15�� �ݺ��ؼ� ���� ���Ѵٸ� �浹�̴�
	}
	return true;
}

bool Collision_OBBToPlane(const Vector3 & OBBPos, const Vector3 & xVec, const Vector3 & yVec, const Vector3 & zVec, float xVecSize, float yVecSize, float zVecSize, const Vector3 & planeNormal, float planeDistance)
{
	float pLen = xVecSize*absFloat(Vec3Dot(&xVec, &planeNormal)) + yVecSize*absFloat(Vec3Dot(&yVec, &planeNormal)) + zVecSize*absFloat(Vec3Dot(&zVec, &planeNormal));

	float dot = Vec3Dot(&OBBPos, &planeNormal);
	float dist = dot - planeDistance;

	return absFloat(dist) <= pLen;
}

bool Collision_PlaneToPlane(const Vector3 & planeNormal1, float planeDistance1, const Vector3 planeNormal2, float planeDistance2)
{
	Vector3 vec;
	Vec3Cross(&vec, &planeNormal1, &planeNormal2);
	return (Vec3Dot(&vec, &vec) != 0);
}

bool Collision_RayToSphere(const Vector3 & rayPos, const Vector3 & rayDir, const Vector3 & spherePos, float radius)
{
	Vector3 e = spherePos - rayPos;

	float a = Vec3Dot(&e, &rayDir);
	float eSide = Vec3Length(&e); //esq


	float b = sqrtf(eSide*eSide - a*a);

	float f = sqrtf(radius*radius - b*b);

	//�浹��������?
	if (radius*radius - b*b < 0.0f) return false;
	else return true;
}

float Time_RayToSphere(const Vector3 & rayPos, const Vector3 & rayDir, const Vector3 & spherePos, float radius)
{
	Vector3 e = spherePos - rayPos;

	float a = Vec3Dot(&e, &rayDir);
	float eSide = Vec3Length(&e); //esq


	float b = eSide*eSide - a*a;

	float f = sqrtf(b - radius*radius);

	//�浹��������?
	if (radius*radius - b < 0.0f) return -1;
	else if (eSide*eSide < radius*radius) return 0;//���ȿ��� ������ �������
	else
	{
		return a - f;
	}
}

bool Collision_RayToAABB(const Vector3 & rayPos, const Vector3 & rayDir, const Vector3 & minPos, const Vector3 & maxPos)
{
//AABB�ڽ��� Ȯ���ؼ� �浹�� üũ�Ѵ�(direction���� ������ �����ʴ����� ���� 0
	float t1 = (minPos.x - rayPos.x) / rayDir.x;
	float t2 = (maxPos.x - rayPos.x) / rayDir.x;
	float t3 = (minPos.y - rayPos.y) / rayDir.y;
	float t4 = (maxPos.y - rayPos.y) / rayDir.y;
	float t5 = (minPos.z - rayPos.z) / rayDir.z;
	float t6 = (maxPos.z - rayPos.z) / rayDir.z;


	//�� t���� ���ؼ� �ּҰ��� �ִ�� �ִ밪�� �ּҸ� ���Ѵ�
	float finalMin = fmaxf(fmaxf(fmin(t1, t2), fmin(t3, t4)), fminf(t5, t6));
	float finalMax = fminf(fminf(fmax(t1, t2), fmax(t3, t4)), fmaxf(t5, t6));

	//������ max�� ������� ���� �ݴ��ʿ� �ִ� -> �浹��������
	if (finalMax < 0.0f) return false;

	//������ min�� ������ max�� ����ũ�� Ȯ��� ������ �����ؼ� �������� ->�Ʊ��� �浹���� ����
	if (finalMin > finalMax) return false;

	//���� ������ min�� �������(���� ������ max�� ���) ������ AABB�ȿ��� �߻�Ǿ��� -> ������ �浹
	if (finalMin< 0.0f) return true;

	return true;}

float Time_RayToAABB(const Vector3 & rayPos, const Vector3 & rayDir, const Vector3 & minPos, const Vector3 & maxPos)
{
//AABB�ڽ��� Ȯ���ؼ� �浹�� üũ�Ѵ�(direction���� ������ �����ʴ����� ���� 0
	float t1 = (minPos.x - rayPos.x) / rayDir.x;
	float t2 = (maxPos.x - rayPos.x) / rayDir.x;
	float t3 = (minPos.y - rayPos.y) / rayDir.y;
	float t4 = (maxPos.y - rayPos.y) / rayDir.y;
	float t5 = (minPos.z - rayPos.z) / rayDir.z;
	float t6 = (maxPos.z - rayPos.z) / rayDir.z;


	//�� t���� ���ؼ� �ּҰ��� �ִ�� �ִ밪�� �ּҸ� ���Ѵ�
	float finalMin = fmaxf(fmaxf(fmin(t1, t2), fmin(t3, t4)), fminf(t5, t6));
	float finalMax = fminf(fminf(fmax(t1, t2), fmax(t3, t4)), fmaxf(t5, t6));

	//������ max�� ������� ���� �ݴ��ʿ� �ִ� -> �浹��������
	if (finalMax < 0.0f) return -1;

	//������ min�� ������ max�� ����ũ�� Ȯ��� ������ �����ؼ� �������� ->�Ʊ��� �浹���� ����
	if (finalMin > finalMax) return -1;

	//���� ������ min�� �������(���� ������ max�� ���) ������ AABB�ȿ��� �߻�Ǿ��� -> ���ްŸ�����
	if (finalMin< 0.0f) return 0;

	//�ּҰ��� ���ްŸ�
	return finalMin;}

bool Collision_RayToOBB(const Vector3 & rayPos, const Vector3 & rayDir, const Vector3 & OBBPos, const Vector3 & xVec, const Vector3 & yVec, const Vector3 & zVec, float xVecSize, float yVecSize, float zVecSize)
{
	//���̿��� OBB�ڽ� �߽����� ���� ���͸� ���Ѵ�
	Vector3 vec1 = OBBPos - rayPos;

	//OBB�ڽ��� ���࿡ ������ ���⺤�͸� �������� float3�� �����Ѵ�
	Vector3 vec2 = Vector3(
		Vec3Dot(&xVec, &rayDir),
		Vec3Dot(&yVec, &rayDir),
		Vec3Dot(&zVec, &rayDir));

	//vec1�� ���������� �������� �������� �����Ѵ�
	Vector3 vec3 = Vector3
	(
		Vec3Dot(&xVec, &vec1),
		Vec3Dot(&yVec, &vec1),
		Vec3Dot(&zVec, &vec1));

	//x���� �ִ�� �ּҸ� ã�´�!!
	float xMax = 0.0f;
	float xMin = 0.0f;
	//���� ������ ������ ��� �����ϰ�(�����Ѱ��� 0�� ���Դ�), OBB�ڽ��� ������� Pos�� x�Ÿ��� ũ�ٸ� �浹�� �� ����.
	if (absFloat(vec2.x) < EPSILON)
	{
		if (vec3.x - xVecSize > 0 || vec3.x + xVecSize < 0)
		{
			return false;
		}
		//�������� 0�̸� ���������Ƿ�~ �ּҰ��� ��
		vec2.x = EPSILON;
	}
	xMin = (vec3.x + xVecSize) / vec2.x;
	xMax = (vec3.x - xVecSize) / vec2.x;


	//y���� �ִ�� �ּҸ� ã�´�!!
	float yMax = 0.0f;
	float yMin = 0.0f;
	//���� ������ ������ ��� �����ϰ�(�����Ѱ��� 0�� ���Դ�), OBB�ڽ��� ������� Pos�� x�Ÿ��� ũ�ٸ� �浹�� �� ����.
	if (absFloat(vec2.y) < EPSILON)
	{
		if (vec3.y - yVecSize > 0 || vec3.y + yVecSize < 0)
		{
			return false;
		}
		//�������� 0�̸� ���������Ƿ�~ �ּҰ��� ��
		vec2.y = EPSILON;
	}
	yMin = (vec3.y + yVecSize) / vec2.y;
	yMax = (vec3.y - yVecSize) / vec2.y;


	//z���� �ִ�� �ּҸ� ã�´�!!
	float zMax = 0.0f;
	float zMin = 0.0f;
	//���� ������ ������ ��� �����ϰ�(�����Ѱ��� 0�� ���Դ�), OBB�ڽ��� ������� Pos�� x�Ÿ��� ũ�ٸ� �浹�� �� ����.
	if (absFloat(vec2.z) < EPSILON)
	{
		if (vec3.z - zVecSize > 0 || vec3.z + zVecSize < 0)
		{
			return false;
		}
		//�������� 0�̸� ���������Ƿ�~ �ּҰ��� ��
		vec2.z = EPSILON;
	}
	zMin = (vec3.z + zVecSize) / vec2.z;
	zMax = (vec3.z - zVecSize) / vec2.z;



	//���³��� �ּ�/�ִ밪���� ���� �ּ�/�ִ밪���� ���Ѵ�
	float FinalMax = 0.0f;
	float FinalMin = 0.0f;

	FinalMin = fminf(
		fminf(
			fminf(xMin, xMax),
			fmin(yMin, yMax)
		)
		,
		fminf(zMin, zMax)
	);

	FinalMax = fmaxf(
		fmaxf(
			fmaxf(xMin, xMax),
			fmaxf(yMin, yMax)
		)
		,
		fmaxf(zMin, zMax)
	);

	//���� ���������� 0���� �۴ٸ�, ������ �ƿ� �ݴ�������� ������� �����Ƿ� �浹�Ұ���
	if (FinalMax < 0) return false;

	//�ּҰ� �ִ뺸�� ũ�ٸ�? �浹���� �ʾҴ�
	if (FinalMin > FinalMax)return false;

	return true;}
