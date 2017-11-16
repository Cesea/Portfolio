#include "stdafx.h"
#include "QuadTree.h"

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

bool QuadTree::Init(video::TerrainVertex *pVertices, uint32 verNumEdge)
{
	_pTerrainVertices = pVertices;

	_corners[eCornerLT] = 0;
	_corners[eCornerRT] = verNumEdge - 1;
	_corners[eCornerLB] = (verNumEdge - 1) * verNumEdge;
	_corners[eCornerRB] = verNumEdge * verNumEdge - 1;

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

																				//좌상단 자식
		_pChilds[eCornerLT] = new QuadTree;
		_pChilds[eCornerLT]->_corners[eCornerLT] = _corners[eCornerLT];
		_pChilds[eCornerLT]->_corners[eCornerRT] = topCenter;
		_pChilds[eCornerLT]->_corners[eCornerLB] = leftCenter;
		_pChilds[eCornerLT]->_corners[eCornerRB] = _center;
		_pChilds[eCornerLT]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerLT]->CreateChildTree();

		//우상단 자식
		_pChilds[eCornerRT] = new QuadTree;
		_pChilds[eCornerRT]->_corners[eCornerLT] = topCenter;
		_pChilds[eCornerRT]->_corners[eCornerRT] = _corners[eCornerRT];
		_pChilds[eCornerRT]->_corners[eCornerLB] = _center;
		_pChilds[eCornerRT]->_corners[eCornerRB] = rightCenter;
		_pChilds[eCornerRT]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerRT]->CreateChildTree();

		//좌하단 자식
		_pChilds[eCornerLB] = new QuadTree;
		_pChilds[eCornerLB]->_corners[eCornerLT] = leftCenter;
		_pChilds[eCornerLB]->_corners[eCornerRT] = _center;
		_pChilds[eCornerLB]->_corners[eCornerLB] = _corners[eCornerLB];
		_pChilds[eCornerLB]->_corners[eCornerRB] = bottomCenter;
		_pChilds[eCornerLB]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerLB]->CreateChildTree();

		//우하단 자식
		_pChilds[eCornerRB] = new QuadTree;
		_pChilds[eCornerRB]->_corners[eCornerLT] = _center;
		_pChilds[eCornerRB]->_corners[eCornerRT] = rightCenter;
		_pChilds[eCornerRB]->_corners[eCornerLB] = bottomCenter;
		_pChilds[eCornerRB]->_corners[eCornerRB] = _corners[eCornerRB];
		_pChilds[eCornerRB]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerRB]->CreateChildTree();
	}
}

void QuadTree::GetRayHits(const Ray * pRay, std::vector<Vector3>* pOutHit)
{
	//TODO : Collision Utils을 사용하여 계산하도록 하자....
	//if (PHYSICS_MGR->IsRayHitSphere(pRay, &_centerPos, this->_radius, NULL, NULL))
	//{
	//	//자식이 있냐?
	//	if ((_corners[eCornerRT] - _corners[eCornerLT]) > 1)
	//	{
	//		//자식 재귀
	//		for (int32 i = 0; i < 4; i++)
	//		{
	//			_pChilds[i]->GetRayHits(pRay, pOutHit);
	//		}
	//	}
	//	//자식이 없는 마지막 노드일떄
	//	else
	//	{
	//		//lt--rt
	//		//|   /|
	//		//|  / |
	//		//| /  |
	//		//|/   |
	//		//lb--rb

	//		//나(타일)의 폴리곤과 Ray의 hit지점을 얻자
	//		float dist = 0.0f;
	//		Vector3 lt = this->_pTerrainVertices[_corners[eCornerLT]]._pos;
	//		Vector3 rt = this->_pTerrainVertices[_corners[eCornerRT]]._pos;
	//		Vector3 lb = this->_pTerrainVertices[_corners[eCornerLB]]._pos;
	//		Vector3 rb = this->_pTerrainVertices[_corners[eCornerRB]]._pos;

	//		//좌상단 폴리곤 체크
	//		if (D3DXIntersectTri(
	//			&lt, &rt, &lb,
	//			&pRay->origin,
	//			&pRay->direction,
	//			nullptr, nullptr,
	//			&dist))			//히트가 되었다면 origin에서 부터 hit위치까지의 거리
	//		{
	//			//히트 지점
	//			Vector3 hitPos = pRay->origin + pRay->direction * dist;

	//			//푸쉬
	//			pOutHit->push_back(hitPos);
	//			return;
	//		}
	//		//우하단 폴리곤 체크
	//		if (D3DXIntersectTri(
	//			&rt, &rb, &lb,
	//			&pRay->origin,
	//			&pRay->direction,
	//			nullptr, nullptr,
	//			&dist))			//히트가 되었다면 origin에서 부터 hit위치까지의 거리
	//		{
	//			//히트 지점
	//			Vector3 hitPos = pRay->origin + pRay->direction * dist;
	//			//푸쉬
	//			pOutHit->push_back(hitPos);
	//			return;
	//		}
	//	}
	//}
}
