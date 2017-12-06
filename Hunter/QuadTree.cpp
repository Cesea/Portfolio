#include "stdafx.h"
#include "QuadTree.h"

#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

bool32 IsRayHitBoundSphere(const Ray &ray, const Vector3 &center, float radius,
	Vector3 *pOutHitPos, Vector3 *pOutHitNormal)
{
	//레이의 오리진에서 부터 구의 센터까지의 방향벡터
	Vector3 dirToCenter = center - ray.origin;

	//길이의 제곱
	float lengthSq = Vec3LengthSq(&dirToCenter);

	//반지름의 제곱
	float r2 = radius * radius;
	float dot = Vec3Dot( &dirToCenter, &ray.direction );
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
	if (nullptr != pOutHitPos)
	{
		//d를 raius 제곱
		d2 = r2;
		//d2 = y2 + x2
		//float x2 = d2 - y2;
		float x = sqrt(d2 - y2);

		//
		*pOutHitPos = ray.origin + (ray.direction * (dot - x));

		//Hit 된 위치의 노말을 얻겠다면..
		if (nullptr != pOutHitNormal)
		{
			*pOutHitNormal = *pOutHitPos - center;
			Vec3Normalize(pOutHitNormal, pOutHitNormal);
		}
	}

	return true;
}

QuadTree::QuadTree()
{
	//차일드 포인터 NULL 처리
	ZeroMemory(_pChilds, sizeof(QuadTree*) * 4);
	//ZeroMemory(_pNeighbors, sizeof(QuadTree *) * 4);

	//센터 인덱스는 -1
	_center = -1;
}

QuadTree::~QuadTree()
{
	//자식 안전 지우기
	SAFE_DELETE(_pChilds[0]);
	SAFE_DELETE(_pChilds[1]);
	SAFE_DELETE(_pChilds[2]);
	SAFE_DELETE(_pChilds[3]);
}


bool QuadTree::Init(video::TerrainVertex *pVertices, uint32 verNumEdge, int32 minRadius)
{
	_pTerrainVertices = pVertices;

	_corners[eCornerLT] = 0;
	_corners[eCornerRT] = verNumEdge - 1;
	_corners[eCornerLB] = (verNumEdge - 1) * verNumEdge;
	_corners[eCornerRB] = verNumEdge * verNumEdge - 1;

	_minRadius = minRadius;

	CreateChildTree();

	//BuildNeighborNode(this, _corners[eCornerRB] + 1);

	return true;
}

void QuadTree::CreateChildTree()
{
	//중심위치
	_centerPos = (_pTerrainVertices[_corners[0]]._pos +
		_pTerrainVertices[_corners[1]]._pos +
		_pTerrainVertices[_corners[2]]._pos +
		_pTerrainVertices[_corners[3]]._pos) * 0.25f;

	//각코너 점 중에서의 거리가 반지름 이다
	Vector3 dir = _pTerrainVertices[_corners[0]]._pos - _centerPos;
	float prevLength = Vec3LengthSq(&dir);
	_radius = sqrt(prevLength);

	//자식이 있다.
	if ((_corners[eCornerRT] - _corners[eCornerLT]) > _minRadius)
	{
		//센터 인덱스 계산
		_center = (_corners[0] + _corners[1] + _corners[2] + _corners[3]) / 4;

		uint32 topCenter = (_corners[eCornerRT] + _corners[eCornerLT]) / 2; //상단 중앙
		uint32 leftCenter = (_corners[eCornerLT] + _corners[eCornerLB]) / 2; //좌 중앙
		uint32 rightCenter = (_corners[eCornerRT] + _corners[eCornerRB]) / 2; //우 중앙
		uint32 bottomCenter = (_corners[eCornerRB] + _corners[eCornerLB]) / 2; //하단 중앙

																			   //좌상단 자식
		_pChilds[eCornerLT] = new QuadTree;
		_pChilds[eCornerLT]->_corners[eCornerLT] = _corners[eCornerLT];
		_pChilds[eCornerLT]->_corners[eCornerRT] = topCenter;
		_pChilds[eCornerLT]->_corners[eCornerLB] = leftCenter;
		_pChilds[eCornerLT]->_corners[eCornerRB] = _center;
		_pChilds[eCornerLT]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerLT]->_minRadius = _minRadius;
		_pChilds[eCornerLT]->CreateChildTree();

		//우상단 자식
		_pChilds[eCornerRT] = new QuadTree;
		_pChilds[eCornerRT]->_corners[eCornerLT] = topCenter;
		_pChilds[eCornerRT]->_corners[eCornerRT] = _corners[eCornerRT];
		_pChilds[eCornerRT]->_corners[eCornerLB] = _center;
		_pChilds[eCornerRT]->_corners[eCornerRB] = rightCenter;
		_pChilds[eCornerRT]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerRT]->_minRadius = _minRadius;
		_pChilds[eCornerRT]->CreateChildTree();

		//좌하단 자식
		_pChilds[eCornerLB] = new QuadTree;
		_pChilds[eCornerLB]->_corners[eCornerLT] = leftCenter;
		_pChilds[eCornerLB]->_corners[eCornerRT] = _center;
		_pChilds[eCornerLB]->_corners[eCornerLB] = _corners[eCornerLB];
		_pChilds[eCornerLB]->_corners[eCornerRB] = bottomCenter;
		_pChilds[eCornerLB]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerLB]->_minRadius = _minRadius;
		_pChilds[eCornerLB]->CreateChildTree();

		//우하단 자식
		_pChilds[eCornerRB] = new QuadTree;
		_pChilds[eCornerRB]->_corners[eCornerLT] = _center;
		_pChilds[eCornerRB]->_corners[eCornerRT] = rightCenter;
		_pChilds[eCornerRB]->_corners[eCornerLB] = bottomCenter;
		_pChilds[eCornerRB]->_corners[eCornerRB] = _corners[eCornerRB];
		_pChilds[eCornerRB]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerRB]->_minRadius = _minRadius;
		_pChilds[eCornerRB]->CreateChildTree();
	}
	else
	{
		//센터 인덱스 계산
		_center = (_corners[0] + _corners[1] + _corners[2] + _corners[3]) / 4;
	}
}

void QuadTree::GetRayHits(const Ray &ray, std::vector<Vector3> *pOutHit)
{
	if (IsRayHitBoundSphere(ray, _centerPos, _radius, NULL, NULL))
	{
		//자식이 있냐?
		if ((_corners[eCornerRT] - _corners[eCornerLT]) > _minRadius)
		{
			//자식 재귀
			for (int32 i = 0; i < 4; i++)
			{
				_pChilds[i]->GetRayHits(ray, pOutHit);
			}
		}
		//자식이 없는 마지막 노드일떄
		else
		{
			//lt--rt
			//|   /|
			//|  / |
			//| /  |
			//|/   |
			//lb--rb

			//나(타일)의 폴리곤과 Ray의 hit지점을 얻자
			float dist = 0.0f;
			Vector3 lt = this->_pTerrainVertices[_corners[eCornerLT]]._pos;
			Vector3 rt = this->_pTerrainVertices[_corners[eCornerRT]]._pos;
			Vector3 lb = this->_pTerrainVertices[_corners[eCornerLB]]._pos;
			Vector3 rb = this->_pTerrainVertices[_corners[eCornerRB]]._pos;

			//좌상단 폴리곤 체크
			if (D3DXIntersectTri( &lt, &rt, &lb, &ray.origin, &ray.direction,
				nullptr, nullptr, &dist))
			{
				Vector3 hitPos = ray.origin + ray.direction * dist;

				//푸쉬
				pOutHit->push_back(hitPos);
				return;
			}
			//우하단 폴리곤 체크
			if (D3DXIntersectTri( &rt, &rb, &lb, &ray.origin, &ray.direction,
				nullptr, nullptr, &dist))			
			{
				//히트 지점
				Vector3 hitPos = ray.origin + ray.direction * dist;
				//푸쉬
				pOutHit->push_back(hitPos);
				return;
			}
		}
	}
}

//int32 QuadTree::GenerateIndex(uint8 * pIndexData, const Frustum & frustum, const Vector3 &camPos, float LODRatio)
//{
//	FrustumCull(frustum);
//	return GenerateTriIndex(0, pIndexData, camPos, LODRatio);
//}
//
//int32 QuadTree::IsInFrustum(const Frustum & frustum)
//{
//	bool32 cornerIn[4];
//
//	Vector3 currentPoint = (_pTerrainVertices + _center)->_pos;
//	if (frustum.IsSphereInFrustum(currentPoint, _radius))
//	{
//		cornerIn[0] = frustum.IsPointIntFrustum((_pTerrainVertices + _corners[0])->_pos);
//		cornerIn[1] = frustum.IsPointIntFrustum((_pTerrainVertices + _corners[1])->_pos);
//		cornerIn[2] = frustum.IsPointIntFrustum((_pTerrainVertices + _corners[2])->_pos);
//		cornerIn[3] = frustum.IsPointIntFrustum((_pTerrainVertices + _corners[3])->_pos);
//
//		if (cornerIn[0] + cornerIn[1] + cornerIn[2] + cornerIn[3] == 4)
//		{
//			return FRUSTUM_COMPLETLY_IN;
//		}
//		else
//		{
//			return FRUSTUM_PARTIALLY_IN;
//		}
//	}
//	else
//	{
//		return FRUSTUM_OUT;
//	}
//}
//
//void QuadTree::FrustumCull(const Frustum & frustum)
//{
//	int32 result;
//	result = IsInFrustum(frustum);
//	switch (result)
//	{
//		//절두체에 완전히 포함 됨, 하위 노드 검색 필요 없음
//	case FRUSTUM_COMPLETLY_IN :
//	{
//		AllInFrustum();
//		return;
//	}break;
//	//절두체에 부분적으로 포함 되어서, 하위 노드 검색 필요
//	case FRUSTUM_PARTIALLY_IN :
//	{
//		_culled = false;
//	}break;
//	//절두체에 완전히 벗어남, 하위 노드 검색 필요 없음
//	case FRUSTUM_OUT : 
//	{
//		_culled = true;
//		return;
//	}break;
//	}
//
//	if (_pChilds[0])
//	{
//		_pChilds[0]->FrustumCull(frustum);
//	}
//	if (_pChilds[1])
//	{
//		_pChilds[1]->FrustumCull(frustum);
//	}
//	if (_pChilds[2])
//	{
//		_pChilds[2]->FrustumCull(frustum);
//	}
//	if (_pChilds[3])
//	{
//		_pChilds[3]->FrustumCull(frustum);
//	}
//}
//
//float QuadTree::GetDistance(const Vector3 & v1, const Vector3 & v2)
//{
//	return Vec3Length(&(v1 - v2));
//}
//
//int32 QuadTree::GetLODLevel(const Vector3 & cameraPos, float LODRatio)
//{
//	float d = GetDistance((_pTerrainVertices + _center)->_pos, cameraPos);
//	return FMax((int32)(d * LODRatio), 1);
//}
//
//int32 QuadTree::GenerateTriIndex(int32 numTri, uint8 * pIndexData, const Vector3 &camPos, float LODRatio)
//{
//	if (_culled)
//	{
//		if (nullptr != _pParent)
//		{
//			if (_pParent->_culled)
//			{
//				_culled = false;
//				return numTri;
//			}
//		}
//	}
//
//	uint32 *p = ((uint32 *)pIndexData) + numTri * 3;
//
//	if (IsVisible(camPos, LODRatio))
//	{
//		if (_corners[eCornerRT] - _corners[eCornerLT] <= 1.0f)
//		{
//			// 좌측상단 삼각형
//			*p++ = _corners[eCornerLB];
//			*p++ = _corners[eCornerLT];
//			*p++ = _corners[eCornerRT];
//			numTri++;
//			// 우측하단 삼각형
//			*p++ = _corners[eCornerLB];
//			*p++ = _corners[eCornerRT];
//			*p++ = _corners[eCornerRB];
//			numTri++;
//
//			return numTri;
//		}
//
//		bool32 neighborVisible[4] = { 0, 0, 0, 0 };
//
//		if (nullptr != _pNeighbors[EDGE_UP])
//		{
//			neighborVisible[EDGE_UP] = _pNeighbors[EDGE_UP]->IsVisible(camPos, LODRatio);
//		}
//		if (nullptr != _pNeighbors[EDGE_DOWN])
//		{
//			neighborVisible[EDGE_DOWN] = _pNeighbors[EDGE_DOWN]->IsVisible(camPos, LODRatio);
//		}
//		if (nullptr != _pNeighbors[EDGE_LEFT])
//		{
//			neighborVisible[EDGE_LEFT] = _pNeighbors[EDGE_LEFT]->IsVisible(camPos, LODRatio);
//		}
//		if (nullptr != _pNeighbors[EDGE_RIGHT])
//		{
//			neighborVisible[EDGE_RIGHT] = _pNeighbors[EDGE_RIGHT]->IsVisible(camPos, LODRatio);
//		}
//
//		if (neighborVisible[EDGE_UP] && neighborVisible[EDGE_DOWN] && 
//			neighborVisible[EDGE_LEFT] && neighborVisible[EDGE_RIGHT])
//		{
//			// 좌측상단 삼각형
//			*p++ = _corners[eCornerLB];
//			*p++ = _corners[eCornerLT];
//			*p++ = _corners[eCornerRT];
//			numTri++;
//			// 우측하단 삼각형
//			*p++ = _corners[eCornerLB];
//			*p++ = _corners[eCornerRT];
//			*p++ = _corners[eCornerRB];
//			numTri++;
//
//			return numTri;
//		}
//
//		int32 n;
//		//상단 부분 갭 채우기
//		if (!neighborVisible[EDGE_UP])
//		{
//			n = (_corners[eCornerLT] + _corners[eCornerRT]) / 2;
//			*p++ = _center; *p++ = _corners[eCornerLT]; *p++ = n;
//			numTri++;
//
//			*p++ = _center; *p++ = n; *p++ = _corners[eCornerRT];
//			numTri++;
//		}
//		else
//		{
//			*p++ = _center; *p++ = _corners[eCornerLT]; *p++ = _corners[eCornerRT];
//			numTri++;
//		}
//		//하단 갭 채우기
//		if (!neighborVisible[EDGE_DOWN])
//		{
//			n = (_corners[eCornerLB] + _corners[eCornerRB]) / 2;
//			*p++ = _center; *p++ = _corners[eCornerRB]; *p++ = n;
//			numTri++;
//
//			*p++ = _center; *p++ = n; *p++ = _corners[eCornerLB];
//			numTri++;
//		}
//		else
//		{
//			*p++ = _center; *p++ = _corners[eCornerRB]; *p++ = _corners[eCornerLB];
//			numTri++;
//		}
//		//좌측 갭 채우기
//		if (!neighborVisible[EDGE_LEFT])
//		{
//			n = (_corners[eCornerLT] + _corners[eCornerLB]) / 2;
//			*p++ = _center; *p++ = _corners[eCornerLB]; *p++ = n;
//			numTri++;
//
//			*p++ = _center; *p++ = n; *p++ = _corners[eCornerLT];
//			numTri++;
//		}
//		else
//		{
//			*p++ = _center; *p++ = _corners[eCornerLB]; *p++ = _corners[eCornerLT];
//			numTri++;
//		}
//		//우측 갭 채우기
//		if (!neighborVisible[EDGE_RIGHT])
//		{
//			n = (_corners[eCornerRT] + _corners[eCornerRB]) / 2;
//			*p++ = _center; *p++ = n; *p++ = _corners[eCornerRB];
//			numTri++;
//
//			*p++ = _center; *p++ = _corners[eCornerRT]; *p++ = n;
//			numTri++;
//		}
//		else
//		{
//			*p++ = _center; *p++ = _corners[eCornerRT]; *p++ = _corners[eCornerRB];
//			numTri++;
//		}
//		return numTri;
//	}
//
//	if (_pChilds[eCornerLB])
//	{
//		numTri = _pChilds[eCornerLB]->GenerateTriIndex(numTri, pIndexData, camPos, LODRatio);
//	}
//	if (_pChilds[eCornerRB])
//	{
//		numTri = _pChilds[eCornerRB]->GenerateTriIndex(numTri, pIndexData, camPos, LODRatio);
//	}
//	if (_pChilds[eCornerLT])
//	{
//		numTri = _pChilds[eCornerLT]->GenerateTriIndex(numTri, pIndexData, camPos, LODRatio);
//	}
//	if (_pChilds[eCornerRT])
//	{
//		numTri = _pChilds[eCornerRT]->GenerateTriIndex(numTri, pIndexData, camPos, LODRatio);
//	}
//	return numTri;
//}
//
//bool32 QuadTree::IsVisible(const Vector3 &camPos, float LODRatio)
//{
//	return (_corners[eCornerRT] - _corners[eCornerLT]) <= GetLODLevel(camPos, LODRatio);
//}
//
//
//void QuadTree::BuildNeighborNode(QuadTree *pRoot, int32 cx)
//{
//	int32 n;
//	int32 i0, i1, i2, i3;
//
//	for (int32 i = 0; i < 4; ++i)
//	{
//		i0 = _corners[0];
//		i1 = _corners[1];
//		i2 = _corners[2];
//		i3 = _corners[3];
//
//		n = GetNodeIndex(i, cx, i0, i1, i2, i3);
//		if (n >= 0)
//		{
//			_pNeighbors[i] = pRoot->FindNode(i0, i1, i2, i3);
//		}
//	}
//	if (nullptr != _pChilds[0])
//	{
//		_pChilds[0]->BuildNeighborNode(pRoot, cx);
//		_pChilds[1]->BuildNeighborNode(pRoot, cx);
//		_pChilds[2]->BuildNeighborNode(pRoot, cx);
//		_pChilds[3]->BuildNeighborNode(pRoot, cx);
//	}
//}
//
//QuadTree *QuadTree::FindNode(int32 i0, int32 i1, int32 i2, int32 i3)
//{
//	QuadTree *result = nullptr;
//	if (_corners[0] == i0 && _corners[1] == i1 && _corners[2] == i2 && _corners[3] == i3)
//	{
//		return this;
//	}
//	if (nullptr != _pChilds[0])
//	{
//		RECT rect;
//		POINT point;
//
//		int32 n = (i0 + i1 + i2 + i3) / 4;
//
//		point.x = (int32)_pTerrainVertices[n]._pos.x;
//		point.y = (int32)_pTerrainVertices[n]._pos.z;
//
//		SetRect(&rect,
//			(int32)_pTerrainVertices[_pChilds[0]->_corners[eCornerLT]]._pos.x,
//			(int32)_pTerrainVertices[_pChilds[0]->_corners[eCornerLT]]._pos.z,
//			(int32)_pTerrainVertices[_pChilds[0]->_corners[eCornerRB]]._pos.x,
//			(int32)_pTerrainVertices[_pChilds[0]->_corners[eCornerRB]]._pos.z);
//
//		if (IsInRect(&rect, point))
//		{
//			return _pChilds[0]->FindNode(i0, i1, i2, i3);
//		}
//
//		SetRect(&rect,
//			(int32)_pTerrainVertices[_pChilds[1]->_corners[eCornerLT]]._pos.x,
//			(int32)_pTerrainVertices[_pChilds[1]->_corners[eCornerLT]]._pos.z,
//			(int32)_pTerrainVertices[_pChilds[1]->_corners[eCornerRB]]._pos.x,
//			(int32)_pTerrainVertices[_pChilds[1]->_corners[eCornerRB]]._pos.z);
//
//		if (IsInRect(&rect, point))
//		{
//			return _pChilds[1]->FindNode(i0, i1, i2, i3);
//		}
//
//		SetRect(&rect,
//			(int32)_pTerrainVertices[_pChilds[2]->_corners[eCornerLT]]._pos.x,
//			(int32)_pTerrainVertices[_pChilds[2]->_corners[eCornerLT]]._pos.z,
//			(int32)_pTerrainVertices[_pChilds[2]->_corners[eCornerRB]]._pos.x,
//			(int32)_pTerrainVertices[_pChilds[2]->_corners[eCornerRB]]._pos.z);
//
//		if (IsInRect(&rect, point))
//		{
//			return _pChilds[2]->FindNode(i0, i1, i2, i3);
//		}
//
//		SetRect(&rect,
//			(int32)_pTerrainVertices[_pChilds[3]->_corners[eCornerLT]]._pos.x,
//			(int32)_pTerrainVertices[_pChilds[3]->_corners[eCornerLT]]._pos.z,
//			(int32)_pTerrainVertices[_pChilds[3]->_corners[eCornerRB]]._pos.x,
//			(int32)_pTerrainVertices[_pChilds[3]->_corners[eCornerRB]]._pos.z);
//
//		if (IsInRect(&rect, point))
//		{
//			return _pChilds[3]->FindNode(i0, i1, i2, i3);
//		}
//	}
//	return nullptr;
//}
//
//int32 QuadTree::GetNodeIndex(int32 edge, int32 cx, int32 &i0, int32 &i1, int32 &i2, int32 &i3)
//{
//	int32 n, a, b, c, d, gap;
//	a = i0;
//	b = i1;
//	c = i2;
//	d = i3;
//
//	gap = b - a;
//
//	switch (edge)
//	{
//	case EDGE_UP :
//	{
//		i0 = c;
//		i1 = d;
//		i2 = c + cx * gap;
//		i3 = d + cx * gap;
//	}break;
//	case EDGE_DOWN :
//	{
//		i0 = a - cx * gap;
//		i1 = b - cx * gap;
//		i2 = a;
//		i3 = b;
//	}break;
//	case EDGE_LEFT :
//	{
//		i0 = a - gap;
//		i1 = a;
//		i2 = c - gap;
//		i3 = c;
//	}break;
//	case EDGE_RIGHT :
//	{
//		i0 = b;
//		i1 = b + gap;
//		i2 = d;
//		i3 = d + gap;
//		if ((i0 / cx) != (a / cx)) return -1;
//		if ((i1 / cx) != (b / cx)) return -1;
//	}break;
//	}
//	
//	n = (i0 + i1 + i2 + i3) / 4;
//
//	if (!IS_IN_RANGE(n, 0, cx * cx - 1))
//	{
//		return -1;
//	}
//	return n;
//
//}
//
//void QuadTree::AllInFrustum()
//{
//	_culled = false;
//	if (nullptr == _pChilds[0])
//	{
//		return;
//	}
//
//	_pChilds[0]->AllInFrustum();
//	_pChilds[1]->AllInFrustum();
//	_pChilds[2]->AllInFrustum();
//	_pChilds[3]->AllInFrustum();
//
//}
