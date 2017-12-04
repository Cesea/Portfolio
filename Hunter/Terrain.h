#ifndef TERRAIN_H
#define TERRAIN_H

#include "SingletonBase.h"

class IScene;
class QuadTree;

constexpr int32 TERRAIN_SHOW_EXTENT = 2;
constexpr int32 TERRAIN_CHUNK_DIM = 64;
constexpr int32 TERRAIN_ALPHA_TEXTURE_SIZE = 512;

constexpr int32 TERRAIN_TILE_DIM = 8;
constexpr int32 TERRAIN_TILE_RES = 8;
constexpr int32 TERRAIN_CHUNKS_TILE_COUNT = 64;


//터레인 하나의 청크에 대한 위치이다
struct TerrainChunkPos
{
	int32 _x{};
	int32 _z{};

	float _relX{};
	float _relZ{};
};

//청크 안에서
struct TerrainTilePos
{
	int32 _chunkX;
	int32 _chunkZ;

	int32 _tileX{};
	int32 _tileZ{};

	float _relX{};
	float _relZ{};
};

//bool UpdateTerrainTilePos(TerrainTilePos &tilePos)
//{
//	int32 tileXDiff;
//	int32 tileZDiff;
//
//	int32 chunkXDiff;
//	int32 chunkZDiff;
//
//	if (tilePos._relX < 0.0f)
//	{
//		tileXDiff -= 1;
//		tilePos._relX += (float)TERRAIN_TILE_DIM;
//	}
//	else if (tilePos._relX > (float)TERRAIN_TILE_DIM)
//	{
//		tileXDiff += 1;
//		tilePos._relX -= (float)TERRAIN_TILE_DIM;
//	}
//	
//	if (tilePos._relZ < 0.0f)
//	{
//		tileZDiff -= 1;
//		tilePos._relZ += (float)TERRAIN_TILE_DIM;
//	}
//	else if (tilePos._relZ > (float)TERRAIN_TILE_DIM)
//	{
//		tileZDiff += 1;
//		tilePos._relZ -= (float)TERRAIN_TILE_DIM;
//	}
//}


class Terrain : public SingletonBase<Terrain>
{
	friend class Editor;

public:
	struct TerrainConfig
	{
		TerrainConfig() {}
		TerrainConfig(const TerrainConfig &other);
		TerrainConfig &operator= (const TerrainConfig &other);

		int32 _xChunkCount{};
		int32 _zChunkCount{};

		float _textureMult{};

		char _tile0FileName[MAX_FILE_NAME]{ 0, };
		char _tile1FileName[MAX_FILE_NAME]{ 0, };
		char _tile2FileName[MAX_FILE_NAME]{ 0, };
		char _tile3FileName[MAX_FILE_NAME]{ 0, };
		char _splatFileName[MAX_FILE_NAME]{ 0, };
	};

	struct TerrainFace
	{
		uint32 i0;
		uint32 i1;
		uint32 i2;
	};

	struct TerrainTile
	{
		int32 _chunkX{};
		int32 _chunkZ{};

		int32 _tileX{};
		int32 _tileZ{};

		std::vector<Entity> _entities{};
	};

	struct TerrainChunk
	{
		void ActivateEntities();
		void DeactivateEntities();

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

		int32 _numTotalEntity{};

		TerrainTile _tiles[TERRAIN_CHUNKS_TILE_COUNT]{0, };

		const video::TerrainVertex *_pVertices;
	};

	Terrain();
	~Terrain();

	//Event Related /////////////////////////////////////////////////////
	void RegisterEvents();
	void UnRegisterEvents();
	void Handle(const GameObjectFactory::ObjectCreatedEvent &event);

	void SetScene(IScene *pScene) { _pCurrentScene = pScene; }
	bool Create(const Terrain::TerrainConfig &config, bool32 inEditMode);

	void Destroy();

	void SaveTerrain(const std::string &fileName);
	void LoadTerrain(const std::string &fileName);

	bool IsIntersectRay(const Ray &ray, Vector3 *pOut);

	float GetHeight(float x, float z);
	float GetSlant(Vector3* pOut, float gravityPower, float x, float z);

	void Render(const Camera &camera, const DirectionalLight &mainLight, const Camera &lightCameera);
	void RenderShadow(const Camera &camera);

	void AddEntityToSection(const Entity &entity, const Vector3 &position);

	TerrainChunkPos ConvertWorldPosToChunkPos(const Vector3 &worldPos);
	void ConvertWorldPostoTilePos(const Vector3 & worldPos, TerrainTilePos *pOutTilePos);

	//void UpdateTerrainTilePos(TerrainTilePos &tilePos);

	//const Vector3 ConvertChunkPosToWorldPos(const TerrainChunkPos &chunkPos);
	//const Vector3 ConvertTilePosToWorldPos(const TerrainTilePos &tilePos);

	void EffectSetTexture(LPCSTR handle, LPDIRECT3DTEXTURE9 texture);
	void EffectSetMatrix(LPCSTR handle, const Matrix &matrix);

	void SetMainTilePosLink(const BaseGameObject *pObject);

	inline TerrainChunk &GetChunkAt(int32 x, int32 z) 
	{ 
		Assert(x >= 0 && x < _xChunkCount ); Assert(z >= 0 && z < _zChunkCount ); 
		return _pChunks[Index2D(x, z, _xChunkCount)]; 
	}
	inline TerrainChunk &GetChunkAt(int32 index) 
	{
		Assert(index > 0 && index < (_xChunkCount * _zChunkCount) ); 
		return _pChunks[index]; 
	}

	inline int32 GetXChunkCount() { return _xChunkCount; }
	inline int32 GetZChunkCount() { return _zChunkCount; }

private:
	//bool CreateInGame(const Terrain::TerrainConfig &config);
	//bool CreateEdit(const Terrain::TerrainConfig &config);

	bool CreateTerrain(int32 tileNum);
	//Editor Only
	void RebuildTerrain(const Terrain::TerrainConfig &config);
	void ReCreateQuadTree();

	bool CreateTerrainChunk(int32 x, int32 z, const video::TerrainVertex *pTerrainVertices);

	void AddHeightOnCursorPos(const Vector2 &cursorPos, float innerRadius, float outterRadius, float intensity);
	void SmoothOnCursorPos(const Vector2 &cursorPos, float brushRadius);
	void SmoothTerrain();

	void AddHeightGausian(int32 minX, int32 maxX, int32 minZ, int32 maxZ, float mult);
	void AddHeightBlock(int32 minX, int32 maxX, int32 minZ, int32 maxZ, float mult);

	void RebuildSection(int32 minX, int32 maxX, int32 minZ, int32 maxZ);

	void SmoothSection(int32 minX, int32 maxX, int32 minZ, int32 maxZ);

	void DrawAlphaTextureOnCursorPos(const Vector2 & cursorPos, float innerRadius,
		float outterRadius, int32 channel);

	void LoadTextureFromConfig(const Terrain::TerrainConfig &config);

	video::VertexDeclHandle _declHandle{};
	//video::MaterialHandle _materialHandle{};
	video::EffectHandle _effect{};

private:

	////높이스케일(픽셀컬러가 255 일때 높이) 높이맵 y축 사이 간격 크기
	float _heightScale{};

	Terrain::TerrainConfig _currentConfig;

	int32 _numTriangleToDraw{};

	bool32 _inEditMode{ false };

	int32 _numVertexX{};	//가로 정점의수
	int32 _numVertexZ{};	//깊이 정점의수
	int32 _numTotalVertex{}; //총 점점의수

	int32 _numCellX{};	//가로 셀 수
	int32 _numCellZ{};	//깊이 셀 수
	int32 _totalCellNum{}; //총 셀수
	int32 _numTotalFace{};		//삼각형 갯수

	int32 _sectionResolution{ 64 };
	int32 _sectionNumCellX;
	int32 _sectionNumCellZ;
	int32 _sectionNumVertexX;
	int32 _sectionNumVertexZ;

	int32 _xChunkCount;
	int32 _zChunkCount;

	float	_terrainSizeX{};
	float	_terrainSizeZ{};
	float _terrainHalfSizeX{};
	float _terrainHalfSizeZ{};

	//정확한 높이 계산을 하기 위해
	float   _terrainStartX{};
	float   _terrainStartZ{};
	float	_terrainEndX{};
	float	_terrainEndZ{};

	//LOD비율
	float _lodRatio{};

	video::TextureHandle _tile0Handle{};
	video::TextureHandle _tile1Handle{};
	video::TextureHandle _tile2Handle{};
	video::TextureHandle _tile3Handle{};
	video::TextureHandle _tileSplatHandle{};

	video::TerrainVertex *_terrainVertices{};
	TerrainFace *_chunkIndex{};

	QuadTree* _pQuadTree{};  //쿼드 트리
	IScene *_pCurrentScene;

	TerrainChunk *_pChunks{};

	//플레이어의 청크위치를 받으면서 월드를 업데이트 시킨다
	const TerrainTilePos *_pMainTilePos{};
};

#define TERRAIN Terrain::GetInstance()

#endif
