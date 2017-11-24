#include "stdafx.h"
#include "CollisionUtils.h"


#define EPSILON 0.001f
#define FLOATEQUAL(a,b) ((abs((a)-(b)) )< EPSILON)

bool Time_RayToOBB(Vector3 rayPos, Vector3 rayDir, Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec,
	float xVecSize, float yVecSize, float zVecSize)
{

	//레이에서 OBB박스 중심으로 뻗는 벡터를 구한다
	Vector3 vec1 = OBBPos - rayPos;

	//OBB박스의 각축에 레이의 방향벡터를 투영시켜 float3에 저장한다
	Vector3 vec2 = Vector3(
		Vec3Dot(&xVec, &rayDir),
		Vec3Dot(&yVec, &rayDir),
		Vec3Dot(&zVec, &rayDir));

	//vec1을 마찬가지로 각축으로 투영시켜 저장한다
	Vector3 vec3 = Vector3
	(
		Vec3Dot(&xVec, &vec1),
		Vec3Dot(&yVec, &vec1),
		Vec3Dot(&zVec, &vec1));

	//x축의 최대와 최소를 찾는다!!
	float xMax = 0.0f;
	float xMin = 0.0f;
	//만약 광선의 방향이 축과 평행하고(내적한값이 0이 나왔다), OBB박스의 사이즈보다 Pos의 x거리가 크다면 충돌할 수 없다.
	if (absFloat(vec2.x) < EPSILON)
	{
		if (vec3.x - xVecSize > 0 || vec3.x + xVecSize < 0)
		{
			return -1;
		}
		//나눠질때 0이면 에러가나므로~ 최소값을 줌
		vec2.x = EPSILON;
	}
	xMin = (vec3.x + xVecSize) / vec2.x;
	xMax = (vec3.x - xVecSize) / vec2.x;


	//y축의 최대와 최소를 찾는다!!
	float yMax = 0.0f;
	float yMin = 0.0f;
	//만약 광선의 방향이 축과 평행하고(내적한값이 0이 나왔다), OBB박스의 사이즈보다 Pos의 x거리가 크다면 충돌할 수 없다.
	if (absFloat(vec2.y) < EPSILON)
	{
		if (vec3.y - yVecSize > 0 || vec3.y + yVecSize < 0)
		{
			return -1;
		}
		//나눠질때 0이면 에러가나므로~ 최소값을 줌
		vec2.y = EPSILON;
	}
	yMin = (vec3.y + yVecSize) / vec2.y;
	yMax = (vec3.y - yVecSize) / vec2.y;


	//z축의 최대와 최소를 찾는다!!
	float zMax = 0.0f;
	float zMin = 0.0f;
	//만약 광선의 방향이 축과 평행하고(내적한값이 0이 나왔다), OBB박스의 사이즈보다 Pos의 x거리가 크다면 충돌할 수 없다.
	if (absFloat(vec2.z) < EPSILON)
	{
		if (vec3.z - zVecSize > 0 || vec3.z + zVecSize < 0)
		{
			return -1;
		}
		//나눠질때 0이면 에러가나므로~ 최소값을 줌
		vec2.z = EPSILON;
	}
	zMin = (vec3.z + zVecSize) / vec2.z;
	zMax = (vec3.z - zVecSize) / vec2.z;

	//여태나온 최소/최대값들의 최종 최소/최대값들을 구한다
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

	//만약 최종광선이 0보다 작다면, 광선은 아예 반대방향으로 쏘아지고 있으므로 충돌불가능
	if (FinalMax < 0) return -1;

	//최소가 최대보다 크다면? 충돌하지 않았다
	if (FinalMin > FinalMax)return -1;


	//최소값이 0보다 작다면 광선은 박스안에서 출발한것 -> 충돌한시간은 최대값
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
	//둘이 충돌되지 않았으면 할필요없다
	if (Collision_AABBToAABB(pBoundA->_localMinPos, pBoundA->_localMaxPos,pBoundB->_localMinPos,pBoundB->_localMaxPos) == false)
		return false;

	moveFactor = Clamp01(moveFactor);

	//둘이 부디쳤스니 밀어내야한다...

	//A의 Min Max
	Vector3 minA = pBoundA->_localMinPos;
	Vector3 maxA = pBoundA->_localMaxPos;

	//B의 Min Max
	Vector3 minB = pBoundB->_localMinPos;
	Vector3 maxB = pBoundB->_localMaxPos;

	//B 의 로컬 사각 8 점

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


	//A 의 월드 역행렬
	Matrix matWorldAInv;
	MatrixInverse(&matWorldAInv, NULL, &pTransA->GetFinalMatrix());

	//B 의 월드 행렬
	Matrix matWorldB = pTransB->GetFinalMatrix();

	//B 월드 만큼 가고 A 의 역으로 다시 움직인 행렬
	Matrix mat = matWorldB * matWorldAInv;

	//B pos 에 적용
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	//이이후 Pos 들은 A 대한 B 의 상대적인 위치값들이된다.


	//최종적으로 적용된 B pos 를 가지고 min max 를 갱신 하자
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

	//사각 형 구조체
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

	//겹칩량의 사각형
	fRect rcInter;
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);


	//각축의 겹칩량을 구하고 그중 가장 작은 축으로 B 이동시킨다.
	float interX = rcInter.right - rcInter.left;
	float interY = rcInter.top - rcInter.bottom;
	float interZ = rcInter.front - rcInter.back;
	float minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	//미는 방향
	D3DXVECTOR3 moveDirA(0, 0, 0);

	//밀량
	float moveLengthA = minInter;

	//X 축의 겹칩량이 제일 작다면..
	if (minInter == interX)
	{
		//A 의 왼쪽으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.left, rcA.left))
			moveDirA = -pTransA->GetRight();

		//A 의 오른쪽으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.right, rcA.right))
			moveDirA = pTransA->GetRight();
	}
	//Y 축의 겹칩량이 제일 작다면..
	else if (minInter == interY)
	{
		//A 의 위으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.top, rcA.top))
			moveDirA = pTransA->GetUp();

		//A 의 아래으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.bottom, rcA.bottom))
			moveDirA = -pTransA->GetUp();

	}

	//Z 축의 겹침량이 제일 작다면..
	else if (minInter == interZ)
	{
		//A 의 정면으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.front, rcA.front))
			moveDirA = pTransA->GetForward();

		//A 의 뒤으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.back, rcA.back))
			moveDirA = -pTransA->GetForward();
	}

	//여기까지온다면 밀량과 미는 방향이 계산된다.
	//moveDirA
	//moveLengthA
	//위의 두값은 A 를 가만히 두고 B 를 A 역으로 계산한 값이된다.

	//
	// 아래의 연산은 B 를 가만히 두고 A 를 B 역으로 계산한 값이된다.
	//
	D3DXVECTOR3 moveDirB(0, 0, 0);
	float moveLengthB = 0.0f;

	//A의 Min Max
	minA = pBoundA->_localMinPos;
	maxA = pBoundA->_localMaxPos;

	//B의 Min Max
	minB = pBoundB->_localMinPos;
	maxB = pBoundB->_localMaxPos;

	//B 의 로컬 사각 8 점

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

	//B 의 월드 역행렬
	Matrix matWorldBInv;
	D3DXMatrixInverse(&matWorldBInv, NULL, &pTransB->GetFinalMatrix());

	//A 의 월드 행렬
	D3DXMATRIXA16 matWorldA = pTransA->GetFinalMatrix();

	//A 월드 만큼 가고 B 의 역으로 다시 움직인 행렬
	mat = matWorldA * matWorldBInv;

	//A pos 에 적용
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	//최종적으로 적용된 A pos 를 가지고 min max 를 갱신 하자
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

	//겹칩량의 사각형
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);

	//각축의 겹칩량을 구하고 그중 가장 작은 축으로 B 이동시킨다.
	interX = rcInter.right - rcInter.left;
	interY = rcInter.top - rcInter.bottom;
	interZ = rcInter.front - rcInter.back;
	minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	moveLengthB = minInter;

	//X 축의 겹칩량이 제일 작다면..
	if (minInter == interX)
	{
		//B 의 왼쪽으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.left, rcB.left))
			moveDirB = -pTransB->GetRight();

		//B 의 오른쪽으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.right, rcB.right))
			moveDirB = pTransB->GetRight();
	}
	//Y 축의 겹칩량이 제일 작다면..
	else if (minInter == interY)
	{
		//B 의 위으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.top, rcB.top))
			moveDirB = pTransB->GetUp();

		//B 의 아래으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.bottom, rcB.bottom))
			moveDirB = -pTransB->GetUp();

	}

	//Z 축의 겹침량이 제일 작다면..
	else if (minInter == interZ)
	{
		//B 의 정면으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.front, rcB.front))
			moveDirB = pTransB->GetForward();

		//B 의 뒤으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.back, rcB.back))
			moveDirB = -pTransB->GetForward();
	}


	//여기까지온다면 아래의 4 개의 변수가 계산된 것이다....
	//D3DXVECTOR3 moveDirA( 0, 0, 0 );
	//float moveLengthA = minInter;
	//D3DXVECTOR3 moveDirB( 0, 0, 0 );
	//float moveLengthB = 0.0f;


	//밀량이 작은쪽으로...
	if (moveLengthB > moveLengthA)
	{
		//A 와 B 의 스케일 적용
		//여긴 A 를 가만히 두고 B 를 건들인거다
		//따라서 A 의 스케일이 2 였고 B 의 스케일 이 1 이였다고 가정하였을 때
		//A 의 역행렬로 계산되어 A 의 스케일이 1 B 의 스케일이 0.5 로 계산된 길이이다
		//그러므로 각 축의 길이는
		//Ascale 값을 곱하여 계산하는 것이 올바르다.
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
		//A 와 B 의 스케일 적용
		//여긴 B 를 가만히 두고 A 를 건들인거다 
		//따라서 A 의 스케일이 2 였고 B 의 스케일 이 1 이였다고 가정하였을때
		//B 의 역행렬로 계산되어 A 의 스케일이 2 B 의 스케일이 1 로 계산된 길이이다
		//그러므로 각 축의 길이는 BScale 값을 곱하여 계산하는 것이 올바르다.

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

	//변환된 위치
	Vec3TransformCoord(&center, &boundingSphere._localCenter, &matWorld);

	//반지름...
	halfSize.x = boundingSphere._radius  * scale.x;
	halfSize.y = boundingSphere._radius * scale.y;
	halfSize.z = boundingSphere._radius * scale.z;
	radius = Vec3Length(&halfSize);

	//레이의 오리진에서 부터 구의 센터까지의 방향벡터
	Vector3 dirToCenter = center - ray.origin;

	//길이의 제곱
	float lengthSq = Vec3LengthSq(&dirToCenter);

	//반지름의 제곱
	float r2 = radius * radius;

	//만약 광선이 구안에 있다면..
	if (r2 > lengthSq)
	{
		//광선이 안에서 나가는 것은 체크 안된다.
		return false;
	}

	//여기까지오면 오리진은 구밖에 있다는예기
	//구센터까지의 방향벡터와 레이의 방향벡터가 직각을 포함한 
	//둔각이라면 죽었다깨어나도 충돌될일없다
	float dot = Vec3Dot(&dirToCenter, &ray.direction);
	if (dot <= 0.0f)
	{
		return false;
	}

	// 피타고라스의 정리를 하기위해 직각 삼각형 공식유도
	// d2 = x2 + y2;
	// d = sqrt( x2 + y2 );

	float x2 = dot * dot;
	float d2 = lengthSq;
	//d2 - x2 = y2;
	float y2 = d2 - x2;

	//광선이 원밖을 벗어났다.
	if (y2 > r2) 
	{
		return false;
	}

	//여기까지온다면 일단은 히트
	//만약 얻아가야알 HitPoint 가있다면..
	if (pOutHitPos != nullptr)
	{
		//d를 raius 제곱
		d2 = r2;
		//d2 = y2 + x2
		//float x2 = d2 - y2;
		float x = sqrt(d2 - y2);

		//
		*pOutHitPos = ray.origin + (ray.direction * (dot - x));

		//Hit 된 위치의 노말을 얻겠다면..
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
	//x축 투영
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

	//y축 투영
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

	//z축 투영
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
	
	// 1. 15개의 필요한 축을 구한다
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

		// 2 . AABB박스의 8개의 vertex 벡터가 축에 projection 됬을 때 최소길이와 최대길이를 구한다
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

		// 3. OBB박스의 8개의 vertex 벡터가 축에 projecttion 됬을 때 최소길이와 최대길이를 구한다
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
		// 4. AABB박스의 최소~최대 와 OBB박스의 최소~최대를 비교해서 둘이 속하는 관계인지를 찾는다
		if (max >= min2&&min <= max2)
		{
			continue;
		}
		else
		{
			return false;
		}
		// 5. 15번 반복해서 전부 속한다면 충돌이다
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
	// 1. 15개의 필요한 축을 구한다
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

		// 2 . AABB박스의 8개의 vertex 벡터가 축에 projection 됬을 때 최소길이와 최대길이를 구한다
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

		// 3. OBB박스의 8개의 vertex 벡터가 축에 projecttion 됬을 때 최소길이와 최대길이를 구한다
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
		// 4. AABB박스의 최소~최대 와 OBB박스의 최소~최대를 비교해서 둘이 속하는 관계인지를 찾는다
		if (max >= min2&&min <= max2)
		{
			continue;
		}
		else
		{
			return false;
		}
		// 5. 15번 반복해서 전부 속한다면 충돌이다
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

	//충돌하지않음?
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

	//충돌하지않음?
	if (radius*radius - b < 0.0f) return -1;
	else if (eSide*eSide < radius*radius) return 0;//구안에서 광선을 쏘았음ㄷ
	else
	{
		return a - f;
	}
}

bool Collision_RayToAABB(const Vector3 & rayPos, const Vector3 & rayDir, const Vector3 & minPos, const Vector3 & maxPos)
{
//AABB박스를 확장해서 충돌을 체크한다(direction으로 나눠서 가지않는쪽은 값이 0
	float t1 = (minPos.x - rayPos.x) / rayDir.x;
	float t2 = (maxPos.x - rayPos.x) / rayDir.x;
	float t3 = (minPos.y - rayPos.y) / rayDir.y;
	float t4 = (maxPos.y - rayPos.y) / rayDir.y;
	float t5 = (minPos.z - rayPos.z) / rayDir.z;
	float t6 = (maxPos.z - rayPos.z) / rayDir.z;


	//각 t들을 비교해서 최소값의 최대와 최대값의 최소를 구한다
	float finalMin = fmaxf(fmaxf(fmin(t1, t2), fmin(t3, t4)), fminf(t5, t6));
	float finalMax = fminf(fminf(fmax(t1, t2), fmax(t3, t4)), fmaxf(t5, t6));

	//마지막 max가 음수라면 광선 반대쪽에 있다 -> 충돌하지못함
	if (finalMax < 0.0f) return false;

	//마지막 min이 마지막 max값 보다크면 확장된 공간을 관통해서 지나갔다 ->아깝게 충돌하지 못함
	if (finalMin > finalMax) return false;

	//만약 마지막 min이 음수라면(현재 마지막 max는 양수) 광선이 AABB안에서 발사되었다 -> 무조건 충돌
	if (finalMin< 0.0f) return true;

	return true;}

float Time_RayToAABB(const Vector3 & rayPos, const Vector3 & rayDir, const Vector3 & minPos, const Vector3 & maxPos)
{
//AABB박스를 확장해서 충돌을 체크한다(direction으로 나눠서 가지않는쪽은 값이 0
	float t1 = (minPos.x - rayPos.x) / rayDir.x;
	float t2 = (maxPos.x - rayPos.x) / rayDir.x;
	float t3 = (minPos.y - rayPos.y) / rayDir.y;
	float t4 = (maxPos.y - rayPos.y) / rayDir.y;
	float t5 = (minPos.z - rayPos.z) / rayDir.z;
	float t6 = (maxPos.z - rayPos.z) / rayDir.z;


	//각 t들을 비교해서 최소값의 최대와 최대값의 최소를 구한다
	float finalMin = fmaxf(fmaxf(fmin(t1, t2), fmin(t3, t4)), fminf(t5, t6));
	float finalMax = fminf(fminf(fmax(t1, t2), fmax(t3, t4)), fmaxf(t5, t6));

	//마지막 max가 음수라면 광선 반대쪽에 있다 -> 충돌하지못함
	if (finalMax < 0.0f) return -1;

	//마지막 min이 마지막 max값 보다크면 확장된 공간을 관통해서 지나갔다 ->아깝게 충돌하지 못함
	if (finalMin > finalMax) return -1;

	//만약 마지막 min이 음수라면(현재 마지막 max는 양수) 광선이 AABB안에서 발사되었다 -> 도달거리없음
	if (finalMin< 0.0f) return 0;

	//최소값이 도달거리
	return finalMin;}

bool Collision_RayToOBB(const Vector3 & rayPos, const Vector3 & rayDir, const Vector3 & OBBPos, const Vector3 & xVec, const Vector3 & yVec, const Vector3 & zVec, float xVecSize, float yVecSize, float zVecSize)
{
	//레이에서 OBB박스 중심으로 뻗는 벡터를 구한다
	Vector3 vec1 = OBBPos - rayPos;

	//OBB박스의 각축에 레이의 방향벡터를 투영시켜 float3에 저장한다
	Vector3 vec2 = Vector3(
		Vec3Dot(&xVec, &rayDir),
		Vec3Dot(&yVec, &rayDir),
		Vec3Dot(&zVec, &rayDir));

	//vec1을 마찬가지로 각축으로 투영시켜 저장한다
	Vector3 vec3 = Vector3
	(
		Vec3Dot(&xVec, &vec1),
		Vec3Dot(&yVec, &vec1),
		Vec3Dot(&zVec, &vec1));

	//x축의 최대와 최소를 찾는다!!
	float xMax = 0.0f;
	float xMin = 0.0f;
	//만약 광선의 방향이 축과 평행하고(내적한값이 0이 나왔다), OBB박스의 사이즈보다 Pos의 x거리가 크다면 충돌할 수 없다.
	if (absFloat(vec2.x) < EPSILON)
	{
		if (vec3.x - xVecSize > 0 || vec3.x + xVecSize < 0)
		{
			return false;
		}
		//나눠질때 0이면 에러가나므로~ 최소값을 줌
		vec2.x = EPSILON;
	}
	xMin = (vec3.x + xVecSize) / vec2.x;
	xMax = (vec3.x - xVecSize) / vec2.x;


	//y축의 최대와 최소를 찾는다!!
	float yMax = 0.0f;
	float yMin = 0.0f;
	//만약 광선의 방향이 축과 평행하고(내적한값이 0이 나왔다), OBB박스의 사이즈보다 Pos의 x거리가 크다면 충돌할 수 없다.
	if (absFloat(vec2.y) < EPSILON)
	{
		if (vec3.y - yVecSize > 0 || vec3.y + yVecSize < 0)
		{
			return false;
		}
		//나눠질때 0이면 에러가나므로~ 최소값을 줌
		vec2.y = EPSILON;
	}
	yMin = (vec3.y + yVecSize) / vec2.y;
	yMax = (vec3.y - yVecSize) / vec2.y;


	//z축의 최대와 최소를 찾는다!!
	float zMax = 0.0f;
	float zMin = 0.0f;
	//만약 광선의 방향이 축과 평행하고(내적한값이 0이 나왔다), OBB박스의 사이즈보다 Pos의 x거리가 크다면 충돌할 수 없다.
	if (absFloat(vec2.z) < EPSILON)
	{
		if (vec3.z - zVecSize > 0 || vec3.z + zVecSize < 0)
		{
			return false;
		}
		//나눠질때 0이면 에러가나므로~ 최소값을 줌
		vec2.z = EPSILON;
	}
	zMin = (vec3.z + zVecSize) / vec2.z;
	zMax = (vec3.z - zVecSize) / vec2.z;



	//여태나온 최소/최대값들의 최종 최소/최대값들을 구한다
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

	//만약 최종광선이 0보다 작다면, 광선은 아예 반대방향으로 쏘아지고 있으므로 충돌불가능
	if (FinalMax < 0) return false;

	//최소가 최대보다 크다면? 충돌하지 않았다
	if (FinalMin > FinalMax)return false;

	return true;}
