#ifndef TERRAIN_H
#define TERRAIN_H

#include "SingletonBase.h"

class BaseScene;
class QuadTree;

#define TERRAIN_CHUNK_RES (128)

struct TerrainChunkPos
{
	int32 _x{};
	int32 _z{};

	float _relX{};
	float _relZ{};
};

class Terrain : public SingletonBase<Terrain>
{
	friend class Editor;
public:
	struct TerrainConfig
	{
		bool32 _createFromHeightMap;
		bool32 _createFromBuffer;

		int32 _xResolution;
		int32 _zResolution;

		//만약 CreateFromHeightMap이 true라면 heightmap을 읽어서 로드한다...
		//CreateFromBuffer가 true라면 버텍스 버퍼와 인덱스 버퍼를 읽어서 로드 한다.
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

		float _lodRatio{};

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

	struct TerrainChunk
	{
		TerrainChunkPos _chunkPos;

		int32 _chunkX{};
		int32 _chunkZ{};

		float _relStartX{};
		float _relStartZ{};

		float _relEndX{};
		float _relEndZ{};

		float _relCenterX{};
		float _relCenterZ{};

		float _radius{};

		video::VertexBufferHandle _vHandle{};
		video::IndexBufferHandle _iHandle{};

		QuadTree* _pQuadTree{};  //쿼드 트리
		video::TerrainVertex *_pVertices;
	};

	Terrain() {}
	~Terrain();

	void SetScene(BaseScene *pScene) { _pScene = pScene; }
	bool Create(const Terrain::TerrainConfig &config, int32 smoothLevel, bool32 inEditMode);

	void RegisterEvents();
	void Handle(const InputManager::MouseReleasedEvent &event);

	void Destroy();

	void FillRenderCommand(video::RenderView &renderView);
	bool IsIntersectRay(const Ray &ray, Vector3 *pOut);

	float GetHeight(float x, float z);
	float GetSlant(Vector3* pOut, float gravityPower, float x, float z);

	//void ElevateVertex();

private:
	bool CreateTerrain(int32 smooth, int32 tileNum);

	bool CreateTerrainSection(int32 x, int32 z, const video::TerrainVertex *pTerrainVertices);

	void SmoothTerrain(int32 passed);

	video::VertexBufferHandle _vHandle{};
	video::IndexBufferHandle _iHandle{};

	video::VertexDeclHandle _declHandle{};
	video::MaterialHandle _materialHandle{};
	video::EffectHandle _effect{};

private:

	////높이스케일(픽셀컬러가 255 일때 높이) 높이맵 y축 사이 간격 크기
	float _heightScale{};
	float _cellScale{};	//셀간격

	int32 _numTriangleToDraw{};

	bool32 _inEditMode{ false };

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

	//LOD비율
	float _lodRatio{};

	video::TextureHandle _heightMapHandle{};
	video::TextureHandle _tile0Handle{};
	video::TextureHandle _tile1Handle{};
	video::TextureHandle _tile2Handle{};
	video::TextureHandle _tile3Handle{};
	video::TextureHandle _tileSplatHandle{};

	video::TerrainVertex *_terrainVertices{};
	TerrainFace *_terrainFaces{};

	QuadTree* _pQuadTree{};  //쿼드 트리
	BaseScene *_pScene;

	TerrainChunk *_pSections{};
};

#endif
