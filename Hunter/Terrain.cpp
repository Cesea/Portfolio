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
	//스케일값 대입
	_heightScale = config._heightScale;
	_cellScale = config._cellScale;

	//높이맵 불러온다
	_heightMapHandle = VIDEO->CreateTexture(config._heightFileName, config._heightFileName);
	video::Texture *heightMap = VIDEO->GetTexture(_heightMapHandle);

	if (nullptr == heightMap)
	{
		return false;
	}

	D3DSURFACE_DESC sd;
	heightMap->_ptr->GetLevelDesc(0, &sd);

	//가로세로 정점 수를 구한다.
	_numVertexX = sd.Width + 1;		
	//가로 정점갯수는 높이 맵에 가로 해상도 + 1 과 같다. ( 이유는 쿼드트리쓰려면 정점갯수가 2의N승 + 1 이여야 하기 때문에 )
	_numVertexZ = sd.Height + 1;		
	//세로 정점잿수는 높이 맵에 세로 해상도 + 1 과 같다. ( 이유는 쿼드트리쓰려면 정점갯수가 2의N승 + 1 이여야 하기 때문에 )
	_numTotalVertex = _numVertexX * _numVertexZ;		//총 정점 갯수

	_numCellX = _numVertexX - 1;
	_numCellZ = _numVertexZ - 1;
	_totalCellNum = _numCellX * _numCellZ;

	//터레인 크기
	_terrainSizeX = _numCellX * _cellScale;
	_terrainSizeZ = _numCellZ * _cellScale;

	//총 삼각형수는
	_numTotalFace = _totalCellNum * 2;

	//Section 설정
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

	//터레인 범위
	_terrainStartX = _terrainVertices[0]._pos.x;
	_terrainStartZ = _terrainVertices[0]._pos.z;
	_terrainEndX = _terrainVertices[_numTotalVertex - 1]._pos.x;
	_terrainEndZ = _terrainVertices[_numTotalVertex - 1]._pos.z;

	//쿼드트리를 만든다.
	_pQuadTree = new QuadTree;
	_pQuadTree->Init(_terrainVertices, _numVertexX, _sectionResolution);

	//터레인 Texture 로딩
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

	VIDEO->DestroyVertexBuffer(_vHandle);
	VIDEO->DestroyIndexBuffer(_iHandle);
	//for (int32 i = 0; i < _numSectionX * _numSectionZ; ++i)
	//{
	//	VIDEO->DestroyVertexBuffer(_pSections[i]._vHandle);
	//	VIDEO->DestroyIndexBuffer(_pSections[i]._iHandle);
	//}
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
	//IntRect drawRange = _pQuadTree->CalculateDrawRange(renderView._pCamera->GetFrustum());

	video::IndexBuffer *pIndexBuffer = VIDEO->GetIndexBuffer(_iHandle);
	uint8 *pData = nullptr;
	pIndexBuffer->_ptr->Lock(0, sizeof(uint32) * _numCellX * _numCellZ * 3 * 2, (void **)&pData, D3DLOCK_DISCARD);
	_numTriangleToDraw = _pQuadTree->GenerateIndex(pData, renderView._pCamera->GetFrustum());
	pIndexBuffer->_ptr->Unlock();

	video::RenderCommand &command = renderView.GetCommand();
	command._drawType = video::RenderCommand::DrawType::eStatic;
	command._primType = video::RenderCommand::PrimType::eTriangleList;
	Assert(_vHandle.IsValid());
	Assert(_iHandle.IsValid());
	command._vHandle = _vHandle;
	command._iHandle = _iHandle;
	command._effectHandle = _effect;
	command._materialHandle = _materialHandle;
	command._numPrim = _numTriangleToDraw;

	//for (int32 i = 0; i < _numSectionX * _numSectionZ; ++i)
	//{
	//	Terrain::TerrainSection &refSection = _pSections[i];
	//	if (renderView._pCamera->GetFrustum().IsSphereInFrustum(Vector3(refSection._centerX, 0.0f, refSection._centerZ), refSection._radius))
	//	{
	//		video::RenderCommand &command = renderView.GetCommand();
	//		command._drawType = video::RenderCommand::DrawType::eStatic;
	//		command._primType = video::RenderCommand::PrimType::eTriangleList;
	//		Assert(refSection._vHandle.IsValid());
	//		Assert(refSection._iHandle.IsValid());
	//		command._vHandle = refSection._vHandle;
	//		command._iHandle = refSection._iHandle;
	//		command._effectHandle = _effect;
	//		command._materialHandle = _materialHandle;
	//	}
	//}
}

//TODO : Implement this
bool Terrain::IsIntersectRay(const Ray &ray, Vector3 *pOutHit)
{
	std::vector<Vector3> hits;

	//최상단 쿼드 트리 부터 Ray Check 들어간다.
	_pQuadTree->GetRayHits(ray, &hits);

	if (hits.size() == 0)
	{
		return false;
	}
	//먼저 처음 위치화 처음 위치에 대한 거리 대입
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
	//터레인 범위을 넘어가면 0.0 값을 리턴한다
	if (x < _terrainStartX || x > _terrainEndX ||
		z < _terrainStartZ || z > _terrainEndZ) 
	{
		return 0.0f;
	}

	//Terrain 의 우하단 0 을 기준으로 월드 Terrain 의 상태적 위치를 찾자
	float pX = x - _terrainStartX;
	float pZ = z - _terrainStartZ;

	//해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f / _cellScale;
	pX *= invCell;
	pZ *= invCell;

	//해당 위치의 셀 인덱스
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//셀의 네군데 정점을 얻는다.
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

	//해당 셸에서의 delta 량을 구한다.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	//해당 점이 좌상단에 있니?
	if (dX < 1.0f - dZ)
	{
		float deltaU = rt.y - lt.y;	//lt 정점에서 rt 정점까지의 y 위치의 변위량
		float deltaV = lb.y - lt.y; //lt 정점에서 lb 정점까지의 y 위치의 변위량

		fHeight = lt.y + (deltaU * dX) + (deltaV * dZ);
	}
	//해당 점이 우하단이 있니?
	else
	{
		float deltaU = lb.y - rb.y;
		float deltaV = rt.y - rb.y;

		//우에서 좌로 하에서 상으로 보간방향이 바뀌었기 때문에
		//delta 량을 역수로 취한다.
		dX = 1.0f - dX;
		dZ = 1.0f - dZ;

		fHeight = rb.y + (deltaU * dX) + (deltaV * dZ);
	}

	return fHeight;
}

float Terrain::GetSlant(Vector3 * pOut, float gravityPower, float x, float z)
{
	//터레인 범위을 넘어가면 0.0 값을 리턴한다
	if (x < _terrainStartX || x > _terrainEndX ||
		z > _terrainStartZ || z < _terrainEndZ) 
	{
		return false;
	}

	//Terrain 의 좌상단 0 을 기준으로 월드 Terrain 의 상태적 위치를 찾자
	float pX = x - _terrainStartX;
	float pZ = -(z + _terrainEndZ);

	//해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f / _cellScale;
	pX *= invCell;
	pZ *= invCell;

	//해당 위치의 셀 인덱스
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//셀의 네군데 정점을 얻는다.
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

	//해당 셸에서의 delta 량을 구한다.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	//폴리곤의 노말 벡터
	Vector3 normal;

	//해당 점이 좌상단에 있니?
	if (dX < 1.0f - dZ)
	{
		//해당폴리곤의 법선 벡터를 구한다.
		Vector3 edge1 = rt - lt;
		Vector3 edge2 = lb - lt;
		Vec3Cross(&normal, &edge1, &edge2);
	}
	else
	{
		//해당폴리곤의 법선 벡터를 구한다.
		Vector3 edge1 = rt - lb;
		Vector3 edge2 = rb - lb;
		Vec3Cross(&normal, &edge1, &edge2);
	}

	//노말은 정규화
	Vec3Normalize(&normal, &normal);


	//중력 방향
	Vector3 gravity(0, -gravityPower, 0);

	//경사면의 우측
	Vector3 right;
	Vec3Cross(&right, &normal, &gravity);
	//우측벡터에서 법선벡터를 외적한 결과가 경사면의 경사 방향이 된다.
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
	heightMap->_ptr->LockRect(0, &lockRect, 0, D3DLOCK_READONLY);

	for (int32 z = 0; z < _numVertexZ; z++) 
	{
		for (int32 x = 0; x < _numVertexX; x++) 
		{
			Vector3 _pos;

			//정점의 x, z 위치 계산
			_pos.x = terrainStartX + x * _cellScale;
			_pos.z = terrainStartY + z * _cellScale;

			//가로마지막 라인이라면 ( 이전 왼쪽의 정점 Y 위치와 맞춘다 )
			if (x == _numVertexX - 1) 
			{
				int32 index = Index2D(x - 1, z, _numVertexX);
				_pos.y = _terrainVertices[index]._pos.y;
			}
			//세로 마지막 라인이라면 ( 이전 아래쪽의 정점 Y 위치와 맞춘다 )
			else if (z == _numVertexZ - 1) 
			{
				int32 index = Index2D(x, z - 1, _numVertexX);
				_pos.y = _terrainVertices[index]._pos.y;
			}
			else
			{
				//해당 픽셀의 컬러 값을 얻는다.
				uint32* pStart = (uint32*)lockRect.pBits;	//(DWORD 형으로 형변환된 lock 된 이미지지의 시작 주소
				uint32 dwColor = *(pStart + (z * (lockRect.Pitch / 4) + x));

				//각 컬러 값을 0 ~ 1 사이의 실수로 나눈다.
				float inv = 1.0f / 255.0f;
				float r = ((dwColor & 0x00ff0000) >> 16) * inv;
				float g = ((dwColor & 0x0000ff00) >> 8) * inv;
				float b = ((dwColor & 0x000000ff)) * inv;

				//높이 맵 값
				float factor = (r + g + b) / 3.0f;

				//높이 값
				_pos.y = factor * _heightScale;
			}

			//정점 UV 계산

			//Terrain Tile Splating UV
			Vector2 baseUV;
			baseUV.x = x / static_cast<float>(_numVertexX - 1);
			baseUV.y = z / static_cast<float>(_numVertexZ - 1);

			//Terrain Tile UV
			Vector2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//버텍스 배열인덱스 계산
			int idx = z * _numVertexX + x;

			_terrainVertices[idx]._pos = _pos;
			_terrainVertices[idx]._normal = Vector3(0, 0, 0);	//아래에서 정점 노말 구할때 더해지니 일단 0 벡터로 초기화
			_terrainVertices[idx]._baseUV = baseUV;
			_terrainVertices[idx]._tileUV = tileUV;
		}
	}

	//텍스쳐의 pixel 정보 Unlock
	heightMap->_ptr->UnlockRect(0);

	//터레인 스무싱 
	SmoothTerrain(smooth);
	// 정점 인덱스를 구한다.....
	TerrainFace *faces = new TerrainFace[_numTotalFace];

	int32 idx = 0;

	for (uint32 z = 0; z < _numCellZ; z++)
	{
		for (uint32 x = 0; x < _numCellX; x++) 
		{
			//해당 셀에 대한 정점 인덱스를 얻자
			uint32 lb = z * _numVertexX + x;
			uint32 lt = (z + 1) * _numVertexX + x;
			uint32 rt = ((z + 1)* _numVertexX) + (x + 1);
			uint32 rb = (z * _numVertexX) + (x + 1);

			//셀의 삼각형 하나
			faces[idx].i0 = lb;
			faces[idx].i1 = lt;
			faces[idx].i2 = rt;
			idx++;

			//셀의 삼각형 하나
			faces[idx].i0 = lb;
			faces[idx].i1 = rt;
			faces[idx].i2 = rb;
			idx++;
		}
	}
	//
	// 노말이랑 Binormal 이랑 Tangent 계산하자...
	//
	Vector3* poses = new Vector3[_numTotalVertex];		//정점위치
	Vector3* normals = new Vector3[_numTotalVertex];
	Vector3* tangents = new Vector3[_numTotalVertex];
	Vector3* binormals = new Vector3[_numTotalVertex];
	Vector2* uvs = new Vector2[_numTotalVertex];
	uint32* indices = (uint32*)faces;

	//정점위치 및 UV 대입
	for (uint32 i = 0; i < this->_numTotalVertex; i++) 
	{
		poses[i] = _terrainVertices[i]._pos;
		uvs[i] = _terrainVertices[i]._baseUV;
	}

	//노말계산
	ComputeNormal(normals, poses, _numTotalVertex, indices, _numTotalFace * 3);

	//탄젠트 바이노말 계산
	ComputeTangentAngBinormal( tangents, binormals, poses, normals,
		uvs, indices, _numTotalFace, _numTotalVertex);

	//계산된거 대입
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

	Memory mem;
	mem._data = &_terrainVertices[0];
	mem._size = sizeof(video::TerrainVertex) * _numVertexX * _numVertexZ;

	_vHandle = VIDEO->CreateVertexBuffer(&mem, _declHandle);
	Assert(_vHandle.IsValid());

	mem._data = nullptr;
	mem._size = sizeof(uint32) * _numCellX * _numCellZ * 2 * 3;

	_iHandle = VIDEO->CreateIndexBuffer(&mem, sizeof(uint32));

	//refSection._iHandle = VIDEO->GetIndexBuffer("TerrainSectionIndex");
	//if (!refSection._iHandle.IsValid())
	//_pSections = new Terrain::TerrainSection[_numSectionX * _numSectionZ];
	//Assert(_pSections);

	//for (uint32 z = 0; z < _numSectionZ; ++z)
	//{
	//	for (uint32 x = 0; x < _numSectionX; ++x)
	//	{
	//		CreateTerrainSection(x, z, _terrainVertices);
	//	}
	//}

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

	//버텍스 정보 넣기...
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

	//refSection._vHandle = VIDEO->CreateVertexBuffer(&mem, _declHandle);
	//Assert(refSection._vHandle.IsValid());

	//mem._data = &indices[0];
	//mem._size = sizeof(uint16) * _sectionResolution * _sectionResolution * 2 * 3;

	////refSection._iHandle = VIDEO->GetIndexBuffer("TerrainSectionIndex");
	////if (!refSection._iHandle.IsValid())
	//{
	//	refSection._iHandle = VIDEO->CreateIndexBuffer(&mem, sizeof(uint16));
	//}

	//Assert(refSection._iHandle.IsValid());

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
				int32 adjacentSections = 0;		//몇개의 정점과 평균값을 내니?
				float totalSections = 0.0f;		//주변의 정점 높이 총합은 얼마니?

												//왼쪽체크
				if ((x - 1) > 0) 
				{
					totalSections += _terrainVertices[(z * _numVertexX) + (x - 1)]._pos.y;
					adjacentSections++;

					//왼쪽 상단
					if ((z - 1) > 0) 
					{
						totalSections += _terrainVertices[((z - 1) * _numVertexX) + (x - 1)]._pos.y;
						adjacentSections++;
					}
					//왼쪽 하단
					if ((z + 1) < _numVertexZ) 
					{
						totalSections += _terrainVertices[((z + 1) * _numVertexX) + (x - 1)]._pos.y;
						adjacentSections++;
					}
				}

				//오른쪽 체크
				if ((x + 1) < _numVertexX) 
				{
					totalSections += _terrainVertices[(z * _numVertexX) + (x + 1)]._pos.y;
					adjacentSections++;
					//오른쪽 상단
					if ((z - 1) > 0) 
					{
						totalSections += _terrainVertices[((z - 1) * _numVertexX) + (x + 1)]._pos.y;
						adjacentSections++;
					}
					//오른쪽 하단 
					if ((z + 1) < _numVertexZ) 
					{
						totalSections += _terrainVertices[((z + 1) * _numVertexX) + (x + 1)]._pos.y;
						adjacentSections++;
					}
				}


				//상단
				if ((z - 1) > 0)
				{
					totalSections += _terrainVertices[((z - 1) * _numVertexX) + x]._pos.y;
					adjacentSections++;
				}

				//하단
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

		//위에서 계산된 y 스무싱 적용
		for (int32 i = 0; i < _numTotalVertex; i++) 
		{
			_terrainVertices[i]._pos.y = smooth[i];
		}
	}

	SAFE_DELETE_ARRAY(smooth);
}
