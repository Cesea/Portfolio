#ifndef TERRAIN_H
#define TERRAIN_H

#include "SingletonBase.h"
#include "GameObjectFactory.h"

class IScene;
class QuadTree;

constexpr int32 TERRAIN_SHOW_EXTENT = 2;
constexpr int32 TERRAIN_CHUNK_DIM = 64;
constexpr int32 TERRAIN_ALPHA_TEXTURE_SIZE = 1024;

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
	TerrainTilePos();
	TerrainTilePos(const TerrainTilePos &other);
	TerrainTilePos & operator=(const TerrainTilePos &other);

	bool operator==(const TerrainTilePos &other);

	int32 _chunkX;
	int32 _chunkZ;

	int32 _tileX;
	int32 _tileZ;

	float _relX;
	float _relZ;
};

struct TerrainVertexPos
{
	int32 _chunkX{};
	int32 _chunkZ{};

	int32 _tileX{};
	int32 _tileZ{};

	float _relX{};
	float _relZ{};
};

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
		char _control1Name[MAX_FILE_NAME]{ 0, };
		char _control2Name[MAX_FILE_NAME]{ 0, };
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

		float _startX{};
		float _startZ{};

		float _endX{};
		float _endZ{};

		float _centerX{};
		float _centerZ{};

		float _radius{};

		std::vector<Entity> _entities{};
	};

	struct TerrainChunk
	{
		void ActivateEntities();
		void DeactivateEntities();

		TerrainChunkPos _chunkPos;

		int32 _chunkX{};
		int32 _chunkZ{};

		float _startX{};
		float _startZ{};

		float _endX{};
		float _endZ{};

		float _centerX{};
		float _centerZ{};

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
	void LoadTerrain(const std::string &fileName, bool editMode);

	bool IsIntersectRay(const Ray &ray, Vector3 *pOut);

	float GetHeight(float x, float z);
	float GetSlant(Vector3* pOut, float gravityPower, float x, float z);

	void Render(const Camera &camera, const DirectionalLight &mainLight, const Camera &lightCameera);
	void RenderShadow(const Camera &camera);

	TerrainChunkPos ConvertWorldPosToChunkPos(const Vector3 &worldPos);
	void ConvertWorldPostoTilePos(const Vector3 & worldPos, TerrainTilePos *pOutTilePos);
	void ConvertWorldPostoVertexPos(const Vector3 &worldPos, TerrainVertexPos *pOutVertexPos);

	void ValidateTerrainChunks(const TerrainTilePos &currentPos, const TerrainTilePos &prevPos);

	//void UpdateTerrainTilePos(TerrainTilePos &tilePos);
	//const Vector3 ConvertChunkPosToWorldPos(const TerrainChunkPos &chunkPos);
	//const Vector3 ConvertTilePosToWorldPos(const TerrainTilePos &tilePos);

	void EffectSetTexture(LPCSTR handle, LPDIRECT3DTEXTURE9 texture);
	void EffectSetMatrix(LPCSTR handle, const Matrix &matrix);

	inline TerrainChunk &GetChunkAt(int32 x, int32 z) 
	{ 
		Assert(x >= 0 && x < _xChunkCount ); Assert(z >= 0 && z < _zChunkCount ); 
		return _pChunks[Index2D(x, z, _xChunkCount)]; 
	}
	inline TerrainChunk &GetChunkAt(int32 index) 
	{
		Assert(index >= 0 && index < ((_xChunkCount) * (_zChunkCount)));
		return _pChunks[index]; 
	}

	bool IsWorldPositionInTerrain(const Vector3 &worldPosition);
	bool IsTileHasCollisionObject(const Vector3 worldPosition);
	bool IsTileHasCollisionObject(const TerrainTilePos &tilePosition);
	bool IsTileHasCollisionObject(const TerrainTilePos &tilePosition, Entity &testObject);
	bool IsObjectExistInRange(const Vector3 &start, const Vector3 &end, Entity &testObject);


	inline int32 GetXChunkCount() { return _xChunkCount; }
	inline int32 GetZChunkCount() { return _zChunkCount; }

	inline std::vector<int32> &GetActiveTerrainChunkIndices() { return _activeChunkIndices; }
	inline std::vector<Terrain::TerrainTile *> &GetVisibleTerrainTiles() { return _visibleTiles; }
	inline std::vector<Terrain::TerrainTile *> &GetShadowVisibleTerrainTiles() { return _shadowVisibleTiles; }

	void RemoveEntityInTile(Entity entity, const TerrainTilePos &tilePos);

	void ReTilelizeTilePosition(TerrainTilePos &tilePos);
	bool IsTerrainTilePosValid(const TerrainTilePos &tilePos);

	inline Terrain::TerrainTile *GetTileAt(const TerrainTilePos &tilePos)
	{
		Terrain::TerrainChunk &refChunk = _pChunks[Index2D(tilePos._chunkX, tilePos._chunkZ, _xChunkCount)];
		return &refChunk._tiles[Index2D(tilePos._tileX, tilePos._tileZ, TERRAIN_TILE_RES)];
	}

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
		float outterRadius, int32 layer, bool32 subtract);

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

	video::TextureHandle _tile0Handle{};
	video::TextureHandle _tile1Handle{};
	video::TextureHandle _tile2Handle{};
	video::TextureHandle _tileControl1Handle{};
	video::TextureHandle _tileControl2Handle{};

	video::TerrainVertex *_terrainVertices{};
	TerrainFace *_chunkIndex{};

	QuadTree* _pQuadTree{};  //쿼드 트리
	IScene *_pCurrentScene;

	TerrainChunk *_pChunks{};

	std::vector<int32> _activeChunkIndices;
	std::vector<Terrain::TerrainChunk *> _visibleChunks;
	std::vector<Terrain::TerrainTile *> _visibleTiles;

	std::vector<Terrain::TerrainChunk *> _shadowVisibleChunks;
	std::vector<Terrain::TerrainTile *> _shadowVisibleTiles;
};

#define TERRAIN Terrain::GetInstance()

#endif
