#include "stdafx.h"
#include "Terrain.h"

#include "QuadTree.h"

Terrain::Terrain()
{
	EventChannel channel;
	channel.Add<GameObjectFactory::ObjectCreatedEvent, Terrain>(*this);

	_visibleChunks.reserve(16);
	_shadowVisibleChunks.reserve(16);

	_visibleTiles.reserve(256);
}

Terrain::~Terrain()
{
}

bool Terrain::Create(const Terrain::TerrainConfig &config, bool32 inEditMode)
{
	_inEditMode = inEditMode;

	_currentConfig = config;

	//가로세로 정점 수를 구한다.
	//가로 세로 정점의 수는 TILE_CHUNK_DIM * tileExtent + 1과 같다..
	_numVertexX = TERRAIN_CHUNK_DIM * _currentConfig._xChunkCount + 1;		
	_numVertexZ = TERRAIN_CHUNK_DIM * _currentConfig._zChunkCount + 1;		
	_numTotalVertex = _numVertexX * _numVertexZ;		//총 정점 갯수

	_numCellX = _numVertexX - 1;
	_numCellZ = _numVertexZ - 1;
	_totalCellNum = _numCellX * _numCellZ;

	//터레인 크기
	_terrainSizeX = _numCellX;
	_terrainSizeZ = _numCellZ;

	_terrainHalfSizeX = _terrainSizeX / 2;
	_terrainHalfSizeZ = _terrainSizeZ / 2;

	//총 삼각형수는
	_numTotalFace = _totalCellNum * 2;

	//Section 설정
	_sectionResolution = TERRAIN_CHUNK_DIM;
		
	_xChunkCount = config._xChunkCount;
	_zChunkCount = config._zChunkCount;

	_sectionNumCellX = _sectionResolution;
	_sectionNumCellZ = _sectionResolution;

	_sectionNumVertexX = _sectionNumCellX + 1;
	_sectionNumVertexZ = _sectionNumCellZ + 1;

	if (!(CreateTerrain(config._textureMult * _xChunkCount)))
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
	_pQuadTree->Init(_terrainVertices, _numVertexX, 1);

	//터레인 Texture 로딩 ////////////////////////////////////
	_effect = VIDEO->GetEffect("TerrainBase.fx");

	LoadTextureFromConfig(_currentConfig);
}

void Terrain::RegisterEvents()
{
	EventChannel channel;
	channel.Add<GameObjectFactory::ObjectCreatedEvent, Terrain>(*this);

}

void Terrain::UnRegisterEvents()
{
}

void Terrain::Handle(const GameObjectFactory::ObjectCreatedEvent & event)
{
	TerrainTilePos tilePos;
	ConvertWorldPostoTilePos(event._worldPosition, &tilePos);

	TerrainChunk &refChunk = _pChunks[Index2D(tilePos._chunkX, tilePos._chunkZ, _xChunkCount)];
	Entity entity = event._entity;
	if (entity.IsValid())
	{
		int32 tileIndex = Index2D(tilePos._tileX, tilePos._tileZ, TERRAIN_TILE_RES);
		refChunk._tiles[tileIndex]._entities.push_back(entity);
		refChunk._numTotalEntity++;

		int32 chunkIndex = Index2D(tilePos._chunkX, tilePos._chunkZ, _xChunkCount);
		for (uint32 i = 0; i < _activeChunkIndices.size(); ++i)
		{
			if (_activeChunkIndices[i] == chunkIndex)
			{
				entity.Activate();
				break;
			}
		}
	}
}


void Terrain::Destroy()
{
	_pCurrentScene = nullptr;

	for (int32 i = 0; i < _xChunkCount * _zChunkCount; ++i)
	{
		VIDEO->DestroyVertexBuffer(_pChunks[i]._vHandle);
		VIDEO->DestroyIndexBuffer(_pChunks[i]._iHandle);
	}

	SAFE_DELETE_ARRAY(_pChunks);

	VIDEO->DestroyVertexDecl(_declHandle);

	VIDEO->DestroyTexture(_tile0Handle);
	VIDEO->DestroyTexture(_tile1Handle);
	VIDEO->DestroyTexture(_tile2Handle);
	VIDEO->DestroyTexture(_tileControl1Handle);
	VIDEO->DestroyTexture(_tileControl2Handle);

	//VIDEO->DestroyMaterial(_materialHandle);

	SAFE_DELETE_ARRAY(_terrainVertices);
	SAFE_DELETE_ARRAY(_chunkIndex);
	SAFE_DELETE(_pQuadTree);
}

void Terrain::SaveTerrain(const std::string & fileName)
{
	//로딩 경로에서 파일명만 제거하고 경로만 받는다.
	std::string fileNameCopy = fileName;
	std::string path;
	std::string name;
	std::string extension;

	SplitFilePathToNamePathExtension(fileNameCopy, name, path, extension);

	DataPackage toSave;

	char control1[MAX_FILE_NAME];
	ZeroMemory(control1, sizeof(control1));
	sprintf(control1, "%s", (path + name).c_str());
	strncat(control1, "_Splat1.png", strlen("_Splat1.png"));
	strncpy(_currentConfig._control1Name, control1, MAX_FILE_NAME);

	char control2[MAX_FILE_NAME];
	ZeroMemory(control2, sizeof(control2));
	sprintf(control2, "%s", (path + name).c_str());
	strncat(control2, "_Splat2.png", strlen("_Splat2.png"));
	strncpy(_currentConfig._control2Name, control2, MAX_FILE_NAME);

	toSave.Create(sizeof(Terrain::TerrainConfig) + (sizeof(video::TerrainVertex) * _numTotalVertex));

	toSave.WriteAs<TerrainConfig>(_currentConfig);

	for (int32 i = 0; i < _numTotalVertex; ++i)
	{
		video::TerrainVertex &vertex = _terrainVertices[i];
		toSave.WriteAs<video::TerrainVertex>(vertex);
	}
	toSave.Save(fileName.c_str());
	
	VIDEO->SaveTexture(control1, _tileControl1Handle);
	VIDEO->SaveTexture(control2, _tileControl2Handle);
}

void Terrain::LoadTerrain(const std::string & fileName, bool editMode)
{
	DataPackage toLoad;

	_inEditMode = editMode;

	TerrainConfig config;

	uint32 fileSize{ };
	if (!toLoad.OpenFile(fileName.c_str(), &fileSize))
	{
		Console::Log("There is no such file");
		return;
	}
	toLoad.ReadAs<TerrainConfig>(&config);

	//기존의 자원들 해제
	for (int32 i = 0; i < _xChunkCount * _zChunkCount; ++i)
	{
		VIDEO->DestroyVertexBuffer(_pChunks[i]._vHandle);
		VIDEO->DestroyIndexBuffer(_pChunks[i]._iHandle);
	}

	SAFE_DELETE_ARRAY(_pChunks);

	if (_tile0Handle.IsValid()) { VIDEO->DestroyTexture(_tile0Handle); }
	if (_tile1Handle.IsValid()) { VIDEO->DestroyTexture(_tile1Handle); }
	if (_tile2Handle.IsValid()) { VIDEO->DestroyTexture(_tile2Handle); }
	if (_tileControl1Handle.IsValid()) { VIDEO->DestroyTexture(_tileControl1Handle); }
	if (_tileControl2Handle.IsValid()) { VIDEO->DestroyTexture(_tileControl2Handle); }

	SAFE_DELETE_ARRAY(_terrainVertices);
	SAFE_DELETE(_pQuadTree);

	//새로운 설정값 세팅
	_currentConfig = config;

	_numVertexX = TERRAIN_CHUNK_DIM * _currentConfig._xChunkCount + 1;		
	_numVertexZ = TERRAIN_CHUNK_DIM * _currentConfig._zChunkCount + 1;		
	_numTotalVertex = _numVertexX * _numVertexZ;		//총 정점 갯수

	_numCellX = _numVertexX - 1;
	_numCellZ = _numVertexZ - 1;
	_totalCellNum = _numCellX * _numCellZ;

	_terrainSizeX = _numCellX;
	_terrainSizeZ = _numCellZ;

	_terrainHalfSizeX = _terrainSizeX / 2;
	_terrainHalfSizeZ = _terrainSizeZ / 2;

	_numTotalFace = _totalCellNum * 2;

	_sectionResolution = TERRAIN_CHUNK_DIM;
		
	_xChunkCount = config._xChunkCount;
	_zChunkCount = config._zChunkCount;

	_sectionNumCellX = _sectionResolution;
	_sectionNumCellZ = _sectionResolution;

	_sectionNumVertexX = _sectionNumCellX + 1;
	_sectionNumVertexZ = _sectionNumCellZ + 1;

	//여기서 저장되어있는 정점정보들을 가져온다
	_terrainVertices = new video::TerrainVertex[_numTotalVertex];
	for (int32 i = 0; i < _numTotalVertex; ++i)
	{
		toLoad.ReadAs<video::TerrainVertex>(&_terrainVertices[i]);
	}
	toLoad.Free();

	_pChunks = new TerrainChunk[_xChunkCount * _zChunkCount];

	for (int32 z = 0; z < _zChunkCount; ++z)
	{
		for (int32 x = 0; x < _xChunkCount; ++x)
		{
			CreateTerrainChunk(x, z, _terrainVertices);
		}
	}
	//터레인 범위
	_terrainStartX = _terrainVertices[0]._pos.x;
	_terrainStartZ = _terrainVertices[0]._pos.z;
	_terrainEndX = _terrainVertices[_numTotalVertex - 1]._pos.x;
	_terrainEndZ = _terrainVertices[_numTotalVertex - 1]._pos.z;

	//쿼드트리를 만든다.
	_pQuadTree = new QuadTree;
	_pQuadTree->Init(_terrainVertices, _numVertexX, _sectionResolution);

	LoadTextureFromConfig(_currentConfig);

}

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
		z > _terrainStartZ || z < _terrainEndZ) 
	{
		return 0.0f;
	}

	float pX = x - _terrainStartX;
	float pZ = -(z + _terrainEndZ);

	//해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f;
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
	float invCell = 1.0f;
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

void Terrain::Render(const Camera &camera, const DirectionalLight &mainLight, const Camera &lightCameera)
{
	//월드 행렬셋팅
	video::Effect *pEffect = VIDEO->GetEffect(_effect);
	Matrix matInd;
	MatrixIdentity(&matInd);
	pEffect->SetMatrix("matWorld", matInd);

	//뷰 행렬셋팅
	pEffect->SetMatrix("matViewProjection", camera.GetViewProjectionMatrix());
	pEffect->SetVector("vEyePosition", Vector4(camera.GetEntity().GetComponent<TransformComponent>()._position, 1.0f));
	pEffect->SetMatrix("baseDirectionalLight", mainLight.GetLightMatrix());

	HRESULT re = pEffect->SetTechnique("ReciveShadow");

	//Texture 셋팅
	pEffect->SetTexture("Terrain0_Tex", *VIDEO->GetTexture(_tile0Handle));
	pEffect->SetTexture("Terrain1_Tex", *VIDEO->GetTexture(_tile1Handle));
	pEffect->SetTexture("Terrain2_Tex", *VIDEO->GetTexture(_tile2Handle));
	pEffect->SetTexture("TerrainControl_Tex1", *VIDEO->GetTexture(_tileControl1Handle));
	pEffect->SetTexture("TerrainControl_Tex2", *VIDEO->GetTexture(_tileControl2Handle));

	//카메라에 보이는 청크, 타일들만 _visible에 넣는다
	_visibleChunks.clear();
	_visibleTiles.clear();
	for (int32 i = 0; i < _xChunkCount * _zChunkCount; ++i)
	{
		TerrainChunk &refChunk = _pChunks[i];
		if (camera.GetFrustum().IsSphereInFrustum(Vector3(refChunk._centerX, 0.0f, refChunk._centerZ),
			refChunk._radius))
		{
			_visibleChunks.push_back(&refChunk);
			//카메라에 보이는 청크가 현재 활성화 되어있는 청크라면.....
			if (VectorHasItem(_activeChunkIndices, i))
			{
				for (int32 t = 0; t < TERRAIN_TILE_RES * TERRAIN_TILE_RES; ++t)
				{
					TerrainTile &refTile = refChunk._tiles[t];
					if (camera.GetFrustum().IsSphereInFrustum(
						Vector3(refTile._centerX, 0.0f, refTile._centerZ),
						refTile._radius))
					{
						_visibleTiles.push_back(&refTile);
					}
				}
			}
		}

	}
	video::VertexBuffer *vBuffer = nullptr;
	video::IndexBuffer *iBuffer = nullptr;
	video::VertexDecl *decl = nullptr;

	if (_inEditMode)
	{
		for (uint32 i = 0; i < _visibleChunks.size(); ++i)
		{
			TerrainChunk &refChunk = *_visibleChunks[i];
			vBuffer = VIDEO->GetVertexBuffer(refChunk._vHandle);
			iBuffer = VIDEO->GetIndexBuffer(refChunk._iHandle);
			decl = VIDEO->GetVertexDecl(vBuffer->_decl);

			//버텍스 버퍼에 정점 정보를 넣는다
			video::TerrainVertex *pVertexData = nullptr;
			video::TerrainVertex *pToCopy = _terrainVertices;
			vBuffer->_ptr->Lock(0, 0, (void **)&pVertexData, D3DLOCK_DISCARD);
			for (int32 j = 0; j < TERRAIN_CHUNK_DIM + 1; ++j)
			{
				int32 vertexXOffset = (refChunk._chunkX * TERRAIN_CHUNK_DIM);
				int32 vertexZOffset = (refChunk._chunkZ * TERRAIN_CHUNK_DIM) + j;

				int32 offset = j * (TERRAIN_CHUNK_DIM + 1);
				int32 toCopyOffset = Index2D(vertexXOffset, vertexZOffset, _numVertexX);

				memcpy(pVertexData + offset, pToCopy + toCopyOffset, 
					sizeof(video::TerrainVertex) * (TERRAIN_CHUNK_DIM + 1));
			}

			vBuffer->_ptr->Unlock();

			//인덱스 버퍼에 정점 정보를 넣는다
			void *pIndexData = nullptr;
			iBuffer->_ptr->Lock(0, 0, (void **)&pIndexData, D3DLOCK_DISCARD);
			memcpy(pIndexData, _chunkIndex, sizeof(TerrainFace) * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM * 2);
			iBuffer->_ptr->Unlock();

			gpDevice->SetStreamSource(0, vBuffer->_ptr, 0, sizeof(video::TerrainVertex));
			gpDevice->SetVertexDeclaration(decl->_ptr);
			gpDevice->SetIndices(iBuffer->_ptr);

			uint32 numPass = pEffect->BeginEffect();

			for (uint32 p = 0; p < numPass; p++)
			{
				pEffect->BeginPass(p);

				HRESULT re = gpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
					_sectionNumVertexX * _sectionNumVertexZ, 0, _sectionNumCellX * _sectionNumCellX * 2);
				pEffect->EndPass();
			}
			pEffect->EndEffect();
		}
	}
	else
	{
		for (uint32 i = 0; i < _visibleChunks.size(); ++i)
		{
			TerrainChunk &refChunk = *_visibleChunks[i];
			vBuffer = VIDEO->GetVertexBuffer(refChunk._vHandle);
			iBuffer = VIDEO->GetIndexBuffer(refChunk._iHandle);
			decl = VIDEO->GetVertexDecl(vBuffer->_decl);

			gpDevice->SetStreamSource(0, vBuffer->_ptr, 0, sizeof(video::TerrainVertex));
			gpDevice->SetVertexDeclaration(decl->_ptr);
			gpDevice->SetIndices(iBuffer->_ptr);

			uint32 numPass = pEffect->BeginEffect();

			for (uint32 p = 0; p < numPass; p++)
			{
				pEffect->BeginPass(p);

				gpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
					_sectionNumVertexX * _sectionNumVertexZ, 0, _sectionNumCellX * _sectionNumCellX * 2);

				pEffect->EndPass();
			}
			pEffect->EndEffect();
		}
	}
}

void Terrain::RenderShadow(const Camera & camera)
{
	video::Effect *pEffect = VIDEO->GetEffect(_effect);
	Matrix matInd;
	MatrixIdentity( &matInd );
	pEffect->SetMatrix( "matWorld", matInd );

	//뷰 행렬셋팅
	pEffect->SetMatrix( "matViewProjection", camera.GetViewProjectionMatrix() );

	pEffect->SetTechnique("CreateShadow");

	_shadowVisibleChunks.clear();
	_shadowVisibleTiles.clear();
	for (int32 i = 0; i < _xChunkCount * _zChunkCount; ++i)
	{
		TerrainChunk &refChunk = _pChunks[i];
		if (camera.GetFrustum().IsSphereInFrustum(Vector3(refChunk._centerX, 0.0f, refChunk._centerZ),
			refChunk._radius))
		{
			_shadowVisibleChunks.push_back(&refChunk);
			//카메라에 보이는 청크가 현재 활성화 되어있는 청크라면.....
			if (VectorHasItem(_activeChunkIndices, i))
			{
				for (int32 t = 0; t < TERRAIN_TILE_RES * TERRAIN_TILE_RES; ++t)
				{
					TerrainTile &refTile = refChunk._tiles[t];
					if (camera.GetFrustum().IsSphereInFrustum(
						Vector3(refTile._centerX, 0.0f, refTile._centerZ),
						refTile._radius))
					{
						_shadowVisibleTiles.push_back(&refTile);
					}
				}
			}
		}

	}

	video::VertexBuffer *vBuffer = nullptr;
	video::IndexBuffer *iBuffer = nullptr;
	video::VertexDecl *decl = nullptr;

	if (_inEditMode)
	{
		for (uint32 i = 0; i < _shadowVisibleChunks.size(); ++i)
		{
			TerrainChunk &refChunk = *_shadowVisibleChunks[i];
			vBuffer = VIDEO->GetVertexBuffer(refChunk._vHandle);
			iBuffer = VIDEO->GetIndexBuffer(refChunk._iHandle);
			decl = VIDEO->GetVertexDecl(vBuffer->_decl);

			//버텍스 버퍼에 정점 정보를 넣는다
			video::TerrainVertex *pVertexData = nullptr;
			video::TerrainVertex *pToCopy = _terrainVertices;
			vBuffer->_ptr->Lock(0, 0, (void **)&pVertexData, D3DLOCK_DISCARD);
			for (int32 j = 0; j < TERRAIN_CHUNK_DIM + 1; ++j)
			{
				int32 vertexXOffset = (refChunk._chunkX * TERRAIN_CHUNK_DIM);
				int32 vertexZOffset = (refChunk._chunkZ * TERRAIN_CHUNK_DIM) + j;

				int32 offset = j * (TERRAIN_CHUNK_DIM + 1);
				int32 toCopyOffset = Index2D(vertexXOffset, vertexZOffset, _numVertexX);

				memcpy(pVertexData + offset, pToCopy + toCopyOffset, 
					sizeof(video::TerrainVertex) * (TERRAIN_CHUNK_DIM + 1));
			}

			vBuffer->_ptr->Unlock();

			//인덱스 버퍼에 정점 정보를 넣는다
			void *pIndexData = nullptr;
			iBuffer->_ptr->Lock(0, 0, (void **)&pIndexData, D3DLOCK_DISCARD);
			memcpy(pIndexData, _chunkIndex, sizeof(TerrainFace) * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM * 2);
			iBuffer->_ptr->Unlock();

			gpDevice->SetStreamSource(0, vBuffer->_ptr, 0, sizeof(video::TerrainVertex));
			gpDevice->SetVertexDeclaration(decl->_ptr);
			gpDevice->SetIndices(iBuffer->_ptr);

			uint32 numPass = pEffect->BeginEffect();

			for (uint32 p = 0; p < numPass; p++)
			{
				pEffect->BeginPass(p);

				HRESULT re = gpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
					_sectionNumVertexX * _sectionNumVertexZ, 0, _sectionNumCellX * _sectionNumCellX * 2);

				pEffect->EndPass();
			}
			pEffect->EndEffect();
		}
	}
	else
	{
		for (uint32 i = 0; i < _shadowVisibleChunks.size(); ++i)
		{
			TerrainChunk &refChunk = *_shadowVisibleChunks[i];
			vBuffer = VIDEO->GetVertexBuffer(refChunk._vHandle);
			iBuffer = VIDEO->GetIndexBuffer(refChunk._iHandle);
			decl = VIDEO->GetVertexDecl(vBuffer->_decl);

			gpDevice->SetStreamSource(0, vBuffer->_ptr, 0, sizeof(video::TerrainVertex));
			gpDevice->SetVertexDeclaration(decl->_ptr);
			gpDevice->SetIndices(iBuffer->_ptr);

			uint32 numPass = pEffect->BeginEffect();

			for (uint32 p = 0; p < numPass; p++)
			{
				pEffect->BeginPass(p);

				gpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
					_sectionNumVertexX * _sectionNumVertexZ, 0, _sectionNumCellX * _sectionNumCellX * 2);

				pEffect->EndPass();
			}
			pEffect->EndEffect();
		}
	}
}


bool Terrain::IsWorldPositionInTerrain(const Vector3 & worldPosition)
{
	if (worldPosition.x < -_terrainHalfSizeX || worldPosition.x > _terrainHalfSizeX ||
		worldPosition.z < -_terrainHalfSizeZ || worldPosition.z > _terrainHalfSizeZ)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Terrain::IsTileHasCollisionObject(const Vector3 worldPosition)
{
	TerrainTilePos pos;
	this->ConvertWorldPostoTilePos(worldPosition, &pos);

	return IsTileHasCollisionObject(pos);

}

bool Terrain::IsTileHasCollisionObject(const TerrainTilePos & tilePosition)
{
	std::vector<Entity> &refEntities = GetTileAt(tilePosition)->_entities;
	for (auto entity : refEntities)
	{
		if (entity.HasComponent<CollisionComponent>())
		{
			return true;
		}
	}
	return false;
}

bool Terrain::IsTileHasCollisionObject(const TerrainTilePos & tilePosition, Entity & testObject)
{
	std::vector<Entity> &refEntities = GetTileAt(tilePosition)->_entities;
	for (auto entity : refEntities)
	{
		if (entity.HasComponent<CollisionComponent>() && 
			entity != testObject)
		{
			return true;
		}
	}
	return false;
}

bool Terrain::IsObjectExistInRange(const Vector3 &start, const Vector3 &end, Entity &testObject)
{
	Vector3 realStart;
	Vector3 realEnd;

	if (start.x < end.x)
	{
		realStart.x = start.x;
		realEnd.x = end.x;
	}
	else
	{
		realStart.x = end.x;
		realEnd.x = start.x;
	}

	if (start.z > end.z)
	{
		realStart.z = start.z;
		realEnd.z = end.z;
	}
	else
	{
		realStart.z = end.z;
		realEnd.z = start.z;
	}

	TerrainTilePos startPos;
	TerrainTilePos endPos;

	this->ConvertWorldPostoTilePos(realStart, &startPos);
	this->ConvertWorldPostoTilePos(realEnd, &endPos);

	if (startPos == endPos)
	{
		return IsTileHasCollisionObject(startPos);
	}
	else
	{
		int32 xChunkDiff = endPos._chunkX - startPos._chunkX;
		int32 xTileDiff{};
		if (xChunkDiff > 0)
		{
			xTileDiff = endPos._tileX - startPos._tileX + (TERRAIN_TILE_RES * xChunkDiff);
		}
		else
		{
			xTileDiff = endPos._tileX - startPos._tileX;
		}

		int32 zChunkDiff = endPos._chunkZ - startPos._chunkZ;
		int32 zTileDiff{};
		if (zChunkDiff > 0)
		{
			zTileDiff = endPos._tileZ - startPos._tileZ + (TERRAIN_TILE_RES * zChunkDiff);
		}
		else
		{
			zTileDiff = endPos._tileZ - startPos._tileZ;
		}

		for (int32 z = 0; z < zTileDiff; ++z)
		{
			for (int32 x = 0; x < xTileDiff; ++x)
			{
				TerrainTilePos testPos = startPos;
				testPos._tileX += x;
				testPos._tileZ += z;

				this->ReTilelizeTilePosition(testPos);

				if (IsTileHasCollisionObject(testPos, testObject))
				{
					return true;
				}

			}
		}
	}
	return false;
}

void Terrain::RemoveEntityInTile(Entity entity, const TerrainTilePos & tilePos)
{
	Terrain::TerrainChunk &refChunk = _pChunks[Index2D(tilePos._chunkX, tilePos._chunkZ, _xChunkCount)];
	Terrain::TerrainTile &refTile = refChunk._tiles[Index2D(tilePos._tileX, tilePos._tileZ, TERRAIN_TILE_RES)];

	for (auto iter = refTile._entities.begin(); iter != refTile._entities.end();)
	{
		if (iter->GetID() == entity.GetID())
		{
			refTile._entities.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
}

void Terrain::ReTilelizeTilePosition(TerrainTilePos & tilePos)
{
	if (tilePos._tileX < 0)
	{
		tilePos._chunkX -= 1;
		//Clamp(tilePos._chunkX, 0, _xChunkCount - 1);
		tilePos._tileX += TERRAIN_TILE_RES;
	}
	else if (tilePos._tileX > TERRAIN_TILE_RES - 1)
	{
		tilePos._chunkX += 1;
		//Clamp(tilePos._chunkX, 0, _xChunkCount - 1);
		tilePos._tileX -= TERRAIN_TILE_RES;
	}

	if (tilePos._tileZ < 0)
	{
		tilePos._chunkZ -= 1;
		//Clamp(tilePos._chunkZ, 0, _zChunkCount - 1);
		tilePos._tileZ += TERRAIN_TILE_RES;
	}
	else if (tilePos._tileZ > TERRAIN_TILE_RES - 1)
	{
		tilePos._chunkZ += 1;
		//Clamp(tilePos._chunkZ, 0, _zChunkCount - 1);
		tilePos._tileZ -= TERRAIN_TILE_RES;
	}
}

bool Terrain::IsTerrainTilePosValid(const TerrainTilePos & tilePos)
{
	if (tilePos._chunkX < 0 || tilePos._chunkZ < 0)
	{
		return false;
	}
	if (tilePos._chunkX > _xChunkCount - 1 || tilePos._chunkZ > _zChunkCount - 1)
	{
		return false;
	}

	return true;
}


bool Terrain::CreateTerrain(int32 tileNum)
{
	float tileIntervalX = static_cast<float>(tileNum) / _numCellX;
	float tileIntervalY = static_cast<float>(tileNum) / _numCellZ;

	float terrainStartX = (float)-(_numCellX / 2);
	float terrainStartZ = (float)(_numCellZ / 2);

	_terrainVertices = new video::TerrainVertex[_numTotalVertex];

	for (int32 z = 0; z < _numVertexZ; z++) 
	{
		for (int32 x = 0; x < _numVertexX; x++) 
		{
			Vector3 _pos;

			//정점의 x, z 위치 계산
			_pos.x = terrainStartX + x;
			_pos.z = terrainStartZ - z;
			_pos.y = 2.0f;

			Vector2 baseUV;
			baseUV.x = x / static_cast<float>(_numVertexX - 1);
			baseUV.y = z / static_cast<float>(_numVertexZ - 1);

			//Terrain Tile UV
			Vector2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//버텍스 배열인덱스 계산
			int32 idx = z * _numVertexX + x;

			_terrainVertices[idx]._pos = _pos;
			_terrainVertices[idx]._normal = Vector3(0, 0, 0);	//아래에서 정점 노말 구할때 더해지니 일단 0 벡터로 초기화
			_terrainVertices[idx]._baseUV = baseUV;
			_terrainVertices[idx]._tileUV = tileUV;
		}
	}
	// 정점 인덱스를 구한다.....
	TerrainFace *terrainFaces = new TerrainFace[_numTotalFace];

	int32 idx = 0;
	for (uint32 z = 0; z < _numCellZ; z++)
	{
		for (uint32 x = 0; x < _numCellX; x++) 
		{
			//해당 셀에 대한 정점 인덱스를 얻자

			uint32 lt = z * _numVertexX + x;
			uint32 rt = z * _numVertexX + x + 1;
			uint32 lb = ((z + 1)* _numVertexX) + x;
			uint32 rb = ((z + 1) * _numVertexX) + (x + 1);

			//셀의 삼각형 하나
			terrainFaces[idx].i0 = lt;
			terrainFaces[idx].i1 = rt;
			terrainFaces[idx].i2 = lb;
			idx++;

			//셀의 삼각형 하나
			terrainFaces[idx].i0 = lb;
			terrainFaces[idx].i1 = rt;
			terrainFaces[idx].i2 = rb;
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
	uint32* indices = (uint32*)terrainFaces;

	//정점위치 및 UV 대입
	for (uint32 i = 0; i < this->_numTotalVertex; i++) 
	{
		poses[i] = _terrainVertices[i]._pos;
		uvs[i] = _terrainVertices[i]._baseUV;
	}

	//노말계산
	ComputeNormal(normals, poses, _numTotalVertex, indices, _numTotalFace * 3);

	//탄젠트 바이노말 계산
	ComputeTangentAndBinormal( tangents, binormals, poses, normals,
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

	_pChunks = new TerrainChunk[_xChunkCount * _zChunkCount];

	_chunkIndex = new TerrainFace[_sectionResolution * _sectionResolution * 2];

	int32 counter = 0;
	for (uint32 z = 0; z < _sectionResolution; z++)
	{
		for (uint32 x = 0; x < _sectionResolution; x++)
		{
			uint32 lt = z * _sectionNumVertexX + x;
			uint32 rt = z * _sectionNumVertexX + x + 1;
			uint32 lb = ((z + 1) * _sectionNumVertexX) + x;
			uint32 rb = ((z + 1) * _sectionNumVertexX) + (x + 1);

			_chunkIndex[counter].i0 = lt;
			_chunkIndex[counter].i1 = rt;
			_chunkIndex[counter++].i2 = lb;

			_chunkIndex[counter].i0 = lb;
			_chunkIndex[counter].i1 = rt;
			_chunkIndex[counter++].i2 = rb;
		}
	}

	for (uint32 z = 0; z < _zChunkCount; ++z)
	{
		for (uint32 x = 0; x < _xChunkCount; ++x)
		{
			CreateTerrainChunk(x, z, _terrainVertices);
		}
	}

	SAFE_DELETE_ARRAY(poses);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(tangents);
	SAFE_DELETE_ARRAY(binormals);

	//에딧모드가 아닐때 index정보는 필요 없다
	SAFE_DELETE_ARRAY(indices);

	return true;
}

void Terrain::RebuildTerrain(const Terrain::TerrainConfig &config)
{
	for (int32 i = 0; i < _xChunkCount * _zChunkCount; ++i)
	{
		VIDEO->DestroyVertexBuffer(_pChunks[i]._vHandle);
		VIDEO->DestroyIndexBuffer(_pChunks[i]._iHandle);
	}

	SAFE_DELETE_ARRAY(_pChunks);

	VIDEO->DestroyVertexDecl(_declHandle);

	if (_tile0Handle.IsValid()) { VIDEO->DestroyTexture(_tile0Handle); }
	if (_tile1Handle.IsValid()) { VIDEO->DestroyTexture(_tile1Handle); }
	if (_tile2Handle.IsValid()) { VIDEO->DestroyTexture(_tile2Handle); }
	if (_tileControl1Handle.IsValid()) { VIDEO->DestroyTexture(_tileControl1Handle); }
	if (_tileControl2Handle.IsValid()) { VIDEO->DestroyTexture(_tileControl2Handle); }

	SAFE_DELETE_ARRAY(_terrainVertices);
	SAFE_DELETE_ARRAY(_chunkIndex);
	SAFE_DELETE(_pQuadTree);

	 _activeChunkIndices.clear();
	 _visibleChunks.clear();
	 _visibleTiles.clear();

	 _shadowVisibleChunks.clear();
	 _shadowVisibleTiles.clear();

	//Rebuild모드는 항상 에디터에서 불린다.
	Create(config, true);
}

void Terrain::ReCreateQuadTree()
{
	SAFE_DELETE(_pQuadTree);
	_pQuadTree = new QuadTree;
	_pQuadTree->Init(_terrainVertices, _numVertexX, TERRAIN_CHUNK_DIM);
}


bool Terrain::CreateTerrainChunk(int32 x, int32 z, const video::TerrainVertex * pTerrainVertices)
{
	int32 sectionIndex = Index2D(x, z, _xChunkCount);

	TerrainChunk &refChunk = _pChunks[sectionIndex];

	refChunk._pVertices = pTerrainVertices;

	refChunk._chunkX = x;
	refChunk._chunkZ = z;

	int32 globalStartX = x * _sectionResolution;
	int32 globalStartZ = z * _sectionResolution;

	std::vector<video::TerrainVertex> vertices;

	vertices.reserve(((_sectionNumVertexX ) * (_sectionNumVertexZ )) + 1);
	
	vertices.clear();

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

	refChunk._startX = vertices[0]._pos.x;
	refChunk._startZ = vertices[0]._pos.z;

	refChunk._endX = vertices.back()._pos.x;
	refChunk._endZ = vertices.back()._pos.z;

	if (_inEditMode)
	{
		Memory mem;

		mem._data = nullptr;
		mem._size = sizeof(video::TerrainVertex) * _sectionNumVertexX * _sectionNumVertexZ;

		refChunk._vHandle = VIDEO->CreateVertexBuffer(&mem, _declHandle);
		Assert(refChunk._vHandle.IsValid());

		mem._data = nullptr;
		mem._size = sizeof(uint32) * _sectionResolution * _sectionResolution * 2 * 3;

		if (!refChunk._iHandle.IsValid())
		{
			refChunk._iHandle = VIDEO->CreateIndexBuffer(&mem, sizeof(uint32));
		}

		Assert(refChunk._iHandle.IsValid());
	}
	else
	{
		Memory mem;

		mem._data = &vertices[0];
		mem._size = sizeof(video::TerrainVertex) * _sectionNumVertexX * _sectionNumVertexZ;

		refChunk._vHandle = VIDEO->CreateVertexBuffer(&mem, _declHandle);
		Assert(refChunk._vHandle.IsValid());

		mem._data = &_chunkIndex[0];
		mem._size = sizeof(uint32) * _sectionResolution * _sectionResolution * 2 * 3;

		if (!refChunk._iHandle.IsValid())
		{
			refChunk._iHandle = VIDEO->CreateIndexBuffer(&mem, sizeof(uint32));
		}
		Assert(refChunk._iHandle.IsValid());
	}

	refChunk._centerX = (refChunk._startX + refChunk._endX) * 0.5f;
	refChunk._centerZ = (refChunk._startZ + refChunk._endZ) * 0.5f;
	refChunk._radius = (refChunk._centerX - refChunk._startX) * 1.35f;

	float tileGap = (float)TERRAIN_CHUNK_DIM / (float)TERRAIN_TILE_RES;

	for (int32 tz = 0; tz < TERRAIN_TILE_RES; ++tz)
	{
		for (int32 tx = 0; tx < TERRAIN_TILE_RES; ++tx)
		{
			Terrain::TerrainTile &refTile = refChunk._tiles[Index2D(tx, tz, TERRAIN_TILE_RES)];

			refTile._chunkX = x;
			refTile._chunkZ = z;

			refTile._tileX = tx;
			refTile._tileZ = tz;

			refTile._startX = refChunk._startX + tileGap * (float)tx;
			refTile._startZ = refChunk._startZ - (tileGap * (float)tz);

			refTile._endX = refTile._startX + tileGap;
			refTile._endZ = refTile._startZ - tileGap;

			refTile._centerX = (refTile._endX + refTile._startX) * 0.5f;
			refTile._centerZ = (refTile._endZ + refTile._startZ) * 0.5f;

			refTile._radius = (refTile._centerX - refTile._startX) * 1.35f;
		}
	}

	return true;
}

void Terrain::AddHeightOnCursorPos(const Vector2 &cursorPos, float innerRadius, float outterRadius, float intensity)
{
	Ray ray;
	_pCurrentScene->_camera.ComputeRay(cursorPos, &ray);

	int32 radius = (int32)outterRadius;

	Vector3 worldPos;
	if (IsIntersectRay(ray, &worldPos))
	{
		TerrainVertexPos vertexPos;
		ConvertWorldPostoVertexPos(worldPos, &vertexPos);

		int32 centerX = vertexPos._chunkX * TERRAIN_CHUNK_DIM + vertexPos._tileX;
		int32 centerZ = vertexPos._chunkZ * TERRAIN_CHUNK_DIM + vertexPos._tileZ;

		int32 minX, minZ, maxX, maxZ;
		minX = centerX - radius;
		maxX = centerX + radius;
		minZ = centerZ - radius;
		maxZ = centerZ + radius;

		ClampInt(minX, 0, _numVertexX - 1);
		ClampInt(maxX, 0, _numVertexX - 1);
		ClampInt(minZ, 0, _numVertexZ - 1);
		ClampInt(maxZ, 0, _numVertexZ - 1);

		AddHeightGausian(minX, maxX, minZ, maxZ, intensity);
		RebuildSection(minX, maxX, minZ, maxZ);
	}
}

void Terrain::SmoothOnCursorPos(const Vector2 & cursorPos, float brushRadius)
{
	Ray ray;
	_pCurrentScene->_camera.ComputeRay(cursorPos, &ray);

	int32 radius = (int32)brushRadius;

	Vector3 worldPos;

	if (IsIntersectRay(ray, &worldPos))
	{
		TerrainVertexPos vertexPos;
		ConvertWorldPostoVertexPos(worldPos, &vertexPos);

		int32 centerX = vertexPos._chunkX * TERRAIN_CHUNK_DIM + vertexPos._tileX;
		int32 centerZ = vertexPos._chunkZ * TERRAIN_CHUNK_DIM + vertexPos._tileZ;

		int32 minX, maxX, minZ, maxZ;
		minX = centerX - radius - 1;
		maxX = centerX + radius + 1;
		minZ = centerZ - radius - 1;
		maxZ = centerZ + radius + 1;

		ClampInt(minX, 0, _numVertexX);
		ClampInt(maxX, 0, _numVertexX);
		ClampInt(minZ, 0, _numVertexZ);
		ClampInt(maxZ, 0, _numVertexZ);

		//SmoothSectionGausian(minX, maxX, minZ, maxZ);
		SmoothSection(minX, maxX, minZ, maxZ);

		RebuildSection(minX, maxX, minZ, maxZ);
	}
}

void Terrain::SmoothTerrain()
{
	float* smooth = new float[_numTotalVertex];

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
	SAFE_DELETE_ARRAY(smooth);
}

TerrainChunkPos Terrain::ConvertWorldPosToChunkPos(const Vector3 & worldPos)
{
	TerrainChunkPos result;

	float terrainPosX = worldPos.x + (float)_terrainHalfSizeX;
	float terrainPosZ = -worldPos.z + (float)_terrainHalfSizeZ;

	result._x = (int32)(terrainPosX / TERRAIN_CHUNK_DIM);
	result._z = (int32)(terrainPosZ / TERRAIN_CHUNK_DIM);

	result._relX = terrainPosX - (float)result._x;
	result._relZ = terrainPosZ - (float)result._z;

	return result;
}

void Terrain::ConvertWorldPostoTilePos(const Vector3 & worldPos, TerrainTilePos *pOutTilePos)
{
	float terrainPosX = worldPos.x + (float)_terrainHalfSizeX;
	float terrainPosZ = -worldPos.z + (float)_terrainHalfSizeZ;

	pOutTilePos->_chunkX = (int32)(terrainPosX / TERRAIN_CHUNK_DIM);
	pOutTilePos->_chunkZ = (int32)(terrainPosZ / TERRAIN_CHUNK_DIM);

	int32 chunkStartX = pOutTilePos->_chunkX * TERRAIN_CHUNK_DIM;
	int32 chunkStartZ = pOutTilePos->_chunkZ * TERRAIN_CHUNK_DIM;

	pOutTilePos->_tileX = ((int32)terrainPosX - (int32)chunkStartX) / TERRAIN_TILE_DIM;
	pOutTilePos->_tileZ = ((int32)terrainPosZ - (int32)chunkStartZ) / TERRAIN_TILE_DIM;

	pOutTilePos->_relX = (float)(terrainPosX - (float)(chunkStartX + (pOutTilePos->_tileX * TERRAIN_TILE_DIM) ));
	pOutTilePos->_relZ = (float)(terrainPosZ - (float)(chunkStartZ + (pOutTilePos->_tileZ * TERRAIN_TILE_DIM) ));
}

void Terrain::ConvertWorldPostoVertexPos(const Vector3 & worldPos, TerrainVertexPos * pOutVertexPos)
{
	float terrainPosX = worldPos.x + (float)_terrainHalfSizeX;
	float terrainPosZ = -worldPos.z + (float)_terrainHalfSizeZ;

	pOutVertexPos->_chunkX = (int32)(terrainPosX / TERRAIN_CHUNK_DIM);
	pOutVertexPos->_chunkZ = (int32)(terrainPosZ / TERRAIN_CHUNK_DIM);

	int32 chunkStartX = pOutVertexPos->_chunkX * TERRAIN_CHUNK_DIM;
	int32 chunkStartZ = pOutVertexPos->_chunkZ * TERRAIN_CHUNK_DIM;

	pOutVertexPos->_tileX = ((int32)terrainPosX - (int32)chunkStartX);
	pOutVertexPos->_tileZ = ((int32)terrainPosZ - (int32)chunkStartZ);

	pOutVertexPos->_relX = (float)(terrainPosX - (float)(chunkStartX + pOutVertexPos->_tileX));
	pOutVertexPos->_relZ = (float)(terrainPosZ - (float)(chunkStartZ + pOutVertexPos->_tileZ));
}

void Terrain::ValidateTerrainChunks(const TerrainTilePos & currentPos, const TerrainTilePos & prevPos)
{
	int32 currentMinX = currentPos._chunkX - 1;
	int32 currentMaxX = currentPos._chunkX + 1;
	int32 currentMinZ = currentPos._chunkZ - 1;
	int32 currentMaxZ = currentPos._chunkZ + 1;

	ClampInt(currentMinX, 0, _xChunkCount - 1);
	ClampInt(currentMaxX, 0, _xChunkCount - 1);
	ClampInt(currentMinZ, 0, _zChunkCount - 1);
	ClampInt(currentMaxZ, 0, _zChunkCount - 1);

	int32 prevMinX = prevPos._chunkX - 1;
	int32 prevMaxX = prevPos._chunkX + 1;
	int32 prevMinZ = prevPos._chunkZ - 1;
	int32 prevMaxZ = prevPos._chunkZ + 1;

	ClampInt(prevMinX, 0, _zChunkCount - 1);
	ClampInt(prevMaxX, 0, _zChunkCount - 1);
	ClampInt(prevMinZ, 0, _zChunkCount - 1);
	ClampInt(prevMaxZ, 0, _zChunkCount - 1);

	std::vector<int32> toDeactivate{};
	toDeactivate.reserve(9);

	_activeChunkIndices.clear();
	_activeChunkIndices.reserve(9);

	for (int32 z = prevMinZ; z <= prevMaxZ; ++z)
	{
		for (int32 x = prevMinX; x <= prevMaxX; ++x)
		{
			toDeactivate.push_back(Index2D(x, z, _xChunkCount));
		}
	}

	for (int32 z = currentMinZ; z <= currentMaxZ; ++z)
	{
		for (int32 x = currentMinX; x <= currentMaxX; ++x)
		{
			_activeChunkIndices.push_back(Index2D(x, z, _xChunkCount));
		}
	}

	for (int32 i = 0; i < toDeactivate.size(); ++i)
	{
		bool32 hasValue = false;
		for (int32 j = 0; j < _activeChunkIndices.size(); ++j)
		{
			if (toDeactivate[i] == _activeChunkIndices[j])
			{
				hasValue = true;
			}
		}
		if (!hasValue)
		{
			_pChunks[toDeactivate[i]].DeactivateEntities();
		}
	}

	for (int32 i = 0; i < _activeChunkIndices.size(); ++i)
	{
		bool32 hasValue = false;
		for (int32 j = 0; j < toDeactivate.size(); ++j)
		{
			if (_activeChunkIndices[i] == toDeactivate[j])
			{
				hasValue = true;
			}
		}
		if (!hasValue)
		{
			_pChunks[_activeChunkIndices[i]].ActivateEntities();
		}
	}
	
	_pCurrentScene->_world.Refresh();
}

void Terrain::EffectSetTexture(LPCSTR handle, LPDIRECT3DTEXTURE9 texture)
{
	VIDEO->GetEffect(_effect)->_ptr->SetTexture(handle, texture);
}

void Terrain::EffectSetMatrix(LPCSTR handle, const Matrix & matrix)
{
	VIDEO->GetEffect(_effect)->SetMatrix(handle, matrix);
}

const Vector3 ConvertChunkPosToWorldPos(const TerrainChunkPos & chunkPos)
{
	return Vector3();
}

const Vector3 ConvertTilePosToWorldPos(const TerrainTilePos & tilePos)
{
	return Vector3();
}

//플레이어가 접근하면 터레인 청크에 있는 모든 엔티티들을 활성화 시킨다
void Terrain::TerrainChunk::ActivateEntities()
{
	for (int32 i = 0; i < TERRAIN_CHUNKS_TILE_COUNT; ++i)
	{
		for (auto &entity : _tiles[i]._entities)
		{
			entity.Activate();
		}
	}
}

//플레이어가 청크에서 멀어진다면.... 터레인 청크에 있는 모든 엔티티들을 비활성화 시킨다
void Terrain::TerrainChunk::DeactivateEntities()
{
	for (int32 i = 0; i < TERRAIN_CHUNKS_TILE_COUNT; ++i)
	{
		for (auto &entity : _tiles[i]._entities)
		{
			entity.Deactivate();
		}
	}
}

//Rebuild Seciton은 인자로 들어온 min, max의 범위를 1씩 증가시켜서 내부 처리를 진행한다.
void Terrain::RebuildSection(int32 minX, int32 maxX, int32 minZ, int32 maxZ)
{
	int32 localMinX = minX - 1;
	int32 localMaxX = maxX + 1;
	int32 localMinZ = minZ - 1;
	int32 localMaxZ = maxZ + 1;

	ClampInt(localMinX, 0, _numVertexX - 1);
	ClampInt(localMaxX, 0, _numVertexX - 1);
	ClampInt(localMinZ, 0, _numVertexZ - 1);
	ClampInt(localMaxZ, 0, _numVertexZ - 1);
	
	int32 numVertX = localMaxX - localMinX + 1;
	int32 numVertZ = localMaxZ - localMinZ + 1;

	int32 triNum = (numVertX - 1) * (numVertZ - 1) * 2;

	Vector3 *vertices = new Vector3[numVertX * numVertZ];
	Vector2 *uvs = new Vector2[numVertX * numVertZ];

	int32 counter = 0;
	for (int32 z = localMinZ; z <= localMaxZ; ++z)
	{
		for (int32 x = localMinX; x <= localMaxX; ++x)
		{
			vertices[counter] = _terrainVertices[Index2D(x, z, _numVertexX)]._pos;
			uvs[counter++] = _terrainVertices[Index2D(x, z, _numVertexX)]._baseUV;
		}
	}

	counter = 0;
	uint32 *indices = new uint32[triNum * 3];
	for (int32 z = 0; z < numVertZ - 1; ++z)
	{
		for (int32 x = 0; x < numVertX - 1; ++x)
		{
			int32 lt = x + z * numVertX;
			int32 rt = (x + 1) + z * numVertX;
			int32 lb = x + (z + 1) * numVertX;
			int32 rb = (x + 1) + (z + 1) * numVertX;

			indices[counter++] = lt;
			indices[counter++] = rt;
			indices[counter++] = lb;

			indices[counter++] = lb;
			indices[counter++] = rt;
			indices[counter++] = rb;
		}
	}

	Vector3 *normals = new Vector3[numVertX * numVertZ];
	Vector3 *binormals = new Vector3[numVertX * numVertZ];
	Vector3 *tangents = new Vector3[numVertX * numVertZ];

	//노말계산
	ComputeNormal(normals, vertices, numVertX * numVertZ, indices, triNum * 3);

	ComputeTangentAndBinormal(tangents, binormals, vertices, normals, uvs, indices, triNum, numVertX * numVertZ);

	counter = 0;
	for (int32 z = localMinZ; z <= localMaxZ; ++z)
	{
		for (int32 x = localMinX; x <= localMaxX; ++x)
		{
			_terrainVertices[Index2D(x, z, _numVertexX)]._normal = normals[counter++];
		}
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(uvs);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(binormals);
	SAFE_DELETE_ARRAY(tangents);
	SAFE_DELETE_ARRAY(indices);
}

void Terrain::SmoothSection(int32 minX, int32 maxX, int32 minZ, int32 maxZ)
{
	int32 numVertX = maxX - minX + 1;
	int32 numVertZ = maxZ - minZ + 1;

	float* smooth = new float[numVertX * numVertZ];

	int32 counter = 0;
	for (int32 z = minZ; z < maxZ; z++)
	{
		for (int32 x = minX; x < maxX; x++)
		{
			int32 adjacentSections = 0;		//몇개의 정점과 평균값을 내니?
			float totalSections = 0.0f;		//주변의 정점 높이 총합은 얼마니?

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

			smooth[counter++] = ( _terrainVertices[(z * _numVertexX) + x]._pos.y + (totalSections / adjacentSections)) * 0.5f;
		}
	}
	counter = 0;

	//위에서 계산된 y 스무싱 적용
	for (int32 z = minZ; z < maxZ; z++)
	{
		for (int32 x = minX; x < maxX; x++)
		{
			_terrainVertices[Index2D(x, z, _numVertexX)]._pos.y = smooth[counter++];

		}
	}
	SAFE_DELETE_ARRAY(smooth);
}

void Terrain::AddHeightGausian(int32 minX, int32 maxX, int32 minZ, int32 maxZ, float mult)
{
	int32 numVertX = maxX - minX + 1;
	int32 numVertZ = maxZ - minZ + 1;

	if (mult > 0)
	{
		mult *= 0.04f;
	}
	else
	{
		mult *= 0.2f;
	}
	float *smooth = new float[numVertX * numVertZ];

	int32 counter = 0;
	for (int32 z = minZ; z < maxZ; z++)
	{
		for (int32 x = minX; x < maxX; x++)
		{
			int32 adjacentSections = 0;		//몇개의 정점과 평균값을 내니?
			float totalSections = 0.0f;		//주변의 정점 높이 총합은 얼마니?

			if ((x - 1) > 0)
			{
				totalSections += _terrainVertices[(z * _numVertexX) + (x - 1)]._pos.y * 2.0f;
				adjacentSections += 2;

				//왼쪽 상단
				if ((z - 1) > 0)
				{
					totalSections += _terrainVertices[((z - 1) * _numVertexX) + (x - 1)]._pos.y * 1.0f;
					adjacentSections += 1;
				}
				//왼쪽 하단
				if ((z + 1) < _numVertexZ)
				{
					totalSections += _terrainVertices[((z + 1) * _numVertexX) + (x - 1)]._pos.y * 1.0f;
					adjacentSections += 1;
				}
			}

			//오른쪽 체크
			if ((x + 1) < _numVertexX)
			{
				totalSections += _terrainVertices[(z * _numVertexX) + (x + 1)]._pos.y * 2.9f;
				adjacentSections += 2;
				//오른쪽 상단
				if ((z - 1) > 0)
				{
					totalSections += _terrainVertices[((z - 1) * _numVertexX) + (x + 1)]._pos.y * 1.0f;
					adjacentSections += 1;
				}
				//오른쪽 하단 
				if ((z + 1) < _numVertexZ)
				{
					totalSections += _terrainVertices[((z + 1) * _numVertexX) + (x + 1)]._pos.y * 1.0f;
					adjacentSections += 1;
				}
			}

			//상단
			if ((z - 1) > 0)
			{
				totalSections += _terrainVertices[((z - 1) * _numVertexX) + x]._pos.y * 2.0f;
				adjacentSections += 2;
			}
			//하단
			if ((z + 1) < _numVertexZ)
			{
				totalSections += _terrainVertices[((z + 1) * _numVertexX) + x]._pos.y * 2.0f;
				adjacentSections += 2;
			}

			totalSections += _terrainVertices[Index2D(x, z, _numVertexX)]._pos.y * 4.0f;
			adjacentSections += 4;

			smooth[counter++] = (totalSections / (float)adjacentSections) * (1.0f + mult);
		}
	}

	counter = 0;
	//위에서 계산된 y 스무싱 적용
	for (int32 z = minZ; z < maxZ; z++)
	{
		for (int32 x = minX; x < maxX; x++)
		{
			_terrainVertices[Index2D(x, z, _numVertexX)]._pos.y = smooth[counter++];

		}
	}
	SAFE_DELETE_ARRAY(smooth);
}

void Terrain::AddHeightBlock(int32 minX, int32 maxX, int32 minZ, int32 maxZ, float mult)
{
}

void Terrain::DrawAlphaTextureOnCursorPos(const Vector2 & cursorPos, float innerRadius, float outterRadius, 
	int32 layer, bool32 subtract)
{
	Ray ray;
	_pCurrentScene->_camera.ComputeRay(cursorPos, &ray);

	Vector3 worldPos;

	if (IsIntersectRay(ray, &worldPos))
	{
		TerrainVertexPos vertexPos;
		ConvertWorldPostoVertexPos(worldPos, &vertexPos);

		int32 centerX = vertexPos._chunkX * TERRAIN_CHUNK_DIM + vertexPos._tileX;
		int32 centerZ = vertexPos._chunkZ * TERRAIN_CHUNK_DIM + vertexPos._tileZ;

		//알파 텍스쳐의 1픽셀이 지형에 대해서 얼마의 크기인지를 구한다..
		float pixelSize = (float)_terrainSizeX / (float)TERRAIN_ALPHA_TEXTURE_SIZE;
		//지형의 브러쉬 크기에 대한 알파 텍스쳐의 브러쉬 크기를 구한다 
		int32 brushSize = (int32)(outterRadius/ pixelSize);

		float tu = centerX / _terrainSizeX;
		float tv = centerZ / _terrainSizeZ;

		int32 centerPixelX = (int32)(tu * TERRAIN_ALPHA_TEXTURE_SIZE);
		int32 centerPixelY = (int32)(tv * TERRAIN_ALPHA_TEXTURE_SIZE);

		int32 minPixelX = centerPixelX - brushSize;
		int32 maxPixelX = centerPixelX + brushSize;
		int32 minPixelY = centerPixelY - brushSize;
		int32 maxPixelY = centerPixelY + brushSize;

		ClampInt(minPixelX, 0, TERRAIN_ALPHA_TEXTURE_SIZE - 1);
		ClampInt(maxPixelX, 0, TERRAIN_ALPHA_TEXTURE_SIZE - 1);
		ClampInt(minPixelY, 0, TERRAIN_ALPHA_TEXTURE_SIZE - 1);
		ClampInt(minPixelY, 0, TERRAIN_ALPHA_TEXTURE_SIZE - 1);

		D3DLOCKED_RECT lockRect{};

		//TODO : 이거 해야한다...
		video::Texture *pTexture = nullptr;
		if (layer == 0)
		{
			pTexture = VIDEO->GetTexture(_tileControl1Handle);
		}
		else if (layer == 1)
		{
			pTexture = VIDEO->GetTexture(_tileControl2Handle);
		}
		if (SUCCEEDED(pTexture->_ptr->LockRect(0, &lockRect, nullptr, 0)))
		{
			uint8 *pStart = (uint8 *)lockRect.pBits;

			uint8 write{};

			for (int32 y = minPixelY; y <= maxPixelY; ++y)
			{
				for (int32 x = minPixelX; x <= maxPixelX; ++x)
				{
					int32 in = (lockRect.Pitch * y) + (x * 4);

					uint8 read = pStart[in + 3];

					Vector3 diff = Vector3(x * pixelSize, 0.0f, y * pixelSize) -
						Vector3(centerPixelX * pixelSize, 0.0f, centerPixelY * pixelSize);
					float length = Vec3Length(&diff);

					if (length <= innerRadius)
					{
						if (subtract)
						{
							write = 0;
						}
						else
						{
							write = 0xff;
						}
					}
					else if(length <= outterRadius + EPSILON)
					{
						length -= innerRadius;
						float range = (float)(outterRadius - innerRadius);
						if (subtract)
						{
							write = 255 - (uint8)(((float)(range - length) / (float)(range)) * 0xff);
						}
						else
						{
							write = (uint8)(((float)(range - length) / (float)(range)) * 0xff);
						}
					}
					else
					{
						continue;
					}

					if (subtract)
					{
						read = (read > write) ? write : read;
					}
					else
					{
						read = (read < write) ? write : read;
					}
					pStart[in + 3] = read;
				}
			}
			pTexture->_ptr->UnlockRect(0);
		}
	}
}

//Terrain Texture loading Function
void Terrain::LoadTextureFromConfig(const Terrain::TerrainConfig & config)
{
	video::TextureHandle loadedHandle = VIDEO->CreateTexture(config._tile0FileName, config._tile0FileName);
	if (loadedHandle.IsValid())
	{
		_tile0Handle = loadedHandle;
	}
	else
	{
		_tile0Handle = VIDEO->GetTexture("defaultDiffuse");
	}

	loadedHandle = VIDEO->CreateTexture(config._tile1FileName, config._tile1FileName);
	if (loadedHandle.IsValid())
	{
		_tile1Handle = loadedHandle;
	}
	else 
	{
		_tile1Handle = VIDEO->GetTexture("defaultDiffuse");
	}

	loadedHandle = VIDEO->CreateTexture(config._tile2FileName, config._tile2FileName);
	if (loadedHandle.IsValid())
	{
		_tile2Handle = loadedHandle;
	}
	else 
	{
		_tile2Handle = VIDEO->GetTexture("defaultDiffuse");
	}

	bool controlLoaded = false;
	//만약 config에서 Splat 텍스쳐가 넘어왔다면
	if (strlen(config._control1Name) > 0)
	{
		_tileControl1Handle = VIDEO->CreateTexture(config._control1Name);
		_tileControl2Handle = VIDEO->CreateTexture(config._control2Name);
		if (!_tileControl1Handle.IsValid())
		{
			_tileControl1Handle = VIDEO->GetTexture("diffuseDefault");
		}

		if (!_tileControl2Handle.IsValid())
		{
			_tileControl2Handle = VIDEO->GetTexture("diffuseDefault");
		}
		else
		{
			controlLoaded = true;
		}
	}
	//만약 config에서 Splat 텍스쳐가 넘어오지 않았다면, 새로운 텍스쳐를 두개 생성하도록 한다...
	//그리고 모든 값을 0x00000000로 채운다.
	if(!controlLoaded)
	{
		_tileControl1Handle = VIDEO->CreateTexture(TERRAIN_ALPHA_TEXTURE_SIZE, TERRAIN_ALPHA_TEXTURE_SIZE,
			D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);

		_tileControl2Handle = VIDEO->CreateTexture(TERRAIN_ALPHA_TEXTURE_SIZE, TERRAIN_ALPHA_TEXTURE_SIZE,
			D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);

		video::Texture *pControl1= VIDEO->GetTexture(_tileControl1Handle);
		D3DLOCKED_RECT lockRect{};
		if (SUCCEEDED(pControl1->_ptr->LockRect(0, &lockRect, nullptr, 0)))
		{
			uint32 *pPixel = (uint32 *)lockRect.pBits;
			for (int32 y = 0; y < TERRAIN_ALPHA_TEXTURE_SIZE; ++y)
			{
				for (int32 x = 0; x < TERRAIN_ALPHA_TEXTURE_SIZE; ++x)
				{
					*pPixel = 0x00000000;
					pPixel++;
				}
			}
			pControl1->_ptr->UnlockRect(0);
		}
		//알파 텍스쳐의 Lock 을 실패하였다...
		else
		{
			Console::Log("Alpha Texture Lock failed\n");
		}

		video::Texture *pControl2 = VIDEO->GetTexture(_tileControl2Handle);
		if (SUCCEEDED(pControl2->_ptr->LockRect(0, &lockRect, nullptr, 0)))
		{
			uint32 *pPixel = (uint32 *)lockRect.pBits;
			for (int32 y = 0; y < TERRAIN_ALPHA_TEXTURE_SIZE; ++y)
			{
				for (int32 x = 0; x < TERRAIN_ALPHA_TEXTURE_SIZE; ++x)
				{
					*pPixel = 0x00000000;
					pPixel++;
				}
			}
			pControl2->_ptr->UnlockRect(0);
		}
		//알파 텍스쳐의 Lock 을 실패하였다...
		else
		{
			Console::Log("Alpha Texture Lock failed\n");
		}
	}
}

Terrain::TerrainConfig::TerrainConfig(const TerrainConfig & other)
{
	this->_xChunkCount = other._xChunkCount;
	this->_zChunkCount = other._zChunkCount;
	this->_textureMult = other._textureMult;

	strncpy(this->_tile0FileName, other._tile0FileName, MAX_FILE_NAME);
	strncpy(this->_tile1FileName, other._tile1FileName, MAX_FILE_NAME);
	strncpy(this->_tile2FileName, other._tile2FileName, MAX_FILE_NAME);
	strncpy(this->_control1Name, other._control1Name, MAX_FILE_NAME);
	strncpy(this->_control2Name, other._control2Name, MAX_FILE_NAME);
}

Terrain::TerrainConfig & Terrain::TerrainConfig::operator=(const TerrainConfig & other)
{
	this->_xChunkCount = other._xChunkCount;
	this->_zChunkCount = other._zChunkCount;
	this->_textureMult = other._textureMult;

	strncpy(this->_tile0FileName, other._tile0FileName, MAX_FILE_NAME);
	strncpy(this->_tile1FileName, other._tile1FileName, MAX_FILE_NAME);
	strncpy(this->_tile2FileName, other._tile2FileName, MAX_FILE_NAME);
	strncpy(this->_control1Name, other._control1Name, MAX_FILE_NAME);
	strncpy(this->_control2Name, other._control2Name, MAX_FILE_NAME);
	return *this;
}

TerrainTilePos::TerrainTilePos()
	:_chunkX(), _chunkZ(), 
	_tileX(), _tileZ(), 
	_relX(), _relZ()
{
}

TerrainTilePos::TerrainTilePos(const TerrainTilePos & other)
	:_chunkX(other._chunkX), _chunkZ(other._chunkZ), 
	_tileX(other._tileX), _tileZ(other._tileZ), 
	_relX(other._relX), _relZ(other._relZ)
{
}

TerrainTilePos & TerrainTilePos::operator=(const TerrainTilePos & other)
{
	_chunkX = other._chunkX;
	_chunkZ = other._chunkZ;
	_tileX = other._tileX;
	_tileZ = other._tileZ;
	_relX = other._relX;
	_relZ = other._relZ;
	return *this;
}

bool TerrainTilePos::operator==(const TerrainTilePos & other)
{
	return (_chunkX == other._chunkX) &&
		(_chunkZ == other._chunkZ) &&
		(_tileX == other._tileX) &&
		(_tileZ == other._tileZ);
}
