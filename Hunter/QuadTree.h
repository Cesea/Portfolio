#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "Terrain.h"

class QuadTree
{
private:
	enum Corner 
	{
		eCornerLT = 0,
		eCornerRT,
		eCornerLB,
		eCornerRB,
	};

	video::TerrainVertex *_pTerrainVertices;

	uint32 _corners[4]; //자신의 쿼드 트리의 각 코너 정점 인덱스
	uint32 _center;     //자신의 쿼드 트리의 주앙 정점 인덱스
						// -1이면 자식이 없다.

	QuadTree* _pChilds[4];// 쿼드 트리 자식들

	Vector3 _centerPos; //자신의 쿼드트리 중심 위치
	float _radius; //자신의 쿼드트리 영역 반지름
public:
	QuadTree();
	~QuadTree();

	//쿼드 트리 초기화
	bool Init(video::TerrainVertex *pVertices, uint32 verNumEdge); 

	void CreateChildTree();

	void GetRayHits(const Ray *pRay, std::vector<Vector3>* pOutHit);

};
#endif
