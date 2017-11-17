#ifndef TERRAIN_H
#define TERRAIN_H

#include "SingletonBase.h"

class QuadTree;

class Terrain : public SingletonBase<Terrain>
{
public:
	struct TerrainConfig
	{
		std::string _heightFileName;
		std::string _tile0FileName;
		std::string _tile1FileName;
		std::string _tile2FileName;
		std::string _tile3FileName;
		std::string _splatFileName;

		float _cellScale;
		float _heightScale;
		//uv�� �󸶳� �ݺ� �� ������...
		float _textureMult;

		//TerrainSection�� �ػ󵵸� �����Ѵ�...	
		//Cell�� �����̴�
		//�⺻ ���� 64...
		int32 _sectionResolution{64};
	};

	struct TerrainFace 
	{
		uint32 i0;
		uint32 i1;
		uint32 i2;
	};

	struct TerrainSection
	{
		int32 _indexX;
		int32 _indexZ;

		float _startX;
		float _startZ;

		float _endX;
		float _endZ;

		float _centerX;
		float _centerZ;

		float _radius{};

		video::VertexBufferHandle _vHandle;
		video::IndexBufferHandle _iHandle;
	};

	Terrain() {}
	~Terrain();

	bool Create(const Terrain::TerrainConfig &config, int32 smoothLevel);

	void Destroy();
	void Render(video::RenderView &renderView);
	bool IsIntersectRay(const Ray &ray, Vector3 *pOut);

	float GetHeight(float x, float z);

	float GetSlant(Vector3* pOut, float gravityPower, float x, float z);
private:
	bool CreateTerrain(int32 smooth, int32 tileNum);

	bool CreateTerrainSection(int32 x, int32 z, const video::TerrainVertex *pTerrainVertices);

	void SmoothTerrain(int32 passed);


	video::VertexDeclHandle _declHandle{};
	video::MaterialHandle _mHandle{};
	video::VertexBufferHandle _vHandle{};
	video::IndexBufferHandle _iHandle{};

	video::EffectHandle _effect{};

private:

	//���̽�����(�ȼ��÷��� 255 �϶� ����) ���̸� y�� ���� ���� ũ��
	float _heightScale{};
	float _cellScale{};	//������

	int32 _numVertexX{};	//���� �����Ǽ�
	int32 _numVertexZ{};	//���� �����Ǽ�
	int32 _numTotalVertex{}; //�� �����Ǽ�

	int32 _numCellX{};	//���� �� ��
	int32 _numCellZ{};	//���� �� ��
	int32 _totalCellNum{}; //�� ����
	int32 _numTotalFace{};		//�ﰢ�� ����

	int32 _sectionResolution{64};
	int32 _sectionNumCellX;
	int32 _sectionNumCellZ;
	int32 _sectionNumVertexX;
	int32 _sectionNumVertexZ;
	int32 _numSectionX;
	int32 _numSectionZ;

	float	_terrainSizeX{};
	float	_terrainSizeZ{};

	//��Ȯ�� ���� ����� �ϱ� ����
	float   _terrainStartX{};
	float   _terrainStartZ{};
	float	_terrainEndX{};
	float	_terrainEndZ{};

	video::TextureHandle _heightMapHandle{};
	video::TextureHandle _tile0Handle{};
	video::TextureHandle _tile1Handle{};
	video::TextureHandle _tile2Handle{};
	video::TextureHandle _tile3Handle{};
	video::TextureHandle _tileSplatHandle{};

	video::TerrainVertex *_terrainVertices{};
	QuadTree* _pQuadTree{};  //���� Ʈ��

	TerrainSection *_pSections{};
};

#define TERRAIN Terrain::GetInstance()

#endif
