#ifndef TERRAIN_H
#define TERRAIN_H

class QuadTree;
class Terrain
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
	};

	struct TerrainFace 
	{
		uint32 i0;
		uint32 i1;
		uint32 i2;
	};

	Terrain() {}
	~Terrain();

	bool Create(const Terrain::TerrainConfig &config, int32 smoothLevel);

	void Destroy();
	void Render(const video::Effect &effect, const Matrix &viewProjection);
	bool IsIntersectRay(Ray *pRay, Vector3 pOut);

	float GetHeight(float x, float z);

	float GetSlant(Vector3* pOut, float gravityPower, float x, float z);
private:
	bool CreateTerrain(int32 smooth, int32 tileNum);
	void SmoothTerrain(int32 passed);

private:
	video::VertexBufferHandle _vHandle{};
	video::IndexBufferHandle _iHandle{};
	video::VertexDeclHandle _declHandle{};
	video::MaterialHandle _mHandle{};

	//���̽�����(�ȼ��÷��� 255 �϶� ����) ���̸� y�� ���� ���� ũ��
	float   _heightScale{};
	float	_cellScale{};	//������

	int32		_numVertexX{};	//���� �����Ǽ�
	int32		_numVertexZ{};	//���� �����Ǽ�
	int32		_numTotalVertex{}; //�� �����Ǽ�

	int32		_numCellX{};	//���� �� ��
	int32		_numCellZ{};	//���� �� ��
	int32		_totalCellNum{}; //�� ����
	int32		_numTotalFace{};		//�ﰢ�� ����

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

	video::EffectHandle _effect{};

	video::TerrainVertex *_terrainVertices{};
	QuadTree* _pQuadTree{};  //���� Ʈ��
};

#endif
