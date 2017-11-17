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
		//uv가 얼마나 반복 될 것인지...
		float _textureMult;

		//TerrainSection의 해상도를 결정한다...	
		//Cell의 갯수이다
		//기본 값이 64...
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

	//높이스케일(픽셀컬러가 255 일때 높이) 높이맵 y축 사이 간격 크기
	float _heightScale{};
	float _cellScale{};	//셀간격

	int32 _numVertexX{};	//가로 정점의수
	int32 _numVertexZ{};	//깊이 정점의수
	int32 _numTotalVertex{}; //총 점점의수

	int32 _numCellX{};	//가로 셀 수
	int32 _numCellZ{};	//깊이 셀 수
	int32 _totalCellNum{}; //총 셀수
	int32 _numTotalFace{};		//삼각형 갯수

	int32 _sectionResolution{64};
	int32 _sectionNumCellX;
	int32 _sectionNumCellZ;
	int32 _sectionNumVertexX;
	int32 _sectionNumVertexZ;
	int32 _numSectionX;
	int32 _numSectionZ;

	float	_terrainSizeX{};
	float	_terrainSizeZ{};

	//정확한 높이 계산을 하기 위해
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
	QuadTree* _pQuadTree{};  //쿼드 트리

	TerrainSection *_pSections{};
};

#define TERRAIN Terrain::GetInstance()

#endif
