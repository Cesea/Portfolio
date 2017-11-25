#include "stdafx.h"
#include "Terrain.h"

#include "QuadTree.h"

#include "BaseScene.h"

Terrain::~Terrain()
{
}

bool Terrain::Create(const Terrain::TerrainConfig &config, bool32 inEditMode)
{
	_inEditMode = inEditMode;

	//가로세로 정점 수를 구한다.
	//가로 세로 정점의 수는 TILE_CHUNK_DIM * tileExtent + 1과 같다..
	_numVertexX = TERRAIN_CHUNK_DIM * config._xChunkCount + 1;		
	_numVertexZ = TERRAIN_CHUNK_DIM * config._zChunkCount + 1;		
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

	//Terrain config 복사
	_currentConfig._xChunkCount = config._xChunkCount;
	_currentConfig._zChunkCount = config._zChunkCount;
	_currentConfig._textureMult = config._textureMult;
	

	if (!(CreateTerrain(config._textureMult)))
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
	//_materialHandle = VIDEO->CreateMaterial("TerrainMaterial");

	if (_currentConfig._tile0FileName != config._tile0FileName)
	{
		video::TextureHandle loadedHandle = VIDEO->CreateTexture(config._tile0FileName, config._tile0FileName);
		if (loadedHandle.IsValid())
		{
			_tile0Handle = loadedHandle;
		}
	}

	if (_currentConfig._tile1FileName != config._tile1FileName)
	{
		video::TextureHandle loadedHandle = VIDEO->CreateTexture(config._tile1FileName, config._tile1FileName);
		if (loadedHandle.IsValid())
		{
			_tile1Handle = loadedHandle;
		}
	}

	if (_currentConfig._tile2FileName !=  config._tile2FileName)
	{
		video::TextureHandle loadedHandle = VIDEO->CreateTexture(config._tile2FileName, config._tile2FileName);
		if (loadedHandle.IsValid())
		{
			_tile2Handle = loadedHandle;
		}
	}
	if (_currentConfig._tile2FileName !=  config._tile2FileName)
	{
		video::TextureHandle loadedHandle = VIDEO->CreateTexture(config._tile3FileName, config._tile3FileName);
		if (loadedHandle.IsValid())
		{
			_tile3Handle = loadedHandle;
		}
	}
	if (config._splatFileName.length() > 0)
	{
		_tileSplatHandle = VIDEO->CreateTexture(config._splatFileName);
		if (!_tileSplatHandle.IsValid())
		{
			_tileSplatHandle = VIDEO->GetTexture("diffuseDefault");
		}
	}

	_currentConfig._tile0FileName = config._tile0FileName;
	_currentConfig._tile1FileName = config._tile1FileName;
	_currentConfig._tile2FileName = config._tile2FileName;
	_currentConfig._tile3FileName = config._tile3FileName;
	_currentConfig._splatFileName = config._splatFileName;

	//VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE0, _tile0Handle);
	//VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE1, _tile1Handle);
	//VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE2, _tile2Handle);
	//VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE3, _tile3Handle);
	//VIDEO->MaterialAddTexture(_materialHandle, VIDEO_TEXTURE4, _tileSplatHandle);
}

void Terrain::RegisterEvents()
{
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
	VIDEO->DestroyTexture(_tile3Handle);
	VIDEO->DestroyTexture(_tileSplatHandle);

	//VIDEO->DestroyMaterial(_materialHandle);

	SAFE_DELETE_ARRAY(_terrainVertices);
	SAFE_DELETE_ARRAY(_chunkIndex);
	SAFE_DELETE(_pQuadTree);
}

void Terrain::SaveTerrain(const std::string & fileName)
{
}

void Terrain::LoadTerrain(const std::string & fileName)
{
}

//void Terrain::FillRenderCommand(video::RenderView & renderView)
//{
//
//	//video::IndexBuffer *pIndexBuffer = VIDEO->GetIndexBuffer(_iHandle);
//	//uint8 *pData = nullptr;
//	//pIndexBuffer->_ptr->Lock(0, 0, (void **)&pData, D3DLOCK_DISCARD);
//	//_numTriangleToDraw = _pQuadTree->GenerateIndex(pData, renderView._pCamera->GetFrustum(), 
//	//	renderView._pCamera->GetTransform()._position, _lodRatio);
//	//pIndexBuffer->_ptr->Unlock();
//
//	if (_inEditMode)
//	{
//		video::RenderCommand &command = renderView.GetCommand();
//		command._drawType = video::RenderCommand::DrawType::eStatic;
//		command._primType = video::RenderCommand::PrimType::eTriangleList;
//		Assert(_vHandle.IsValid());
//		Assert(_iHandle.IsValid());
//
//		video::VertexBuffer *pVBuffer = VIDEO->GetVertexBuffer(_vHandle);
//		void *pVertexData = nullptr;
//		pVBuffer->_ptr->Lock(0, 0, (void **)&pVertexData, D3DLOCK_DISCARD);
//		memcpy(pVertexData, _terrainVertices, sizeof(video::TerrainVertex) * _numTotalVertex);
//		pVBuffer->_ptr->Unlock();
//
//		Assert(_terrainFaces);
//		video::IndexBuffer *pIBuffer = VIDEO->GetIndexBuffer(_iHandle);
//		void *pIndexData = nullptr;
//		pIBuffer->_ptr->Lock(0, 0, (void **)&pIndexData, D3DLOCK_DISCARD);
//		memcpy(pIndexData, _terrainFaces, sizeof(TerrainFace) * _numTotalFace);
//		pIBuffer->_ptr->Unlock();
//
//		command._vHandle = _vHandle;
//		command._iHandle = _iHandle;
//		command._effectHandle = _effect;
//		command._materialHandle = _materialHandle;
//		command._numPrim = _numTriangleToDraw;
//	}
//	else
//	{
//		video::RenderCommand &command = renderView.GetCommand();
//		command._drawType = video::RenderCommand::DrawType::eStatic;
//		command._primType = video::RenderCommand::PrimType::eTriangleList;
//		Assert(_vHandle.IsValid());
//		Assert(_iHandle.IsValid());
//		command._vHandle = _vHandle;
//		command._iHandle = _iHandle;
//		command._effectHandle = _effect;
//		command._materialHandle = _materialHandle;
//		command._numPrim = _numTriangleToDraw;
//	}
//
//
//	//for (int32 i = 0; i < _numSectionX * _numSectionZ; ++i)
//	//{
//	//	Terrain::TerrainSection &refSection = _pSections[i];
//	//	if (renderView._pCamera->GetFrustum().IsSphereInFrustum(Vector3(refSection._centerX, 0.0f, refSection._centerZ), refSection._radius))
//	//	{
//	//		video::RenderCommand &command = renderView.GetCommand();
//	//		command._drawType = video::RenderCommand::DrawType::eStatic;
//	//		command._primType = video::RenderCommand::PrimType::eTriangleList;
//	//		Assert(refSection._vHandle.IsValid());
//	//		Assert(refSection._iHandle.IsValid());
//	//		command._vHandle = refSection._vHandle;
//	//		command._iHandle = refSection._iHandle;
//	//		command._effectHandle = _effect;
//	//		command._materialHandle = _materialHandle;
//	//	}
//	//}
//}


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

void Terrain::Render(const Camera & camera)
{
	//월드 행렬셋팅
	video::Effect *pEffect = VIDEO->GetEffect(_effect);
	//video::VertexBuffer *vBuffer = VIDEO->GetVertexBuffer(_vHandle);
	//video::IndexBuffer *iBuffer = VIDEO->GetIndexBuffer(_iHandle);
	//video::VertexDecl *decl = VIDEO->GetVertexDecl(vBuffer->_decl);

	Matrix matInd;
	MatrixIdentity(&matInd);
	pEffect->SetMatrix("matWorld", matInd);

	//뷰 행렬셋팅
	pEffect->SetMatrix("matViewProjection", camera.GetViewProjectionMatrix());
	pEffect->SetMatrix("baseDirectionalLight", _pCurrentScene->_pMainLight->GetLightMatrix());

	//Texture 셋팅
	pEffect->SetTexture("Terrain0_Tex", *VIDEO->GetTexture(_tile0Handle));
	pEffect->SetTexture("Terrain1_Tex", *VIDEO->GetTexture(_tile1Handle));
	pEffect->SetTexture("Terrain2_Tex", *VIDEO->GetTexture(_tile2Handle));
	pEffect->SetTexture("Terrain3_Tex", *VIDEO->GetTexture(_tile3Handle));
	pEffect->SetTexture("TerrainControl_Tex", *VIDEO->GetTexture(_tileSplatHandle));

	pEffect->SetTechnique("Base");

	if (_inEditMode)
	{
		for (int32 z = 0; z < _zChunkCount; ++z)
		{
			for (int32 x = 0; x < _xChunkCount; ++x)
			{
				TerrainChunk &refChunk = _pChunks[Index2D(x, z, _xChunkCount)];
				if (camera.GetFrustum().IsSphereInFrustum( Vector3(refChunk._relCenterX, 0.0f, refChunk._relCenterZ), refChunk._radius))
				{
					video::VertexBuffer *vBuffer = VIDEO->GetVertexBuffer(refChunk._vHandle);
					video::IndexBuffer *iBuffer = VIDEO->GetIndexBuffer(refChunk._iHandle);
					video::VertexDecl *decl = VIDEO->GetVertexDecl(vBuffer->_decl);

					//버텍스 버퍼에 정점 정보를 넣는다
					video::TerrainVertex *pVertexData = nullptr;
					video::TerrainVertex *pToCopy = _terrainVertices;
					vBuffer->_ptr->Lock(0, 0, (void **)&pVertexData, D3DLOCK_DISCARD);
					for (int32 i = 0; i < TERRAIN_CHUNK_DIM + 1; ++i)
					{
						int32 vertexXOffset = (x * TERRAIN_CHUNK_DIM);
						int32 vertexZOffset = (z * TERRAIN_CHUNK_DIM) + i;

						int32 offset = i * (TERRAIN_CHUNK_DIM + 1);
						int32 toCopyOffset = Index2D(vertexXOffset, vertexZOffset, _numVertexX);

						memcpy(pVertexData + offset, pToCopy + toCopyOffset, sizeof(video::TerrainVertex) * (TERRAIN_CHUNK_DIM + 1));
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

					for (uint32 i = 0; i < numPass; i++)
					{
						pEffect->BeginPass(i);

						gpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
							_sectionNumVertexX * _sectionNumVertexZ, 0, _sectionNumCellX * _sectionNumCellX * 2);

						pEffect->EndPass();
					}
					pEffect->EndEffect();
				}

			}
		}
	}
	else
	{
		for (int32 i = 0; i < _xChunkCount * _zChunkCount; ++i)
		{
			TerrainChunk &refChunk = _pChunks[i];
			if (camera.GetFrustum().IsSphereInFrustum(
				Vector3(refChunk._relCenterX, 0.0f, refChunk._relCenterZ), refChunk._radius))
			{
				video::VertexBuffer *vBuffer = VIDEO->GetVertexBuffer(refChunk._vHandle);
				video::IndexBuffer *iBuffer = VIDEO->GetIndexBuffer(refChunk._iHandle);
				video::VertexDecl *decl = VIDEO->GetVertexDecl(vBuffer->_decl);

				gpDevice->SetStreamSource(0, vBuffer->_ptr, 0, sizeof(video::TerrainVertex));
				gpDevice->SetVertexDeclaration(decl->_ptr);
				gpDevice->SetIndices(iBuffer->_ptr);

				uint32 numPass = pEffect->BeginEffect();

				for (uint32 i = 0; i < numPass; i++)
				{
					pEffect->BeginPass(i);

					gpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
						_sectionNumVertexX * _sectionNumVertexZ, 0, _sectionNumCellX * _sectionNumCellX * 2);

					pEffect->EndPass();
				}
				pEffect->EndEffect();
			}
		}
	}
}

void Terrain::AddEntityToSection(const Entity & entity, const Vector3 & position)
{
	
}

bool Terrain::CreateTerrain(int32 tileNum)
{
	float tileIntervalX = static_cast<float>(tileNum) / _numCellX;
	float tileIntervalY = static_cast<float>(tileNum) / _numCellZ;

	float terrainStartX = -(_numCellX / 2);
	float terrainStartZ = (_numCellZ / 2);

	_terrainVertices = new video::TerrainVertex[_numTotalVertex];

	for (int32 z = 0; z < _numVertexZ; z++) 
	{
		for (int32 x = 0; x < _numVertexX; x++) 
		{
			Vector3 _pos;

			//정점의 x, z 위치 계산
			_pos.x = terrainStartX + x;
			_pos.z = terrainStartZ - z;

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
	//터레인 스무싱 
	//SmoothTerrain(smooth);
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
			CreateTerrainSection(x, z, _terrainVertices);
		}
	}

	SAFE_DELETE_ARRAY(poses);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(tangents);
	SAFE_DELETE_ARRAY(binormals);
	SAFE_DELETE_ARRAY(uvs);

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
	if (_tile3Handle.IsValid()) { VIDEO->DestroyTexture(_tile3Handle); }
	if (_tileSplatHandle.IsValid()) { VIDEO->DestroyTexture(_tileSplatHandle); }


	SAFE_DELETE_ARRAY(_terrainVertices);
	SAFE_DELETE_ARRAY(_chunkIndex);
	SAFE_DELETE(_pQuadTree);

	//Rebuild모드는 항상 에디터에서 불린다.
	Create(config, true);
}

bool Terrain::CreateTerrainSection(int32 x, int32 z, const video::TerrainVertex * pTerrainVertices)
{
	int32 sectionIndex = Index2D(x, z, _xChunkCount);

	TerrainChunk &refSection = _pChunks[sectionIndex];

	refSection._chunkX = x;
	refSection._chunkZ = z;

	int32 globalStartX = x * _sectionResolution;
	int32 globalStartZ = z * _sectionResolution;

	std::vector<video::TerrainVertex> vertices;
	//std::vector<uint16> indices;

	vertices.reserve(((_sectionNumVertexX ) * (_sectionNumVertexZ )) + 1);
	//indices.reserve((_sectionNumCellX * (_sectionNumCellZ + 1)) * 3 * 2 + 1);
	
	vertices.clear();
	//indices.clear();

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

	refSection._relStartX = vertices[0]._pos.x;
	refSection._relStartZ = vertices[0]._pos.z;

	refSection._relEndX = vertices.back()._pos.x;
	refSection._relEndZ = vertices.back()._pos.z;

	

	if (_inEditMode)
	{
		Memory mem;

		mem._data = nullptr;
		mem._size = sizeof(video::TerrainVertex) * _sectionNumVertexX * _sectionNumVertexZ;

		refSection._vHandle = VIDEO->CreateVertexBuffer(&mem, _declHandle);
		Assert(refSection._vHandle.IsValid());

		mem._data = nullptr;
		mem._size = sizeof(uint32) * _sectionResolution * _sectionResolution * 2 * 3;

		if (!refSection._iHandle.IsValid())
		{
			refSection._iHandle = VIDEO->CreateIndexBuffer(&mem, sizeof(uint32));
		}

		Assert(refSection._iHandle.IsValid());
	}
	else
	{
		Memory mem;

		mem._data = &vertices[0];
		mem._size = sizeof(video::TerrainVertex) * _sectionNumVertexX * _sectionNumVertexZ;

		refSection._vHandle = VIDEO->CreateVertexBuffer(&mem, _declHandle);
		Assert(refSection._vHandle.IsValid());

		mem._data = &_chunkIndex[0];
		mem._size = sizeof(uint32) * _sectionResolution * _sectionResolution * 2 * 3;

		if (!refSection._iHandle.IsValid())
		{
			refSection._iHandle = VIDEO->CreateIndexBuffer(&mem, sizeof(uint32));
		}
		Assert(refSection._iHandle.IsValid());
	}

	refSection._relCenterX = (refSection._relStartX + refSection._relEndX) * 0.5f;
	refSection._relCenterZ = (refSection._relStartZ + refSection._relEndZ) * 0.5f;
	refSection._radius = (refSection._relCenterX - refSection._relStartX) * 1.2f;

	return true;
}

void Terrain::AddHeightOnCursorPos(const Vector2 & cursorPos, float brushRadius, float intensity)
{
	Ray ray;
	_pCurrentScene->_camera.ComputeRay(cursorPos, &ray);

	int32 radius = (int32)brushRadius;

	Vector3 worldPos;
	if (IsIntersectRay(ray, &worldPos))
	{
		TerrainTilePos tilePos =  ConvertWorldPostoTilePos(worldPos);

		for (int32 z = -radius; z <= radius; ++z)
		{
			for (int32 x = -radius; x <= radius; ++x)
			{
				int32 globalX = tilePos._chunkX * TERRAIN_CHUNK_DIM + tilePos._tileX;
				int32 globalZ = tilePos._chunkZ * TERRAIN_CHUNK_DIM + tilePos._tileZ;

				_terrainVertices[Index2D(globalX, globalZ, _numVertexX)]._pos.y += 1;
				//ComputeTangentAndBinormal
			}
		}
	}
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

TerrainTilePos Terrain::ConvertWorldPostoTilePos(const Vector3 & worldPos)
{
	TerrainTilePos result;
	
	float terrainPosX = worldPos.x + (float)_terrainHalfSizeX;
	float terrainPosZ = -worldPos.z + (float)_terrainHalfSizeZ;

	result._chunkX = (int32)(terrainPosX / TERRAIN_CHUNK_DIM);
	result._chunkZ = (int32)(terrainPosZ / TERRAIN_CHUNK_DIM);
	int32 chunkStartX = result._chunkX * TERRAIN_CHUNK_DIM;
	int32 chunkStartZ = result._chunkZ * TERRAIN_CHUNK_DIM;

	result._tileX = ((int32)terrainPosX - (int32)chunkStartX);
	result._tileZ = ((int32)terrainPosZ - (int32)chunkStartZ);

	result._relX = (float)(terrainPosX - (float)(chunkStartX + result._tileX));
	result._relZ = (float)(terrainPosZ - (float)(chunkStartZ + result._tileZ));

	return result;
}

const Vector3 ConvertChunkPosToWorldPos(const TerrainChunkPos & chunkPos)
{
	return Vector3();
}

const Vector3 ConvertTilePosToWorldPos(const TerrainTilePos & tilePos)
{
	return Vector3();
}

void Terrain::TerrainChunk::ValidateEntities()
{
	for (int32 i = 0; i < _entities.size(); ++i)
	{
		_entities[i].Deactivate();
	}
}

void Terrain::TerrainChunk::InvalidateEntities()
{
	for (int32 i = 0; i < _entities.size(); ++i)
	{
		_entities[i].Activate();
	}
}
