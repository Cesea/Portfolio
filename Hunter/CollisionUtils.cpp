#include "stdafx.h"
#include "CollisionUtils.h"


#define EPSILON 0.001f

Vector3 getSphereClosetVector(Vector3 SpherePos, Vector3 point, float radius)
{
	Vector3 toPoint;
	toPoint = point - SpherePos;
	Vec3Normalize(&toPoint, &toPoint);
	Vector3 closetVec = toPoint*radius;
	return (closetVec + SpherePos);
}

Vector3 getAABBClosetVector(Vector3 MinPoint, Vector3 MaxPoint, Vector3 point)
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

Vector3 getOBBClosetVector(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize, Vector3 point)
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

Vector3 getPlaneClosetVector(Vector3 planeNormal, float planeDistance, Vector3 point)
{
	float dot = Vec3Dot(&planeNormal, &point);
	float distance = dot - planeDistance;
	return point - planeNormal*distance;
}

Vector3 getLineClosetVector(Vector3 startPoint, Vector3 endPoint, Vector3 point)
{
	Vector3 lineVec = endPoint - startPoint;
	Vector3 lineToPoint = point - startPoint;
	float t = Vec3Dot(&lineVec, &lineToPoint) / Vec3Dot(&lineVec, &lineVec);
	if (t < 0) t = 0;
	if (t > 1)t = 1;
	return startPoint + lineVec*t;
}

Vector3 getRayClosetVector(Vector3 rayOrigin, Vector3 rayDirection, Vector3 point)
{
	Vector3 rayToPoint = point - rayOrigin;
	float t = Vec3Dot(&rayToPoint, &rayDirection);
	if (t < 0) t = 0;
	return rayOrigin + rayDirection*t;
}

bool Collision_SphereToSphere(Vector3 spherePos1, Vector3 spherePos2, float radius1, float radius2)
{
	Vector3 dis = spherePos2 - spherePos1;
	float distance = Vec3Length(&dis);
	return (distance < radius1 + radius2);
}

bool Collision_SphereToAABB(Vector3 MinPoint, Vector3 MaxPoint, Vector3 spherePos, float radius)
{
	Vector3 closetVec = getAABBClosetVector(MinPoint, MaxPoint, spherePos);

	Vector3 dis = closetVec - spherePos;

	float distance = Vec3Length(&dis);

	return(distance < radius);
}

bool Collision_SphereToOBB(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize,
	Vector3 spherePos, float radius)
{
	Vector3 closetVec = getOBBClosetVector(OBBPos, xVec, yVec, zVec, xVecSize, yVecSize, zVecSize, spherePos);

	Vector3 dis = closetVec - spherePos;

	float distance = Vec3Length(&dis);

	return (distance < radius);
}

bool Collision_SphereToPlane(Vector3 planeNormal, float planeDistance, Vector3 spherePos, float radius)
{
	Vector3 closetVec = getPlaneClosetVector(planeNormal, planeDistance, spherePos);

	Vector3 dis = closetVec - spherePos;

	float distance = Vec3Length(&dis);

	return (distance < radius);

}

bool Collision_AABBToAABB(Vector3 minPoint1, Vector3 maxPoint1, Vector3 minPoint2, Vector3 maxPoint2)
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

bool Collision_AABBToOBB(Vector3 minPoint, Vector3 maxPoint, Vector3 OBBPos,
	Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize)
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

bool Collision_AABBtoPlane(Vector3 minPoint, Vector3 maxPoint, Vector3 planeNormal, float planeDistance)
{
	float pLen =
		(maxPoint.x - minPoint.x)*(planeNormal.x) + (maxPoint.y - minPoint.y)*absFloat(planeNormal.y) + (maxPoint.z - minPoint.z)*absFloat(planeNormal.z);

	Vector3 AABBPos = Vector3((minPoint.x + maxPoint.x) / 2, (minPoint.y + maxPoint.y) / 2, (minPoint.z + maxPoint.z) / 2);

	float dot = Vec3Dot(&planeNormal, &AABBPos);

	float dist = dot - planeDistance;

	return (absFloat(dist) <= pLen);
}

bool Collision_OBBToOBB(Vector3 OBBPos1, Vector3 xVec1, Vector3 yVec1, Vector3 zVec1, float xVecSize1, float yVecSize1, float zVecSize1
	, Vector3 OBBPos2, Vector3 xVec2, Vector3 yVec2, Vector3 zVec2, float xVecSize2, float yVecSize2, float zVecSize2)
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

bool Collision_OBBToPlane(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize
	, Vector3 planeNormal, float planeDistance)
{
	float pLen = xVecSize*absFloat(Vec3Dot(&xVec, &planeNormal)) + yVecSize*absFloat(Vec3Dot(&yVec, &planeNormal)) + zVecSize*absFloat(Vec3Dot(&zVec, &planeNormal));

	float dot = Vec3Dot(&OBBPos, &planeNormal);
	float dist = dot - planeDistance;

	return absFloat(dist) <= pLen;
}

bool Collision_PlaneToPlane(Vector3 planeNormal1, float planeDistance1, Vector3 planeNormal2, float planeDistance2)
{
	Vector3 vec;
	Vec3Cross(&vec, &planeNormal1, &planeNormal2);
	return (Vec3Dot(&vec, &vec) != 0);
}

bool Collision_RayToSphere(Vector3 rayPos, Vector3 rayDir, Vector3 spherePos, float radius)
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

float Time_RayToSphere(Vector3 rayPos, Vector3 rayDir, Vector3 spherePos, float radius)
{
	Vector3 e = spherePos - rayPos;

	float a = Vec3Dot(&e, &rayDir);
	float eSide = Vec3Length(&e); //esq


	float b = eSide*eSide - a*a;

	float f = sqrtf(radius*radius - b);


	//충돌하지않음?
	if (radius*radius - b < 0.0f) return -1;
	else if (eSide*eSide < radius*radius) return 0;//구안에서 광선을 쏘았음ㄷ
	else
	{
		return a - f;
	}

}

bool Collision_RayToAABB(Vector3 rayPos, Vector3 rayDir, Vector3 minPos, Vector3 maxPos)
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

	return true;
}

float Time_RayToAABB(Vector3 rayPos, Vector3 rayDir, Vector3 minPos, Vector3 maxPos)
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
	return finalMin;
}

bool Collision_RayToOBB(Vector3 rayPos, Vector3 rayDir, Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec,
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

	return true;
}

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
