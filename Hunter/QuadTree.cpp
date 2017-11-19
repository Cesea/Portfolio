#include "stdafx.h"
#include "QuadTree.h"

int32 QuadTree::SectionResolution;

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



bool QuadTree::Init(video::TerrainVertex *pVertices, uint32 verNumEdge, int32 sectionRes)
{
	_pTerrainVertices = pVertices;

	_corners[eCornerLB] = 0;
	_corners[eCornerRB] = verNumEdge - 1;
	_corners[eCornerLT] = (verNumEdge - 1) * verNumEdge;
	_corners[eCornerRT] = verNumEdge * verNumEdge - 1;

	SectionResolution = sectionRes;

	CreateChildTree();
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
	if ((_corners[eCornerRT] - _corners[eCornerLT]) > 1)
	{
		//센터 인덱스 계산
		_center = (_corners[0] + _corners[1] + _corners[2] + _corners[3]) / 4;

		uint32 topCenter = (_corners[eCornerRT] + _corners[eCornerLT]) / 2; //상단 중앙
		uint32 leftCenter = (_corners[eCornerLT] + _corners[eCornerLB]) / 2; //좌 중앙
		uint32 rightCenter = (_corners[eCornerRT] + _corners[eCornerRB]) / 2; //우 중앙
		uint32 bottomCenter = (_corners[eCornerRB] + _corners[eCornerLB]) / 2; //하단 중앙

		//좌하단 자식
		_pChilds[eCornerLB] = new QuadTree;
		_pChilds[eCornerLB]->_corners[eCornerLT] = leftCenter;
		_pChilds[eCornerLB]->_corners[eCornerRT] = _center;
		_pChilds[eCornerLB]->_corners[eCornerLB] = _corners[eCornerLB];
		_pChilds[eCornerLB]->_corners[eCornerRB] = bottomCenter;
		_pChilds[eCornerLB]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerLB]->_culled = false;
		_pChilds[eCornerLB]->CreateChildTree();

		//우하단 자식
		_pChilds[eCornerRB] = new QuadTree;
		_pChilds[eCornerRB]->_corners[eCornerLT] = _center;
		_pChilds[eCornerRB]->_corners[eCornerRT] = rightCenter;
		_pChilds[eCornerRB]->_corners[eCornerLB] = bottomCenter;
		_pChilds[eCornerRB]->_corners[eCornerRB] = _corners[eCornerRB];
		_pChilds[eCornerRB]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerRB]->_culled = false;
		_pChilds[eCornerRB]->CreateChildTree();
																				//좌상단 자식
		//좌상단
		_pChilds[eCornerLT] = new QuadTree;
		_pChilds[eCornerLT]->_corners[eCornerLT] = _corners[eCornerLT];
		_pChilds[eCornerLT]->_corners[eCornerRT] = topCenter;
		_pChilds[eCornerLT]->_corners[eCornerLB] = leftCenter;
		_pChilds[eCornerLT]->_corners[eCornerRB] = _center;
		_pChilds[eCornerLT]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerLT]->_culled = false;
		_pChilds[eCornerLT]->CreateChildTree();

		//우상단 자식
		_pChilds[eCornerRT] = new QuadTree;
		_pChilds[eCornerRT]->_corners[eCornerLT] = topCenter;
		_pChilds[eCornerRT]->_corners[eCornerRT] = _corners[eCornerRT];
		_pChilds[eCornerRT]->_corners[eCornerLB] = _center;
		_pChilds[eCornerRT]->_corners[eCornerRB] = rightCenter;
		_pChilds[eCornerRT]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerRT]->_culled = false;
		_pChilds[eCornerRT]->CreateChildTree();
	}
}

void QuadTree::GetRayHits(const Ray &ray, std::vector<Vector3> *pOutHit)
{
	if (IsRayHitBoundSphere(ray, _centerPos, _radius, NULL, NULL))
	{
		//자식이 있냐?
		if ((_corners[eCornerRT] - _corners[eCornerLT]) > 1)
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

int32 QuadTree::GenerateIndex(uint8 * pIndexData, const Frustum & frustum)
{
	FrustumCull(frustum);
	return GenerateTriIndex(0, pIndexData);
}

int32 QuadTree::IsInFrustum(const Frustum & frustum)
{
	bool32 cornerIn[4];

	Vector3 currentPoint = (_pTerrainVertices + _center)->_pos;
	if (frustum.IsSphereInFrustum(currentPoint, _radius))
	{
		cornerIn[0] = frustum.IsPointIntFrustum((_pTerrainVertices + _corners[0])->_pos);
		cornerIn[1] = frustum.IsPointIntFrustum((_pTerrainVertices + _corners[1])->_pos);;
		cornerIn[2] = frustum.IsPointIntFrustum((_pTerrainVertices + _corners[2])->_pos);;
		cornerIn[3] = frustum.IsPointIntFrustum((_pTerrainVertices + _corners[3])->_pos);;

		if (cornerIn[0] + cornerIn[1] + cornerIn[2] + cornerIn[3] == 4)
		{
			return FRUSTUM_COMPLETLY_IN;
		}
		else
		{
			return FRUSTUM_PARTIALLY_IN;
		}
	}
	else
	{
		return FRUSTUM_OUT;
	}
}

void QuadTree::FrustumCull(const Frustum & frustum)
{
	int32 result;
	result = IsInFrustum(frustum);
	switch (result)
	{
		//절두체에 완전히 포함 됨, 하위 노드 검색 필요 없음
	case FRUSTUM_COMPLETLY_IN :
	{
		_culled = false;
		return;
	}break;
	//절두체에 부분적으로 포함 되어서, 하위 노드 검색 필요
	case FRUSTUM_PARTIALLY_IN :
	{
		_culled = false;
	}break;
	//절두체에 완전히 벗어남, 하위 노드 검색 필요 없음
	case FRUSTUM_OUT : 
	{
		_culled = true;
		return;
	}break;
	}

	if (_pChilds[0])
	{
		_pChilds[0]->FrustumCull(frustum);
	}
	if (_pChilds[1])
	{
		_pChilds[1]->FrustumCull(frustum);
	}
	if (_pChilds[2])
	{
		_pChilds[2]->FrustumCull(frustum);
	}
	if (_pChilds[3])
	{
		_pChilds[3]->FrustumCull(frustum);
	}
}

int32 QuadTree::GenerateTriIndex(int32 numTri, uint8 * pIndexData)
{
	if (_culled)
	{
		_culled = false;
		return numTri;
	}

	if (IsVisible())
	{
		uint32 *p = ((uint32 *)pIndexData) + numTri * 3;
		// 좌측상단 삼각형
		*p++ = _corners[eCornerLB];
		*p++ = _corners[eCornerLT];
		*p++ = _corners[eCornerRT];
		numTri++;
		// 우측하단 삼각형
		*p++ = _corners[eCornerLB];
		*p++ = _corners[eCornerRT];
		*p++ = _corners[eCornerRB];
		numTri++;

		return numTri;
	}

	if (_pChilds[eCornerLB])
	{
		numTri = _pChilds[eCornerLB]->GenerateTriIndex(numTri, pIndexData);
	}
	if (_pChilds[eCornerRB])
	{
		numTri = _pChilds[eCornerRB]->GenerateTriIndex(numTri, pIndexData);
	}
	if (_pChilds[eCornerLT])
	{
		numTri = _pChilds[eCornerLT]->GenerateTriIndex(numTri, pIndexData);
	}
	if (_pChilds[eCornerRT])
	{
		numTri = _pChilds[eCornerRT]->GenerateTriIndex(numTri, pIndexData);
	}
	return numTri;
}

int32 QuadTree::MapQuadIndexTo2DIndex(int32 level, QuadTree::Corner corner)
{
	int32 grid = pow(2, level);

	return int32();
}
