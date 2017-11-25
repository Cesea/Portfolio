#ifndef TERRAIN_H
#define TERRAIN_H

class BaseScene;
class QuadTree;

constexpr int32 TERRAIN_CHUNK_DIM = 64;

constexpr int32 TERRAIN_HORI_SIZE = 1024;
constexpr int32 TERRAIN_VERT_SIZE = 1024;

constexpr int32 TERRAIN_HORI_HALF_SIZE = TERRAIN_HORI_SIZE / 2;
constexpr int32 TERRAIN_VERT_HALF_SIZE = TERRAIN_VERT_SIZE / 2;

constexpr int32 TERRAIN_HORI_CHUNK_COUNT = TERRAIN_HORI_SIZE / TERRAIN_CHUNK_DIM;
constexpr int32 TERRAIN_VERT_CHUNK_COUNT = TERRAIN_VERT_SIZE / TERRAIN_CHUNK_DIM;

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

TerrainChunkPos ConvertWorldPosToChunkPos(const Vector3 &worldPos);
TerrainTilePos ConvertWorldPostoTilePos(const Vector3 &worldPos);

const Vector3 ConvertChunkPosToWorldPos(const TerrainChunkPos &chunkPos);
const Vector3 ConvertTilePosToWorldPos(const TerrainTilePos &tilePos);

class Terrain 
{
	friend class Editor;

public:
	struct TerrainConfig
	{
		bool32 _createFromHeightMap;
		bool32 _createFromBuffer;

		int32 _xResolution;
		int32 _zResolution;

		//���� CreateFromHeightMap�� true��� heightmap�� �о �ε��Ѵ�...
		//CreateFromBuffer�� true��� ���ؽ� ���ۿ� �ε��� ���۸� �о �ε� �Ѵ�.
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

		float _lodRatio{};

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

		//QuadTree* _pQuadTree{};  //���� Ʈ��
		video::TerrainVertex *_pVertices;

		std::vector<Entity> _entities;
	};

	Terrain() {}
	~Terrain();

	void SetScene(BaseScene *pScene) { _pCurrentScene = pScene; }
	bool Create(const Terrain::TerrainConfig &config, int32 smoothLevel, bool32 inEditMode);

	void RegisterEvents();

	void Destroy();

	bool IsIntersectRay(const Ray &ray, Vector3 *pOut);

	float GetHeight(float x, float z);
	float GetSlant(Vector3* pOut, float gravityPower, float x, float z);

	void Render(const Camera &camear);

	void AddEntityToSection(const Entity &entity, const Vector3 &position);

private:
	bool CreateTerrain(int32 smooth, int32 tileNum);

	bool CreateTerrainSection(int32 x, int32 z, const video::TerrainVertex *pTerrainVertices);

	void SmoothTerrain(int32 passed);

	//video::VertexBufferHandle _vHandle{};
	//video::IndexBufferHandle _iHandle{};

	video::VertexDeclHandle _declHandle{};
	//video::MaterialHandle _materialHandle{};
	video::EffectHandle _effect{};

private:

	////���̽�����(�ȼ��÷��� 255 �϶� ����) ���̸� y�� ���� ���� ũ��
	float _heightScale{};
	float _cellScale{};	//������

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
	int32 _numSectionX;
	int32 _numSectionZ;

	float	_terrainSizeX{};
	float	_terrainSizeZ{};

	//��Ȯ�� ���� ����� �ϱ� ����
	float   _terrainStartX{};
	float   _terrainStartZ{};
	float	_terrainEndX{};
	float	_terrainEndZ{};

	//LOD����
	float _lodRatio{};

	video::TextureHandle _heightMapHandle{};
	video::TextureHandle _tile0Handle{};
	video::TextureHandle _tile1Handle{};
	video::TextureHandle _tile2Handle{};
	video::TextureHandle _tile3Handle{};
	video::TextureHandle _tileSplatHandle{};

	video::TerrainVertex *_terrainVertices{};
	TerrainFace *_terrainFaces{};

	QuadTree* _pQuadTree{};  //���� Ʈ��
	BaseScene *_pCurrentScene;

	TerrainChunk *_pChunks{};

	//�÷��̾��� ûũ��ġ�� �����鼭 ���带 ������Ʈ ��Ų��
	//Player *_pPlayer{};
};

#endif
