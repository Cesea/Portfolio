#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "Terrain.h"

class QuadTree
{
public :
	enum Corner 
	{
		eCornerLB = 0,
		eCornerRB,
		eCornerLT,
		eCornerRT,
	};

	enum QuadLocation
	{
		eFrustumOut = 0,
		eFrustumPartiallyIn = 1,
		eFrustumCompletlyIn = 2,
		eFrustumUnkown = -1, 	
	};

public:
	QuadTree();
	~QuadTree();

	//쿼드 트리 초기화
	bool Init(video::TerrainVertex *pVertices, uint32 verNumEdge, int32 sectionRes); 

	void CreateChildTree();

	void GetRayHits(const Ray &ray, std::vector<Vector3>* pOutHit);

	int32 GenerateIndex(uint8 *pIndexData, const Frustum &frustum);
	//int32 GenerateIndexInternal(int32 triangles, uint8 *pData);

	int32 IsInFrustum(const Frustum &frustum);
	void FrustumCull(const Frustum &frustum);

private :

private :

	int32 GenerateTriIndex(int32 numTri, uint8 *pIndexData);

	bool32 IsVisible() { return (float)_corners[eCornerRT] - (float)_corners[eCornerLT] <= 1.0f; }

	int32 MapQuadIndexTo2DIndex(int32 level, QuadTree::Corner corner);

	video::TerrainVertex *_pTerrainVertices;

	uint32 _corners[4]; //자신의 쿼드 트리의 각 코너 정점 인덱스
	uint32 _center;     //자신의 쿼드 트리의 주앙 정점 인덱스
						// -1이면 자식이 없다.

	QuadTree* _pChilds[4];// 쿼드 트리 자식들

	Vector3 _centerPos; //자신의 쿼드트리 중심 위치
	float _radius; //자신의 쿼드트리 영역 반지름
	bool32 _culled{false};

	static int32 SectionResolution;

};
#endif
