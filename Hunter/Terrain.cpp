#include "stdafx.h"
#include "Terrain.h"

#include "QuadTree.h"

#include "BaseScene.h"

Terrain::~Terrain()
{
}

bool Terrain::Create(const Terrain::TerrainConfig &config, int32 smoothLevel)
{
	RegisterEvents();
	//�����ϰ� ����
	_heightScale = config._heightScale;
	_cellScale = config._cellScale;

	//���̸� �ҷ��´�
	_heightMapHandle = VIDEO->CreateTexture(config._heightFileName, config._heightFileName);
	video::Texture *heightMap = VIDEO->GetTexture(_heightMapHandle);

	if (nullptr == heightMap)
	{
		return false;
	}

	D3DSURFACE_DESC sd;
	heightMap->_ptr->GetLevelDesc(0, &sd);

	//���μ��� ���� ���� ���Ѵ�.
	_numVertexX = sd.Width + 1;		
	//���� ���������� ���� �ʿ� ���� �ػ� + 1 �� ����. ( ������ ����Ʈ�������� ���������� 2��N�� + 1 �̿��� �ϱ� ������ )
	_numVertexZ = sd.Height + 1;		
	//���� ���������� ���� �ʿ� ���� �ػ� + 1 �� ����. ( ������ ����Ʈ�������� ���������� 2��N�� + 1 �̿��� �ϱ� ������ )
	_numTotalVertex = _numVertexX * _numVertexZ;		//�� ���� ����

	_numCellX = _numVertexX - 1;
	_numCellZ = _numVertexZ - 1;
	_totalCellNum = _numCellX * _numCellZ;

	//�ͷ��� ũ��
	_terrainSizeX = _numCellX * _cellScale;
	_terrainSizeZ = _numCellZ * _cellScale;

	//�� �ﰢ������
	_numTotalFace = _totalCellNum * 2;

	//Section ����
	_sectionResolution = config._sectionResolution;
		
	_numSectionX = _numCellX / _sectionResolution;
	_numSectionZ = _numCellZ / _sectionResolution;

	_sectionNumCellX = _sectionResolution;
	_sectionNumCellZ = _sectionResolution;

	_sectionNumVertexX = _sectionNumCellX + 1;
	_sectionNumVertexZ = _sectionNumCellZ + 1;

	if (!(CreateTerrain(smoothLevel, config._textureMult))) 
	{
		Destroy();
		return false;
	}

	//�ͷ��� ����
	_terrainStartX = _terrainVertices[0]._pos.x;
	_terrainStartZ = _terrainVertices[0]._pos.z;
	_terrainEndX = _terrainVertices[_numTotalVertex - 1]._pos.x;
	_terrainEndZ = _terrainVertices[_numTotalVertex - 1]._pos.z;

	//����Ʈ���� �����.
	_pQuadTree = new QuadTree;
	_pQuadTree->Init(_terrainVertices, _numVertexX, _sectionResolution);

	//�ͷ��� Texture �ε�
	_effect = VIDEO->GetEffect("TerrainBase.fx");
	_materialHandle = VIDEO->CreateMaterial("TerrainMaterial");

	_tile0Handle = VIDEO->CreateTexture(config._tile0FileName, config._tile0FileName);
	_tile1Handle = VIDEO->CreateTexture(config._tile1FileName, config._tile1FileName);
	_tile2Handle = VIDEO->CreateTexture(config._tile2FileName, config._tile2FileName);
	_tile3Handle = VIDEO->CreateTexture(config._tile3FileName, config._tile3FileName);
	_tileSplatHandle = VIDEO->CreateTexture(config._splatFileName);

	VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE0, _tile0Handle);
	VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE1, _tile1Handle);
	VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE2, _tile2Handle);
	VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE3, _tile3Handle);
	VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE4, _tileSplatHandle);

}

void Terrain::RegisterEvents()
{
	EventChannel channel;
	channel.Add<InputManager::MouseReleasedEvent, Terrain>(*this);
}

void Terrain::Handle(const InputManager::MouseReleasedEvent & event)
{
	EventChannel channel;

	Ray ray;
	_pScene->_camera.ComputeRay(Vector2(event.coord.x, event.coord.y), &ray);
	Vector3 hitPos;
	if (this->IsIntersectRay(ray, &hitPos))
	{
		//channel.Queue<BaseScene::SpawnEvent>(new BaseScene::SpawnEvent(hitPos));
	}
}

void Terrain::Destroy()
{
	_pScene = nullptr;
	for (int32 i = 0; i < _numSectionX * _numSectionZ; ++i)
	{
		VIDEO->DestroyVertexBuffer(_pSections[i]._vHandle);
		VIDEO->DestroyIndexBuffer(_pSections[i]._iHandle);
	}
	SAFE_DELETE_ARRAY(_pSections);

	VIDEO->DestroyVertexDecl(_declHandle);

	VIDEO->DestroyTexture(_tile0Handle);
	VIDEO->DestroyTexture(_tile1Handle);
	VIDEO->DestroyTexture(_tile2Handle);
	VIDEO->DestroyTexture(_tile3Handle);
	VIDEO->DestroyTexture(_tileSplatHandle);

	VIDEO->DestroyMaterial(_materialHandle);

	SAFE_DELETE_ARRAY(_terrainVertices);
	SAFE_DELETE(_pQuadTree);
}

void Terrain::FillRenderCommand(video::RenderView & renderView)
{

	IntRect drawRange = _pQuadTree->CalculateDrawRange(renderView._pCamera->GetFrustum());

	//for (int32 y = drawRange._bottom; y < drawRange._top; ++y)
	//{
	//	for (int32 x = drawRange._left; x < drawRange._right; ++x)
	//	{
	for (int32 i = 0; i < _numSectionX * _numSectionZ; ++i)
	{
		Terrain::TerrainSection &refSection = _pSections[i];
		if (renderView._pCamera->GetFrustum().IsSphereInFrustum(Vector3(refSection._centerX, 0.0f, refSection._centerZ), refSection._radius))
		{
			video::RenderCommand &command = renderView.GetCommand();

			command._drawType = video::RenderCommand::DrawType::eStatic;
			command._primType = video::RenderCommand::PrimType::eTriangleList;

			Assert(refSection._vHandle.IsValid());
			Assert(refSection._iHandle.IsValid());
			command._vHandle = refSection._vHandle;
			command._iHandle = refSection._iHandle;

			command._effectHandle = _effect;
			command._materialHandle = _materialHandle;
		}
	}
}

//TODO : Implement this
bool Terrain::IsIntersectRay(const Ray &ray, Vector3 *pOutHit)
{
	std::vector<Vector3> hits;

	//�ֻ�� ���� Ʈ�� ���� Ray Check ����.
	_pQuadTree->GetRayHits(ray, &hits);

	if (hits.size() == 0)
	{
		return false;
	}
	//���� ó�� ��ġȭ ó�� ��ġ�� ���� �Ÿ� ����
	Vector3 hitPos = hits[0];
	Vector3 dir = hitPos - ray.origin;
	float distSq = Vec3LengthSq(&dir);

	for (int i = 1; i < hits.size(); i++)
	{
		dir = hits[i] - ray.origin;
		float newDist = Vec3LengthSq(&dir);
		if (newDist < distSq)
		{
			distSq = newDist;
			hitPos = hits[i];
		}
	}
	*pOutHit = hitPos;
	return true;
}

float Terrain::GetHeight(float x, float z)
{
	//�ͷ��� ������ �Ѿ�� 0.0 ���� �����Ѵ�
	if (x < _terrainStartX || x > _terrainEndX ||
		z < _terrainStartZ || z > _terrainEndZ) 
	{
		return 0.0f;
	}

	//Terrain �� ���ϴ� 0 �� �������� ���� Terrain �� ������ ��ġ�� ã��
	float pX = x - _terrainStartX;
	float pZ = z - _terrainStartZ;

	//�ش� ��ġ�� ��� ���� ���ԵǴ��� �ľ�
	float invCell = 1.0f / _cellScale;
	pX *= invCell;
	pZ *= invCell;

	//�ش� ��ġ�� �� �ε���
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//���� �ױ��� ������ ��´�.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	Vector3 lb = _terrainVertices[idxZ * _numVertexX + idxX]._pos;
	Vector3 rb = _terrainVertices[idxZ * _numVertexX + idxX + 1]._pos;
	Vector3 lt = _terrainVertices[(idxZ + 1) * _numVertexX + idxX]._pos;
	Vector3 rt = _terrainVertices[(idxZ + 1) * _numVertexX + idxX + 1]._pos;

	//�ش� �п����� delta ���� ���Ѵ�.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	//�ش� ���� �»�ܿ� �ִ�?
	if (dX < 1.0f - dZ)
	{
		float deltaU = rt.y - lt.y;	//lt �������� rt ���������� y ��ġ�� ������
		float deltaV = lb.y - lt.y; //lt �������� lb ���������� y ��ġ�� ������

		fHeight = lt.y + (deltaU * dX) + (deltaV * dZ);
	}
	//�ش� ���� ���ϴ��� �ִ�?
	else
	{
		float deltaU = lb.y - rb.y;
		float deltaV = rt.y - rb.y;

		//�쿡�� �·� �Ͽ��� ������ ���������� �ٲ���� ������
		//delta ���� ������ ���Ѵ�.
		dX = 1.0f - dX;
		dZ = 1.0f - dZ;

		fHeight = rb.y + (deltaU * dX) + (deltaV * dZ);
	}

	return fHeight;
}

float Terrain::GetSlant(Vector3 * pOut, float gravityPower, float x, float z)
{
	//�ͷ��� ������ �Ѿ�� 0.0 ���� �����Ѵ�
	if (x < _terrainStartX || x > _terrainEndX ||
		z > _terrainStartZ || z < _terrainEndZ) 
	{
		return false;
	}

	//Terrain �� �»�� 0 �� �������� ���� Terrain �� ������ ��ġ�� ã��
	float pX = x - _terrainStartX;
	float pZ = -(z + _terrainEndZ);

	//�ش� ��ġ�� ��� ���� ���ԵǴ��� �ľ�
	float invCell = 1.0f / _cellScale;
	pX *= invCell;
	pZ *= invCell;

	//�ش� ��ġ�� �� �ε���
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//���� �ױ��� ������ ��´�.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb
	Vector3 lt = _terrainVertices[idxZ * _numVertexX + idxX]._pos;
	Vector3 rt = _terrainVertices[idxZ * _numVertexX + idxX + 1]._pos;
	Vector3 lb = _terrainVertices[(idxZ + 1) * _numVertexX + idxX]._pos;
	Vector3 rb = _terrainVertices[(idxZ + 1) * _numVertexX + idxX + 1]._pos;

	//�ش� �п����� delta ���� ���Ѵ�.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	//�������� �븻 ����
	Vector3 normal;

	//�ش� ���� �»�ܿ� �ִ�?
	if (dX < 1.0f - dZ)
	{
		//�ش��������� ���� ���͸� ���Ѵ�.
		Vector3 edge1 = rt - lt;
		Vector3 edge2 = lb - lt;
		Vec3Cross(&normal, &edge1, &edge2);
	}
	else
	{
		//�ش��������� ���� ���͸� ���Ѵ�.
		Vector3 edge1 = rt - lb;
		Vector3 edge2 = rb - lb;
		Vec3Cross(&normal, &edge1, &edge2);
	}

	//�븻�� ����ȭ
	Vec3Normalize(&normal, &normal);


	//�߷� ����
	Vector3 gravity(0, -gravityPower, 0);

	//������ ����
	Vector3 right;
	Vec3Cross(&right, &normal, &gravity);
	//�������Ϳ��� �������͸� ������ ����� ������ ��� ������ �ȴ�.
	Vec3Cross(pOut, &right, &normal);

	return true;
}

bool Terrain::CreateTerrain(int32 smooth, int32 tileNum)
{
	float tileIntervalX = static_cast<float>(tileNum) / _numCellX;
	float tileIntervalY = static_cast<float>(tileNum) / _numCellZ;

	float terrainStartX = -(_numCellX / 2) * _cellScale;
	float terrainStartY = -(_numCellZ / 2) * _cellScale;

	_terrainVertices = new video::TerrainVertex[_numTotalVertex];

	D3DLOCKED_RECT lockRect;
	video::Texture *heightMap = VIDEO->GetTexture(_heightMapHandle);
	heightMap->_ptr->LockRect(0, &lockRect, 0, 0);

	for (int32 z = 0; z < _numVertexZ; z++) 
	{
		for (int32 x = 0; x < _numVertexX; x++) 
		{
			Vector3 _pos;

			//������ x, z ��ġ ���
			_pos.x = terrainStartX + x * _cellScale;
			_pos.z = terrainStartY + z * _cellScale;

			//���θ����� �����̶�� ( ���� ������ ���� Y ��ġ�� ����� )
			if (x == _numVertexX - 1) 
			{
				int32 index = Index2D(x - 1, z, _numVertexX);
				_pos.y = _terrainVertices[index]._pos.y;
			}
			//���� ������ �����̶�� ( ���� �Ʒ����� ���� Y ��ġ�� ����� )
			else if (z == _numVertexZ - 1) 
			{
				int32 index = Index2D(x, z - 1, _numVertexX);
				_pos.y = _terrainVertices[index]._pos.y;
			}
			else
			{
				//�ش� �ȼ��� �÷� ���� ��´�.
				uint32* pStart = (uint32*)lockRect.pBits;	//(DWORD ������ ����ȯ�� lock �� �̹������� ���� �ּ�
				uint32 dwColor = *(pStart + (z * (lockRect.Pitch / 4) + x));

				//�� �÷� ���� 0 ~ 1 ������ �Ǽ��� ������.
				float inv = 1.0f / 255.0f;
				float r = ((dwColor & 0x00ff0000) >> 16) * inv;
				float g = ((dwColor & 0x0000ff00) >> 8) * inv;
				float b = ((dwColor & 0x000000ff)) * inv;

				//���� �� ��
				float factor = (r + g + b) / 3.0f;

				//���� ��
				_pos.y = factor * _heightScale;
			}

			//���� UV ���

			//Terrain Tile Splating UV
			Vector2 baseUV;
			baseUV.x = x / static_cast<float>(_numVertexX - 1);
			baseUV.y = z / static_cast<float>(_numVertexZ - 1);

			//Terrain Tile UV
			Vector2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//���ؽ� �迭�ε��� ���
			int idx = z * _numVertexX + x;

			_terrainVertices[idx]._pos = _pos;
			_terrainVertices[idx]._normal = Vector3(0, 0, 0);	//�Ʒ����� ���� �븻 ���Ҷ� �������� �ϴ� 0 ���ͷ� �ʱ�ȭ
			_terrainVertices[idx]._baseUV = baseUV;
			_terrainVertices[idx]._tileUV = tileUV;
		}
	}

	//�ؽ����� pixel ���� Unlock
	heightMap->_ptr->UnlockRect(0);

	//�ͷ��� ������ 
	SmoothTerrain(smooth);
	// ���� �ε����� ���Ѵ�.....
	TerrainFace *faces = new TerrainFace[_numTotalFace];

	int32 idx = 0;

	for (uint32 z = 0; z < _numCellZ; z++)
	{
		for (uint32 x = 0; x < _numCellX; x++) 
		{
			//�ش� ���� ���� ���� �ε����� ����
			uint32 lb = z * _numVertexX + x;
			uint32 lt = (z + 1) * _numVertexX + x;
			uint32 rt = ((z + 1)* _numVertexX) + (x + 1);
			uint32 rb = (z * _numVertexX) + (x + 1);

			//���� �ﰢ�� �ϳ�
			faces[idx].i0 = lb;
			faces[idx].i1 = lt;
			faces[idx].i2 = rt;
			idx++;

			//���� �ﰢ�� �ϳ�
			faces[idx].i0 = lb;
			faces[idx].i1 = rt;
			faces[idx].i2 = rb;
			idx++;
		}
	}
	//
	// �븻�̶� Binormal �̶� Tangent �������...
	//
	Vector3* poses = new Vector3[_numTotalVertex];		//������ġ
	Vector3* normals = new Vector3[_numTotalVertex];
	Vector3* tangents = new Vector3[_numTotalVertex];
	Vector3* binormals = new Vector3[_numTotalVertex];
	Vector2* uvs = new Vector2[_numTotalVertex];
	uint32* indices = (uint32*)faces;

	//������ġ �� UV ����
	for (uint32 i = 0; i < this->_numTotalVertex; i++) 
	{
		poses[i] = _terrainVertices[i]._pos;
		uvs[i] = _terrainVertices[i]._baseUV;
	}

	//�븻���
	ComputeNormal(normals, poses, _numTotalVertex, indices, _numTotalFace * 3);

	//ź��Ʈ ���̳븻 ���
	ComputeTangentAngBinormal( tangents, binormals, poses, normals,
		uvs, indices, _numTotalFace, _numTotalVertex);

	//���Ȱ� ����
	for (int32 i = 0; i < this->_numTotalVertex; i++) 
	{
		this->_terrainVertices[i]._normal = normals[i];
		this->_terrainVertices[i]._binormal = binormals[i];
		this->_terrainVertices[i]._tangent = tangents[i];
	}

	video::VertexDecl decl;
	decl.Begin();
	decl.Add(D3DVERTEXELEMENT9{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 });
	decl.Add(D3DVERTEXELEMENT9{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 });
	decl.Add(D3DVERTEXELEMENT9{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 });
	decl.Add(D3DVERTEXELEMENT9{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 });
	decl.Add(D3DVERTEXELEMENT9{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 });
	decl.Add(D3DVERTEXELEMENT9{ 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 });
	decl.End(sizeof(video::TerrainVertex));

	_declHandle = VIDEO->CreateVertexDecl(&decl, "TerrainDecl");

	_pSections = new Terrain::TerrainSection[_numSectionX * _numSectionZ];
	Assert(_pSections);

	for (uint32 z = 0; z < _numSectionZ; ++z)
	{
		for (uint32 x = 0; x < _numSectionX; ++x)
		{
			CreateTerrainSection(x, z, _terrainVertices);
		}
	}

	SAFE_DELETE_ARRAY(poses);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(tangents);
	SAFE_DELETE_ARRAY(binormals);
	SAFE_DELETE_ARRAY(uvs);
	SAFE_DELETE_ARRAY(indices);

	return true;
}

bool Terrain::CreateTerrainSection(int32 x, int32 z, const video::TerrainVertex * pTerrainVertices)
{
	int32 sectionIndex = Index2D(x, z, _numSectionX);

	TerrainSection &refSection = _pSections[sectionIndex];

	refSection._indexX = x;
	refSection._indexZ = z;

	int32 globalStartX = x * _sectionResolution;
	int32 globalStartZ = z * _sectionResolution;

	std::vector<video::TerrainVertex> vertices;
	std::vector<uint16> indices;

	vertices.reserve(((_sectionNumVertexX ) * (_sectionNumVertexZ )) + 1);
	indices.reserve((_sectionNumCellX * (_sectionNumCellZ + 1)) * 3 * 2 + 1);
	
	vertices.clear();
	indices.clear();

	//���ؽ� ���� �ֱ�...
	for (int32 localZ = 0; localZ < _sectionNumVertexZ; localZ++)
	{
		for (int32 localX = 0; localX < _sectionNumVertexX; localX++)
		{
			int32 globalX = globalStartX + localX;
			int32 globalZ = globalStartZ + localZ;

			int32 globalIndex = Index2D(globalX, globalZ, _numVertexX);

			vertices.push_back(pTerrainVertices[globalIndex]);
		}
	}

	refSection._startX = vertices[0]._pos.x;
	refSection._startZ = vertices[0]._pos.z;

	refSection._endX = vertices.back()._pos.x;
	refSection._endZ = vertices.back()._pos.z;

	for (uint32 z = 0; z < _sectionResolution; z++)
	{
		for (uint32 x = 0; x < _sectionResolution ; x++)
		{
			uint32 lb = z * _sectionNumVertexX + x;
			uint32 lt = (z + 1) * _sectionNumVertexX + x;
			uint32 rt = ((z + 1)* _sectionNumVertexX) + (x + 1);
			uint32 rb = (z * _sectionNumVertexX) + (x + 1);

			indices.push_back((uint16)lb);
			indices.push_back((uint16)lt);
			indices.push_back((uint16)rt);

			indices.push_back((uint16)lb);
			indices.push_back((uint16)rt);
			indices.push_back((uint16)rb);
		}
	}

	Memory mem;
	mem._data = &vertices[0];
	mem._size = sizeof(video::TerrainVertex) * _sectionNumVertexX * _sectionNumVertexZ;

	int32 size = sizeof(video::TerrainVertex) * vertices.size();

	refSection._vHandle = VIDEO->CreateVertexBuffer(&mem, _declHandle);
	Assert(refSection._vHandle.IsValid());

	mem._data = &indices[0];
	mem._size = sizeof(uint16) * _sectionResolution * _sectionResolution * 2 * 3;

	//refSection._iHandle = VIDEO->GetIndexBuffer("TerrainSectionIndex");
	//if (!refSection._iHandle.IsValid())
	{
		refSection._iHandle = VIDEO->CreateIndexBuffer(&mem, sizeof(uint16));
	}

	Assert(refSection._iHandle.IsValid());

	refSection._centerX = (refSection._startX + refSection._endX) * 0.5f;
	refSection._centerZ = (refSection._startZ + refSection._endZ) * 0.5f;
	refSection._radius = (refSection._centerX - refSection._startX) * 1.2f;

	return true;
}

void Terrain::SmoothTerrain(int32 passed)
{
	if (passed <= 0)
	{
		return;
	}

	float* smooth = new float[_numTotalVertex];

	while (passed > 0) {

		passed--;

		for (int32 z = 0; z < _numVertexZ; z++) 
		{
			for (int32 x = 0; x < _numVertexX; x++) 
			{
				int32 adjacentSections = 0;		//��� ������ ��հ��� ����?
				float totalSections = 0.0f;		//�ֺ��� ���� ���� ������ �󸶴�?

												//����üũ
				if ((x - 1) > 0) 
				{
					totalSections += _terrainVertices[(z * _numVertexX) + (x - 1)]._pos.y;
					adjacentSections++;

					//���� ���
					if ((z - 1) > 0) 
					{
						totalSections += _terrainVertices[((z - 1) * _numVertexX) + (x - 1)]._pos.y;
						adjacentSections++;
					}
					//���� �ϴ�
					if ((z + 1) < _numVertexZ) 
					{
						totalSections += _terrainVertices[((z + 1) * _numVertexX) + (x - 1)]._pos.y;
						adjacentSections++;
					}
				}

				//������ üũ
				if ((x + 1) < _numVertexX) 
				{
					totalSections += _terrainVertices[(z * _numVertexX) + (x + 1)]._pos.y;
					adjacentSections++;
					//������ ���
					if ((z - 1) > 0) 
					{
						totalSections += _terrainVertices[((z - 1) * _numVertexX) + (x + 1)]._pos.y;
						adjacentSections++;
					}
					//������ �ϴ� 
					if ((z + 1) < _numVertexZ) 
					{
						totalSections += _terrainVertices[((z + 1) * _numVertexX) + (x + 1)]._pos.y;
						adjacentSections++;
					}
				}


				//���
				if ((z - 1) > 0)
				{
					totalSections += _terrainVertices[((z - 1) * _numVertexX) + x]._pos.y;
					adjacentSections++;
				}

				//�ϴ�
				if ((z + 1) < _numVertexZ)
				{
					totalSections += _terrainVertices[((z + 1) * _numVertexX) + x]._pos.y;
					adjacentSections++;
				}
				smooth[(z * _numVertexX) + x] = (
					_terrainVertices[(z * _numVertexX) + x]._pos.y +
					(totalSections / adjacentSections)) * 0.5f;
			}
		}

		//������ ���� y ������ ����
		for (int32 i = 0; i < _numTotalVertex; i++) 
		{
			_terrainVertices[i]._pos.y = smooth[i];
		}
	}

	SAFE_DELETE_ARRAY(smooth);
}