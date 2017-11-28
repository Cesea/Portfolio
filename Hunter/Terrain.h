#ifndef TERRAIN_H
#define TERRAIN_H

#include "SingletonBase.h"

class IScene;
class QuadTree;

constexpr int32 TERRAIN_SHOW_EXTENT = 2;

constexpr int32 TERRAIN_CHUNK_DIM = 64;

constexpr int32 TERRAIN_ALPHA_TEXTURE_SIZE = 512;

#define MAX_FILE_NAME 256

//constexpr int32 TERRAIN_HORI_SIZE = 1024;
//constexpr int32 TERRAIN_VERT_SIZE = 1024;
//constexpr int32 TERRAIN_HORI_HALF_SIZE = TERRAIN_HORI_SIZE / 2;
//constexpr int32 TERRAIN_VERT_HALF_SIZE = TERRAIN_VERT_SIZE / 2;
//constexpr int32 TERRAIN_HORI_CHUNK_COUNT = TERRAIN_HORI_SIZE / TERRAIN_CHUNK_DIM;
//constexpr int32 TERRAIN_VERT_CHUNK_COUNT = TERRAIN_VERT_SIZE / TERRAIN_CHUNK_DIM;

//�ͷ��� �ϳ��� ûũ�� ���� ��ġ�̴�
struct TerrainChunkPos
{
	int32 _x{};
	int32 _z{};

	float _relX{};
	float _relZ{};
};

//ûũ �ȿ���
struct TerrainTilePos
{
	int32 _chunkX;
	int32 _chunkZ;


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

		char _tile0FileName[MAX_FILE_NAME]{0, };
		char _tile1FileName[MAX_FILE_NAME]{0, };
		char _tile2FileName[MAX_FILE_NAME]{0, };
		char _tile3FileName[MAX_FILE_NAME]{0, };
		char _splatFileName[MAX_FILE_NAME]{0, };
	};

	struct TerrainFace 
	{
		uint32 i0;
		uint32 i1;
		uint32 i2;
	};

	struct TerrainChunk
	{
		void ValidateEntities();
		void InvalidateEntities();

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

		//���࿡ �ͷ��� �����Ϳ��� ���ؽ� ������ ����Ǿ��ٸ� dirty�� true�� �ٲٰ�....
		//smoothing�� ���ϴ� ������ ����Ѵ�??
		bool32 _dirty{false};

		//QuadTree* _pQuadTree{};  //���� Ʈ��
		const video::TerrainVertex *_pVertices;

		std::vector<Entity> _entities;
	};

	Terrain() {}
	~Terrain();

	void SetScene(IScene *pScene) { _pCurrentScene = pScene; }
	bool Create(const Terrain::TerrainConfig &config, bool32 inEditMode);

	void RegisterEvents();

	void Destroy();

	void SaveTerrain(const std::string &fileName);
	void LoadTerrain(const std::string &fileName);

	bool IsIntersectRay(const Ray &ray, Vector3 *pOut);

	float GetHeight(float x, float z);
	float GetSlant(Vector3* pOut, float gravityPower, float x, float z);

	void Render(const Camera &camear);

	void AddEntityToSection(const Entity &entity, const Vector3 &position);

	TerrainChunkPos ConvertWorldPosToChunkPos(const Vector3 &worldPos);
	TerrainTilePos ConvertWorldPostoTilePos(const Vector3 &worldPos);

	//const Vector3 ConvertChunkPosToWorldPos(const TerrainChunkPos &chunkPos);
	const Vector3 ConvertTilePosToWorldPos(const TerrainTilePos &tilePos);

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
	void SmoothTerrain(int32 passed);

	void AddHeightGausian(int32 minX, int32 maxX, int32 minZ, int32 maxZ, float mult);

	void RebuildSection(int32 minX, int32 maxX, int32 minZ, int32 maxZ);

	void SmoothSection(int32 minX, int32 maxX, int32 minZ, int32 maxZ);

	void DrawAlphaTextureOnCursorPos(const Vector2 & cursorPos, float innerRadius, float outterRadius, 
		float intensity, video::TextureHandle alphaHandle, int32 channel);


	void LoadTextureFromConfig(const Terrain::TerrainConfig &config);

	video::VertexDeclHandle _declHandle{};
	//video::MaterialHandle _materialHandle{};
	video::EffectHandle _effect{};

private:

	////���̽�����(�ȼ��÷��� 255 �϶� ����) ���̸� y�� ���� ���� ũ��
	float _heightScale{};

	Terrain::TerrainConfig _currentConfig;

	int32 _numTriangleToDraw{};

	bool32 _inEditMode{ false };

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

	int32 _xChunkCount;
	int32 _zChunkCount;

	float	_terrainSizeX{};
	float	_terrainSizeZ{};
	float _terrainHalfSizeX{};
	float _terrainHalfSizeZ{};

	//��Ȯ�� ���� ����� �ϱ� ����
	float   _terrainStartX{};
	float   _terrainStartZ{};
	float	_terrainEndX{};
	float	_terrainEndZ{};

	//LOD����
	float _lodRatio{};

	video::TextureHandle _tile0Handle{};
	video::TextureHandle _tile1Handle{};
	video::TextureHandle _tile2Handle{};
	video::TextureHandle _tile3Handle{};
	video::TextureHandle _tileSplatHandle{};

	video::TerrainVertex *_terrainVertices{};
	TerrainFace *_chunkIndex{};

	QuadTree* _pQuadTree{};  //���� Ʈ��
	IScene *_pCurrentScene;

	TerrainChunk *_pChunks{};

	//�÷��̾��� ûũ��ġ�� �����鼭 ���带 ������Ʈ ��Ų��
	TerrainTilePos *_mainTilePos;
};

#define TERRAIN Terrain::GetInstance()

#endif
