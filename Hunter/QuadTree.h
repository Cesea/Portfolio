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

	//���� Ʈ�� �ʱ�ȭ
	bool Init(video::TerrainVertex *pVertices, uint32 verNumEdge, int32 sectionRes); 

	void CreateChildTree();

	void GetRayHits(const Ray &ray, std::vector<Vector3>* pOutHit);

	int32 GenerateIndex(uint8 *pData);
	int32 GenerateIndexInternal(int32 triangles, uint8 *pData);

	int32 IsInFrustum(const Frustum &frustum);
	int32 FrustumCull(const Frustum &frustum, int32 &checkLevel);

	IntRect CalculateDrawRange(const Frustum &frustum);
	int32 CalculateDrawRangeInternal(IntRect &range, const Frustum &frustum, QuadTree *pQuad);

private :

private :

	bool32 IsVisible() { return (float)_corners[eCornerRT] - (float)_corners[eCornerLT] <= 1.0f; }

	int32 MapQuadIndexTo2DIndex(int32 level, QuadTree::Corner corner);

	video::TerrainVertex *_pTerrainVertices;

	uint32 _corners[4]; //�ڽ��� ���� Ʈ���� �� �ڳ� ���� �ε���
	uint32 _center;     //�ڽ��� ���� Ʈ���� �־� ���� �ε���
						// -1�̸� �ڽ��� ����.

	QuadTree* _pChilds[4];// ���� Ʈ�� �ڽĵ�

	Vector3 _centerPos; //�ڽ��� ����Ʈ�� �߽� ��ġ
	float _radius; //�ڽ��� ����Ʈ�� ���� ������

	static int32 SectionResolution;

};
#endif
