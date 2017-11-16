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

	uint32 _corners[4]; //�ڽ��� ���� Ʈ���� �� �ڳ� ���� �ε���
	uint32 _center;     //�ڽ��� ���� Ʈ���� �־� ���� �ε���
						// -1�̸� �ڽ��� ����.

	QuadTree* _pChilds[4];// ���� Ʈ�� �ڽĵ�

	Vector3 _centerPos; //�ڽ��� ����Ʈ�� �߽� ��ġ
	float _radius; //�ڽ��� ����Ʈ�� ���� ������
public:
	QuadTree();
	~QuadTree();

	//���� Ʈ�� �ʱ�ȭ
	bool Init(video::TerrainVertex *pVertices, uint32 verNumEdge); 

	void CreateChildTree();

	void GetRayHits(const Ray *pRay, std::vector<Vector3>* pOutHit);

};
#endif
