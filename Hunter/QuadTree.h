#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "Terrain.h"

class QuadTree
{
public :
	enum Corner 
	{
		eCornerLT = 0,
		eCornerRT,
		eCornerLB,
		eCornerRB,
	};

	//enum QuadLocation
	//{
	//	eFrustumOut = 0,
	//	eFrustumPartiallyIn = 1,
	//	eFrustumCompletlyIn = 2,
	//	eFrustumUnkown = -1, 	
	//};

	//enum {EDGE_UP = 0, EDGE_DOWN, EDGE_LEFT, EDGE_RIGHT};

public:
	QuadTree();
	~QuadTree();

	//쿼드 트리 초기화
	bool Init(video::TerrainVertex *pVertices, uint32 verNumEdge, int32 minRadius); 

	void CreateChildTree();


	void GetRayHits(const Ray &ray, std::vector<Vector3>* pOutHit);

	//int32 GenerateIndex(uint8 *pIndexData, const Frustum &frustum, const Vector3 &camPos, float LODRatio);
	//int32 IsInFrustum(const Frustum &frustum);
	//void FrustumCull(const Frustum &frustum);

	//float GetDistance(const Vector3 &v1, const Vector3 &v2);
	//int32 GetLODLevel(const Vector3 &cameraPos, float LODRatio);

private :

private :

	//void BuildNeighborNode(QuadTree *pRoot, int32 cx);
	//QuadTree *FindNode(int32 i0, int32 i1, int32 i2, int32 i3);;
	//int32 GetNodeIndex(int32 edge, int32 cs, int32 &i0, int32 &i1, int32 &i2, int32 &i3);
	//void AllInFrustum();

	//int32 GenerateTriIndex(int32 numTri, uint8 *pIndexData, const Vector3 &camPos, float LODRatio);

	//bool32 IsVisible(const Vector3 &camPos, float LODRatio);

	video::TerrainVertex *_pTerrainVertices;
	//QuadTree *_pParent;
	//QuadTree *_pNeighbors[4];

	uint32 _corners[4]; //자신의 쿼드 트리의 각 코너 정점 인덱스
	uint32 _center;     //자신의 쿼드 트리의 주앙 정점 인덱스
						// -1이면 자식이 없다.

	QuadTree* _pChilds[4];// 쿼드 트리 자식들

	Vector3 _centerPos; //자신의 쿼드트리 중심 위치
	float _radius; //자신의 쿼드트리 영역 반지름

	int32 _minRadius{};
	//bool32 _culled{false};
};
#endif
