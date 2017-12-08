#include "stdafx.h"
#include "VideoResource_Mesh.h"

#define DEFAULT_MATRIX_PALETTE_SIZE (45)

namespace video
{
	EffectHandle StaticXMesh::_sEffectHandle;
	EffectHandle SkinnedXMesh::_sEffectHandle;

	void CalculateMeshBoundInfo(ID3DXMesh *pMesh, const Matrix &correction, MeshBoundInfo *pOutBoundInfo)
	{
		D3DVERTEXELEMENT9 vertexElements[MAX_FVF_DECL_SIZE];

		pMesh->GetDeclaration(vertexElements);
		int32 positionOffset = -1;
		int32 normalOffet = -1;
		int32 tangentOffet = -1;
		int32 binormalOffet = -1;

		//일단 돌아재낀다..
		for (uint32 i = 0; i < MAX_FVF_DECL_SIZE; i++)
		{
			if (vertexElements[i].Type == D3DDECLTYPE_UNUSED)
			{
				break;
			}
			if (vertexElements[i].Usage == D3DDECLUSAGE_POSITION)
			{
				positionOffset = vertexElements[i].Offset;
			}
			else if (vertexElements[i].Usage == D3DDECLUSAGE_NORMAL)
			{
				normalOffet = vertexElements[i].Offset;
			}
			//정점탄젠트 정보를 만났다면...
			else if (vertexElements[i].Usage == D3DDECLUSAGE_TANGENT)
			{
				tangentOffet = vertexElements[i].Offset;
			}
			//정점바이노말 정보를 만났다면...
			else if (vertexElements[i].Usage == D3DDECLUSAGE_BINORMAL)
			{
				binormalOffet = vertexElements[i].Offset;
			}
		}

		uint32 vertexStride = D3DXGetDeclVertexSize(vertexElements, 0);

		//메쉬의 버텍스 버퍼를 Lock 한다
		void* pVertexData = nullptr;
		pMesh->LockVertexBuffer(0, &pVertexData);

		//바운드 MinMax 계산을 위한 초기화......
		pOutBoundInfo->_min = Vector3(0, 0, 0);
		pOutBoundInfo->_max = Vector3(0, 0, 0);

		//버텍스 수만클 돌아 재낀다....
		for (uint32 i = 0; i < pMesh->GetNumVertices(); i++)
		{
			//버텍스 시작 주소
			uint8* pVertex = ((uint8*)pVertexData + (i * vertexStride));

			//정점 위치가 있다면...
			if (positionOffset != -1)
			{
				Vector3* pos = (Vector3*)(pVertex + positionOffset);

				Vec3TransformCoord(pos, pos, &correction);

				//정점 최소 값갱신
				if (pOutBoundInfo->_min.x > pos->x)		pOutBoundInfo->_min.x = pos->x;
				if (pOutBoundInfo->_min.y > pos->y)		pOutBoundInfo->_min.y = pos->y;
				if (pOutBoundInfo->_min.z > pos->z)		pOutBoundInfo->_min.z = pos->z;

				//정점 최대 값갱신
				if (pOutBoundInfo->_max.x < pos->x)		pOutBoundInfo->_max.x = pos->x;
				if (pOutBoundInfo->_max.y < pos->y)		pOutBoundInfo->_max.y = pos->y;
				if (pOutBoundInfo->_max.z < pos->z)		pOutBoundInfo->_max.z = pos->z;
			}

			//노말정보가 있다면..
			if (normalOffet != -1)
			{
				Vector3* nor = (Vector3*)(pVertex + normalOffet);
				Vec3TransformNormal(nor, nor, &correction);
				Vec3Normalize(nor, nor);
			}
			//tangent 정보가 있다면.
			if (tangentOffet != -1)
			{
				Vector3* tangent = (Vector3*)(pVertex + tangentOffet);
				Vec3TransformNormal(tangent, tangent, &correction);
				Vec3Normalize(tangent, tangent);
			}
			//binormal 정보가 있다면
			if (binormalOffet != -1)
			{
				Vector3* binor = (Vector3*)(pVertex + binormalOffet);
				Vec3TransformNormal(binor, binor, &correction);
				Vec3Normalize(binor, binor);
			}
		}
		pMesh->UnlockVertexBuffer();

		//Bound 추가 계산
		pOutBoundInfo->_center = (pOutBoundInfo->_min + pOutBoundInfo->_max) * 0.5f;

		pOutBoundInfo->_size =
			Vector3(pOutBoundInfo->_max.x - pOutBoundInfo->_min.x,
				pOutBoundInfo->_max.y - pOutBoundInfo->_min.y,
				pOutBoundInfo->_max.z - pOutBoundInfo->_min.z);

		pOutBoundInfo->_halfSize = pOutBoundInfo->_size * 0.5f;
		pOutBoundInfo->_radius = D3DXVec3Length(&(pOutBoundInfo->_center - pOutBoundInfo->_min));
	}

	void ResizeMeshAndGetInfos(ID3DXMesh *pMesh, const Matrix &correction,
		MeshVertInfo *pOutVertInfo, MeshBoundInfo *pOutBoundInfo)
	{
		D3DVERTEXELEMENT9 vertexElements[MAX_FVF_DECL_SIZE];

		pMesh->GetDeclaration(vertexElements);
		int32 positionOffset = -1;
		int32 normalOffet = -1;
		int32 tangentOffet = -1;
		int32 binormalOffet = -1;

		//일단 돌아재낀다..
		for (uint32 i = 0; i < MAX_FVF_DECL_SIZE; i++)
		{
			if (vertexElements[i].Type == D3DDECLTYPE_UNUSED)
			{
				break;
			}
			if (vertexElements[i].Usage == D3DDECLUSAGE_POSITION)
			{
				positionOffset = vertexElements[i].Offset;
			}
			else if (vertexElements[i].Usage == D3DDECLUSAGE_NORMAL)
			{
				normalOffet = vertexElements[i].Offset;
			}
			//정점탄젠트 정보를 만났다면...
			else if (vertexElements[i].Usage == D3DDECLUSAGE_TANGENT)
			{
				tangentOffet = vertexElements[i].Offset;
			}
			//정점바이노말 정보를 만났다면...
			else if (vertexElements[i].Usage == D3DDECLUSAGE_BINORMAL)
			{
				binormalOffet = vertexElements[i].Offset;
			}
		}

		pOutVertInfo->_numVertices = pMesh->GetNumVertices();
		pOutVertInfo->_positions.reserve(pOutVertInfo->_numVertices + 1);
		uint32 vertexStride = D3DXGetDeclVertexSize(vertexElements, 0);

		//메쉬의 버텍스 버퍼를 Lock 한다
		void* pVertexData = nullptr;
		pMesh->LockVertexBuffer(0, &pVertexData);

		//바운드 MinMax 계산을 위한 초기화......
		pOutBoundInfo->_min = Vector3(0, 0, 0);
		pOutBoundInfo->_max = Vector3(0, 0, 0);

		//버텍스 수만클 돌아 재낀다....
		for (uint32 i = 0; i < pOutVertInfo->_numVertices; i++)
		{
			//버텍스 시작 주소
			uint8* pVertex = ((uint8*)pVertexData + (i * vertexStride));

			//정점 위치가 있다면...
			if (positionOffset != -1)
			{
				Vector3* pos = (Vector3*)(pVertex + positionOffset);

				Vec3TransformCoord(pos, pos, &correction);

				//정점 최소 값갱신
				if (pOutBoundInfo->_min.x > pos->x)		pOutBoundInfo->_min.x = pos->x;
				if (pOutBoundInfo->_min.y > pos->y)		pOutBoundInfo->_min.y = pos->y;
				if (pOutBoundInfo->_min.z > pos->z)		pOutBoundInfo->_min.z = pos->z;

				//정점 최대 값갱신
				if (pOutBoundInfo->_max.x < pos->x)		pOutBoundInfo->_max.x = pos->x;
				if (pOutBoundInfo->_max.y < pos->y)		pOutBoundInfo->_max.y = pos->y;
				if (pOutBoundInfo->_max.z < pos->z)		pOutBoundInfo->_max.z = pos->z;

				//정점 위치 푸쉬
				pOutVertInfo->_positions.push_back(*pos);
			}

			//노말정보가 있다면..
			if (normalOffet != -1)
			{
				Vector3* nor = (Vector3*)(pVertex + normalOffet);
				Vec3TransformNormal(nor, nor, &correction);
				Vec3Normalize(nor, nor);
				pOutVertInfo->_normals.push_back(*nor);
			}
			//tangent 정보가 있다면.
			if (tangentOffet != -1)
			{
				Vector3* tangent = (Vector3*)(pVertex + tangentOffet);
				Vec3TransformNormal(tangent, tangent, &correction);
				Vec3Normalize(tangent, tangent);
			}
			//binormal 정보가 있다면
			if (binormalOffet != -1)
			{
				Vector3* binor = (Vector3*)(pVertex + binormalOffet);
				Vec3TransformNormal(binor, binor, &correction);
				Vec3Normalize(binor, binor);
			}
		}
		pMesh->UnlockVertexBuffer();

		//Bound 추가 계산
		pOutBoundInfo->_center = (pOutBoundInfo->_min + pOutBoundInfo->_max) * 0.5f;

		pOutBoundInfo->_size =
			Vector3(pOutBoundInfo->_max.x - pOutBoundInfo->_min.x,
				pOutBoundInfo->_max.y - pOutBoundInfo->_min.y,
				pOutBoundInfo->_max.z - pOutBoundInfo->_min.z);

		pOutBoundInfo->_halfSize = pOutBoundInfo->_size * 0.5f;
		pOutBoundInfo->_radius = D3DXVec3Length(&(pOutBoundInfo->_center - pOutBoundInfo->_min));

		LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
		pMesh->GetIndexBuffer(&pIndexBuffer);

		//면의 갯수
		pOutVertInfo->_numFaces = pMesh->GetNumFaces();

		//인덱스 버퍼에 대한 정보를 얻는다.
		D3DINDEXBUFFER_DESC desc;
		pIndexBuffer->GetDesc(&desc);

		if (desc.Format == D3DFMT_INDEX16)
		{
			uint16* pIndexData = NULL;
			pIndexBuffer->Lock(0, 0, (void**)&pIndexData, 0);

			for (uint32 i = 0; i < pOutVertInfo->_numFaces; i++)
			{
				pOutVertInfo->_indices.push_back(pIndexData[i * 3 + 0]);
				pOutVertInfo->_indices.push_back(pIndexData[i * 3 + 1]);
				pOutVertInfo->_indices.push_back(pIndexData[i * 3 + 2]);

			}
			pIndexBuffer->Unlock();
		}
		else if (desc.Format == D3DFMT_INDEX32)
		{
			uint32* pIndexData = NULL;
			pIndexBuffer->Lock(0, 0, (void**)&pIndexData, 0);

			for (uint32 i = 0; i < pOutVertInfo->_numFaces; i++)
			{
				pOutVertInfo->_indices.push_back(pIndexData[i * 3 + 0]);
				pOutVertInfo->_indices.push_back(pIndexData[i * 3 + 1]);
				pOutVertInfo->_indices.push_back(pIndexData[i * 3 + 2]);
			}

			pIndexBuffer->Unlock();
		}
		//얻어온 인덱스 버퍼는 해재
		SAFE_RELEASE(pIndexBuffer);
	}

	void CalculateBoundInfo(std::vector<Vector3>& positions, MeshBoundInfo * pOutBoundInfo, uint32 startVertex, uint32 endVertex)
	{
		memset(pOutBoundInfo, 0, sizeof(MeshBoundInfo));
		Assert(startVertex >= 0);
		Assert(endVertex <= positions.size());
		for (uint32 i = startVertex; i < endVertex; ++i)
		{
			const Vector3 &refVertex = positions[i];
			//정점 최소 값갱신
			if (pOutBoundInfo->_min.x > refVertex.x)		pOutBoundInfo->_min.x = refVertex.x;
			if (pOutBoundInfo->_min.y > refVertex.y)		pOutBoundInfo->_min.y = refVertex.y;
			if (pOutBoundInfo->_min.z > refVertex.z)		pOutBoundInfo->_min.z = refVertex.z;

			//정점 최대 값갱신
			if (pOutBoundInfo->_max.x < refVertex.x)		pOutBoundInfo->_max.x = refVertex.x;
			if (pOutBoundInfo->_max.y < refVertex.y)		pOutBoundInfo->_max.y = refVertex.y;
			if (pOutBoundInfo->_max.z < refVertex.z)		pOutBoundInfo->_max.z = refVertex.z;

		}
		//Bound 추가 계산
		pOutBoundInfo->_center = (pOutBoundInfo->_min + pOutBoundInfo->_max) * 0.5f;

		pOutBoundInfo->_size =
			Vector3(pOutBoundInfo->_max.x - pOutBoundInfo->_min.x,
				pOutBoundInfo->_max.y - pOutBoundInfo->_min.y,
				pOutBoundInfo->_max.z - pOutBoundInfo->_min.z);

		pOutBoundInfo->_halfSize = pOutBoundInfo->_size * 0.5f;
		pOutBoundInfo->_radius = D3DXVec3Length(&(pOutBoundInfo->_center - pOutBoundInfo->_min));
	}

	void StaticXMesh::SetCamera(const Camera & camera)
	{
		Matrix matViewProj = camera.GetViewProjectionMatrix();
		video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);

		pEffect->SetMatrix("matViewProjection", matViewProj);

		Vector3 vEyePos = camera.GetEntity().GetComponent<TransformComponent>().GetWorldPosition();

		HRESULT re = pEffect->_ptr->SetVector("vEyePos", &Vector4(vEyePos, 1));
		re = pEffect->_ptr->SetFloat("camFar", camera._camFar);
		re = pEffect->_ptr->SetFloat("camNear", camera._camNear);
	}

	void StaticXMesh::SetBaseLight(DirectionalLight * pDirectional)
	{
		video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);
		pEffect->SetMatrix("baseDirectionalLight", pDirectional->GetLightMatrix());
	}

	void StaticXMesh::SetTechnique(D3DXHANDLE name)
	{
		VIDEO->GetEffect(_sEffectHandle)->SetTechnique(name);
	}

	bool StaticXMesh::Create(const std::string & fileName, const Matrix * matCorrection)
	{
		DWORD dwMatNum{};

		LPD3DXBUFFER pAdjacency{};
		LPD3DXBUFFER pMaterial{};

		if (FAILED(D3DXLoadMeshFromX(fileName.c_str(), D3DXMESH_MANAGED, gpDevice,
			&pAdjacency, &pMaterial, NULL, &dwMatNum, &this->_pMesh)))
		{
			SAFE_RELEASE(pAdjacency);
			SAFE_RELEASE(pMaterial);
			return false;
		}
		_numMaterial = dwMatNum;
		//_materialHandles.reserve(_numMaterial + 1);
		//_submeshBoundInfos.reserve(_numMaterial + 1);

		//로딩 경로에서 파일명만 제거하고 경로만 받는다.
		std::string path;
		int lastPathIndex = 0;

		lastPathIndex = fileName.find_last_of('/');
		if (lastPathIndex == -1) 
		{			
			lastPathIndex = fileName.find_last_of('\\');	
		}
		//경로 구분이 있다면...
		if (lastPathIndex != -1) 
		{
			path = fileName.substr(0, lastPathIndex + 1);
		}

		//Texture 로딩시 필요한 문자열들..
		std::string texFilePath;	//최종 경로...
		std::string texExp;			//파일 확장자 명
		std::string texFile;		//파일 명	

		LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)pMaterial->GetBufferPointer();

		for (uint32 i = 0; i < this->_numMaterial; i++)
		{
			//로드된 메터리얼
			D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

			//메쉬의 메터리얼에는 Ambient 정보가 없으니
			//Diffuse 정보와 동일하게 하자..
			mtrl.Ambient = mtrl.Diffuse;

			//메터리얼 정보 푸쉬
			_materials.push_back(mtrl);

			//메터리얼의 Texture 정보가 있다면..
			if (pMaterials[i].pTextureFilename != nullptr)
			{
				//tex 파일경로는 Mesh 파일경로 + texture 파일이름
				texFilePath = path + pMaterials[i].pTextureFilename;

#pragma region Load Rock Texture
				if (strncmp(pMaterials[i].pTextureFilename, "Rocks", 4) == 0)
				{
					video::TextureHandle diffuseHandle = VIDEO->GetTexture(texFilePath);
					if (!diffuseHandle.IsValid())
					{
						_diffuseTextures.push_back(VIDEO->CreateTexture(texFilePath, texFilePath));
					}
					else
					{
						_diffuseTextures.push_back(diffuseHandle);
					}

					texFilePath = path + "RocksStonesAlbedo_N.png";
					video::TextureHandle normalHandle = VIDEO->GetTexture(texFilePath);
					if (!normalHandle.IsValid())
					{
						video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
						if (!loadedHandle.IsValid())
						{
							_normalTextures.push_back(VIDEO->GetTexture("normalDefault.png"));
						}
						else
						{
							_normalTextures.push_back(loadedHandle);
						}
					}
					else
					{
						_normalTextures.push_back(normalHandle);
					}

					texFilePath = path + "RocksStonesAlbedo_S.png";
					video::TextureHandle specularHandle = VIDEO->GetTexture(texFilePath);
					if (!specularHandle.IsValid())
					{
						video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
						if (!loadedHandle.IsValid())
						{
							_specularTextures.push_back(VIDEO->GetTexture("specularDefault.png"));
						}
						else
						{
							_specularTextures.push_back(loadedHandle);
						}
					}
					else
					{
						_specularTextures.push_back(specularHandle);
					}

					_emissionTexture.push_back(VIDEO->GetTexture("emissionDefault.png"));
				}
#pragma endregion
			else if (strncmp(pMaterials[i].pTextureFilename, "TreeTrunk", strlen("TreeTrunk")) == 0)
#pragma region Load TreeTrunk
			{
				video::TextureHandle diffuseHandle = VIDEO->GetTexture(texFilePath);
				if (!diffuseHandle.IsValid())
				{
					_diffuseTextures.push_back(VIDEO->CreateTexture(texFilePath, texFilePath));
				}
				else
				{
					_diffuseTextures.push_back(diffuseHandle);
				}

				texFilePath = path + "TreeTrunk_N.png";
				video::TextureHandle normalHandle = VIDEO->GetTexture(texFilePath);
				if (!normalHandle.IsValid())
				{
					video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
					if (!loadedHandle.IsValid())
					{
						_normalTextures.push_back(VIDEO->GetTexture("normalDefault.png"));
					}
					else
					{
						_normalTextures.push_back(loadedHandle);
					}
				}
				else
				{
					_normalTextures.push_back(normalHandle);
				}

				texFilePath = path + "TreeTrunk_S.png";
				video::TextureHandle specularHandle = VIDEO->GetTexture(texFilePath);
				if (!specularHandle.IsValid())
				{
					video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
					if (!loadedHandle.IsValid())
					{
						_specularTextures.push_back(VIDEO->GetTexture("specularDefault.png"));
					}
					else
					{
						_specularTextures.push_back(loadedHandle);
					}
				}
				else
				{
					_specularTextures.push_back(specularHandle);
				}

				_emissionTexture.push_back(VIDEO->GetTexture("emissionDefault.png"));
			}
#pragma endregion
			else if (strncmp(pMaterials[i].pTextureFilename, "Mush", strlen("Mush")) == 0)
#pragma region Load Mushroom
			{
				video::TextureHandle diffuseHandle = VIDEO->GetTexture(texFilePath);
				if (!diffuseHandle.IsValid())
				{
					_diffuseTextures.push_back(VIDEO->CreateTexture(texFilePath, texFilePath));
				}
				else
				{
					_diffuseTextures.push_back(diffuseHandle);
				}

				texFilePath = path + "Mushroom_N.png";
				video::TextureHandle normalHandle = VIDEO->GetTexture(texFilePath);
				if (!normalHandle.IsValid())
				{
					video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
					if (!loadedHandle.IsValid())
					{
						_normalTextures.push_back(VIDEO->GetTexture("normalDefault.png"));
					}
					else
					{
						_normalTextures.push_back(loadedHandle);
					}
				}
				else
				{
					_normalTextures.push_back(normalHandle);
				}

				texFilePath = path + "Mushroom_S.png";
				video::TextureHandle specularHandle = VIDEO->GetTexture(texFilePath);
				if (!specularHandle.IsValid())
				{
					video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
					if (!loadedHandle.IsValid())
					{
						_specularTextures.push_back(VIDEO->GetTexture("specularDefault.png"));
					}
					else
					{
						_specularTextures.push_back(loadedHandle);
					}
				}
				else
				{
					_specularTextures.push_back(specularHandle);
				}

				_emissionTexture.push_back(VIDEO->GetTexture("emissionDefault.png"));
			}
#pragma endregion

			else
			{
				//Texture 로딩하고 푸쉬
				video::TextureHandle diffuseHandle = VIDEO->GetTexture(texFilePath);
				if (!diffuseHandle.IsValid())
				{
					_diffuseTextures.push_back(VIDEO->CreateTexture(texFilePath, texFilePath));
				}

					//파일 이름과 확장자 위치
					int dotIndex = texFilePath.find_last_of(".");

					//파일 명과 확장자를 나눈다.
					texFile = texFilePath.substr(0, dotIndex);
					texExp = texFilePath.substr(dotIndex + 1, fileName.length());

					texFilePath = texFile + "_N." + texExp;

					video::TextureHandle normalHandle = VIDEO->GetTexture(texFilePath);
					if (!normalHandle.IsValid())
					{
						video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
						if (!loadedHandle.IsValid())
						{
							_normalTextures.push_back(VIDEO->GetTexture("normalDefault.png"));
						}
						else
						{
							_normalTextures.push_back(loadedHandle);
						}
					}

					texFilePath = texFile + "_S." + texExp;
					video::TextureHandle specularHandle = VIDEO->GetTexture(texFilePath);
					if (!specularHandle.IsValid())
					{
						video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
						if (!loadedHandle.IsValid())
						{
							_specularTextures.push_back(VIDEO->GetTexture("specularDefault.png"));
						}
						else
						{
							_specularTextures.push_back(loadedHandle);
						}
					}

					texFilePath = texFile + "_E." + texExp;
					video::TextureHandle emissionHandle = VIDEO->GetTexture(texFilePath);
					if (!emissionHandle.IsValid())
					{
						video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
						if (!loadedHandle.IsValid())
						{
							_emissionTexture.push_back(VIDEO->GetTexture("emissionDefault.png"));
						}
						else
						{
							_emissionTexture.push_back(loadedHandle);
						}
					}
				}
			}
			else
			{
				_diffuseTextures.push_back(VIDEO->GetTexture("diffuseDefault.png"));
				_normalTextures.push_back(VIDEO->GetTexture("normalDefault.png"));
				_specularTextures.push_back(VIDEO->GetTexture("specularDefault.png"));
				_emissionTexture.push_back(VIDEO->GetTexture("emissionDefault.png"));
			}
		}

		SAFE_RELEASE(pMaterial);

		_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
			(DWORD*)pAdjacency->GetBufferPointer(), nullptr, nullptr, nullptr);

		//attribute table은 Optimization을 할때 D3DXMESHOPT_ATTSORT를 함으로서 생성된다
		DWORD attributeTableSize{};
		_pMesh->GetAttributeTable(nullptr, &attributeTableSize);
		_attributeRange = new D3DXATTRIBUTERANGE[attributeTableSize];
		_pMesh->GetAttributeTable(_attributeRange, &attributeTableSize);

		D3DVERTEXELEMENT9 elements[64];
		_pMesh->GetDeclaration(elements);


		// 메쉬 보정 처리
		if (nullptr != matCorrection)
		{
			ResizeMeshAndGetInfos(_pMesh, *matCorrection, &_meshVertInfo, &_meshBoundInfo);
		}
		else
		{
			//보정행렬이 없더라도 보정처리를해야 Bound 정보를 얻을수 있다.
			Matrix matIden;
			MatrixIdentity(&matIden);
			ResizeMeshAndGetInfos(_pMesh, matIden, &_meshVertInfo, &_meshBoundInfo);
		}

		BuidSubMeshBoundInfo();
	}

	void StaticXMesh::Destroy()
	{
		COM_RELEASE(_pMesh);
		SAFE_DELETE_ARRAY(_attributeRange);

	}

	//TODO : Implement this
	void StaticXMesh::BuidSubMeshBoundInfo()
	{
		//for (uint32 i = 0; i < _numMaterial; ++i)
		//{
		//	const D3DXATTRIBUTERANGE &pAttributeRange = _attributeRange[i];
		//	_submeshBoundInfos.push_back(MeshBoundInfo());
		//	CalculateBoundingInfo(_vertices, &_submeshBoundInfos[i],
		//		pAttributeRange.VertexStart, pAttributeRange.VertexStart + pAttributeRange.VertexCount);
		//}
	}

	void StaticXMesh::Render(ARCHE_TYPE type, const TransformComponent & transform)
	{
		//월드행렬 셋팅
		video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);

		Matrix matWorld = transform.GetFinalMatrix();
		pEffect->SetMatrix("matWorld", matWorld);

		//Effect 로 그리기 시작
		//HRESULT result = pEffect->SetTechnique(ArcheToString(type));
		HRESULT  result;
		if (type == ARCHE_GRASS)
		{
			result = pEffect->SetTechnique("ReciveShadowFoliage");
		}
		else
		{
			result = pEffect->SetTechnique("ReciveShadow");
		}


		uint32 passNum = pEffect->BeginEffect();

		for (uint32 i = 0; i < passNum; i++)
		{
			pEffect->BeginPass(i);

			for (uint32 m = 0; m < this->_numMaterial; m++) 
			{
				////텍스쳐 셋팅
				pEffect->SetTexture("DiffuseTexture", *VIDEO->GetTexture(_diffuseTextures[m]));
				pEffect->SetTexture("NormalTexture", *VIDEO->GetTexture(_normalTextures[m]));
				pEffect->SetTexture("SpecularTexture", *VIDEO->GetTexture(_specularTextures[m]));
				pEffect->SetTexture("EmissionTexture", *VIDEO->GetTexture(_emissionTexture[m]));
				////스펙파워
				//pEffect->SetFloat("fSpecPower", this->_materials[m].Power);

				//Begin 이 들어오고 난후 값이 바뀌면 다음과 같이 실행
				pEffect->CommitChanges();

				_pMesh->DrawSubset(m);
			}
			pEffect->EndPass();
		}
		pEffect->EndEffect();
	}

	void StaticXMesh::RenderShadow(const TransformComponent & transform)
	{
		//월드행렬 셋팅
		video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);

		//Effect 로 그리기 시작
		HRESULT result = pEffect->SetTechnique("CreateShadow");
		pEffect->SetMatrix("matWorld", transform.GetFinalMatrix());
		pEffect->CommitChanges();

		uint32 passNum = pEffect->BeginEffect();

		for (uint32 i = 0; i < passNum; i++)
		{
			pEffect->BeginPass(i);

			for (uint32 m = 0; m < this->_numMaterial; m++) 
			{
				pEffect->SetTexture("DiffuseTexture", *VIDEO->GetTexture(_diffuseTextures[m]));
				pEffect->CommitChanges();
				_pMesh->DrawSubset(m);
			}
			pEffect->EndPass();
		}
		pEffect->EndEffect();
	}

	BoneMesh * SkinnedXMesh::FindBoneMesh(Bone *pBone, LPCSTR name)
	{
		if (nullptr == pBone)
		{
			pBone = _pRootBone;
		}

		if (pBone->pMeshContainer)
		{
			BoneMesh *pBoneMesh = (BoneMesh *)pBone->pMeshContainer;
			if (strcmp(pBoneMesh->Name, name) == 0)
			{
				return pBoneMesh;
			}

			if (nullptr != pBoneMesh->pNextMeshContainer)
			{
				while (pBoneMesh)
				{
					pBoneMesh = (BoneMesh *)pBoneMesh->pNextMeshContainer;
					if (strcmp(pBoneMesh->Name, name) == 0)
					{
						return pBoneMesh;
					}
				}
			}

		}

		if (nullptr != pBone->pFrameSibling)
		{
			return FindBoneMesh((Bone *)pBone->pFrameSibling, name);
		}

		if (nullptr != pBone->pFrameFirstChild)
		{
			return FindBoneMesh((Bone *)pBone->pFrameFirstChild, name);
		}

		return nullptr;
	}

	void SkinnedXMesh::SetCamera(const Camera & camera)
	{
		video::Effect *pSkinned = VIDEO->GetEffect(_sEffectHandle);

		pSkinned->SetMatrix("matViewProjection", camera.GetViewProjectionMatrix());

		Vector3 vEyePos = camera.GetEntity().GetComponent<TransformComponent>().GetWorldPosition();
		pSkinned->_ptr->SetVector("vEyePos", &Vector4(vEyePos, 1));
		pSkinned->_ptr->SetFloat("camFar", camera._camFar);
		pSkinned->_ptr->SetFloat("camNear", camera._camNear);
	}

	void SkinnedXMesh::SetBaseLight(DirectionalLight * pDirectional)
	{
		video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);

		pEffect->SetMatrix("baseDirectionalLight", pDirectional->GetLightMatrix());
	}

	void SkinnedXMesh::SetTechnique(D3DXHANDLE name)
	{
		VIDEO->GetEffect(_sEffectHandle)->SetTechnique(name);
	}

	//Skinned XMesh ////////////////////////////////////////////////////////
	bool SkinnedXMesh::Create(const std::string & fileName, const Matrix * matCorrection)
	{
		if (nullptr != matCorrection)
		{
			_matCorrection = *matCorrection;
		}
		else
		{
			MatrixIdentity(&_matCorrection);
		}

		BoneHierachy boneHierachy;
		std::string path;
		int lastPathIndex = 0;		//마지막 \ 의 위치

		lastPathIndex = fileName.find_last_of('/');		//뒤에서 부터 검색
		if (lastPathIndex == -1)
		{
			lastPathIndex = fileName.find_last_of('\\');
		}
		//경로 구분이 있다면...
		if (lastPathIndex != -1)
		{
			path = fileName.substr(0, lastPathIndex + 1);
		}

		boneHierachy.SetFilePath(path);
		boneHierachy.SetSkinnedMesh(this);

		if (FAILED(D3DXLoadMeshHierarchyFromX(fileName.c_str(), D3DXMESH_MANAGED, gpDevice,
			&boneHierachy, nullptr, (D3DXFRAME **)&_pRootBone, &_pAnimationController)))
		{
			Console::Log("Load Mesh HierarchyFromX failed\n");
			return false;
		}
		//본 매트릭스 포인터 생성
		InitBoneMatrixPointer((Bone*)_pRootBone);

		UpdateMatrices(_pRootBone, &_matCorrection);

		CalculateTotalBoundInfo(_pRootBone);
	}

	void SkinnedXMesh::Destroy()
	{
		//for (uint32 i = 0; i < _animations.size(); ++i)
		//{
		//	COM_RELEASE(_animations[i]);
		//}
		//for (auto animSet : _animationTable)
		//{
		//	COM_RELEASE(animSet.second);
		//}

		COM_RELEASE(_pAnimationController);

		BoneHierachy hierarchy;
		D3DXFrameDestroy(_pRootBone, &hierarchy);
	}

	void SkinnedXMesh::InitBoneMatrixPointer(Bone * pBone)
	{
		if (nullptr == pBone)
		{
			return;
		}

		if (pBone->pMeshContainer)
		{
			//메쉬 컨테이너를 BONE_MESH 형으로 형변환
			BoneMesh* pBoneMesh = (BoneMesh*)pBone->pMeshContainer;

			//메쉬에 스키닝 정보가 있다면
			if (pBoneMesh->pSkinInfo)
			{
				int numBones = pBoneMesh->pSkinInfo->GetNumBones();
				pBoneMesh->ppBoneMatrixPtrs = new Matrix*[numBones];
				for (int32 i = 0; i < numBones; i++)
				{
					//해당 스킨에 적용되는 i 번째의 본이름으로 해당 본 프레임을 루프 본부터 찾아 들어간가
					Bone* pB = (Bone*)D3DXFrameFind((D3DXFRAME *)_pRootBone, pBoneMesh->pSkinInfo->GetBoneName(i));
					if (pB != NULL)
					{
						pBoneMesh->ppBoneMatrixPtrs[i] = &pB->CombinedTransformationMatrix;
					}
					else
					{
						pBoneMesh->ppBoneMatrixPtrs[i] = NULL;
					}
				}
			}
		}
		if (pBone->pFrameSibling)
		{
			InitBoneMatrixPointer((Bone*)pBone->pFrameSibling);
		}
		if (pBone->pFrameFirstChild)
		{
			InitBoneMatrixPointer((Bone*)pBone->pFrameFirstChild);
		}
	}

	void SkinnedXMesh::Update(const Matrix * pWorld)
	{
		Matrix finalWorld;
		if (nullptr != pWorld)
		{
			finalWorld = _matCorrection * (*pWorld);
		}
		else
		{
			finalWorld = _matCorrection;
		}

		UpdateMatrices(_pRootBone, &finalWorld);
	}

	void SkinnedXMesh::UpdateMatrices(Bone * pBone, Matrix *pParentMatrix) const
	{
		if (nullptr == pBone)
		{
			return;
		}

		//부모 행렬이 있다면..
		if (nullptr != pParentMatrix)
		{
			Matrix matTransformation = pBone->TransformationMatrix;

			//pBone 의 행렬 부모행렬을 곱하여 pBone 의 최종 행렬을 구한다.
			MatrixMultiply(&pBone->CombinedTransformationMatrix, &matTransformation, pParentMatrix);
		}
		else
		{
			//자신의 행렬이 현재 행렬이 된다.
			pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
		}

		if (pBone->pFrameSibling)
		{
			UpdateMatrices((Bone*)pBone->pFrameSibling, pParentMatrix);
		}
		if (pBone->pFrameFirstChild)
		{
			UpdateMatrices((Bone*)pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);
		}
	}

	void SkinnedXMesh::Render(const TransformComponent & transform)
	{
		Matrix matFinal;

		//Transform 이 들어왔다면...
		Matrix matWorld = transform.GetFinalMatrix();
		matFinal = _matCorrection * matWorld;

		//행렬 업데이트 
		Update(&transform.GetFinalMatrix());

		//루트 본부터 타고 들어간다.
		RenderBone((Bone*)_pRootBone);
	}

	void SkinnedXMesh::RenderShadow()
	{
		RenderShadowInternal(_pRootBone);
	}


	void SkinnedXMesh::RenderBone(Bone * pBone)
	{
		if (nullptr == pBone)
		{
			return;
		}

		if (pBone->pMeshContainer)
		{
			//본에 있는 메쉬 컨테이너는 BONE_MESH 이다
			BoneMesh* pBoneMesh = (BoneMesh*)pBone->pMeshContainer;
			if (pBoneMesh->_visible)
			{
				//본 컴비네이션 정보가  
				if (nullptr != pBoneMesh->BufBoneCombos)
				{
					video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);
					pEffect->SetTechnique("RecieveShadowSkinned");

					LPD3DXBONECOMBINATION pBoneComb =
						(LPD3DXBONECOMBINATION)(pBoneMesh->BufBoneCombos->GetBufferPointer());

					for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
					{
						for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
						{
							DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

							if (dwMatrixIndex != UINT_MAX)
							{
								D3DXMatrixMultiply(&_workingPalettes[palEntry],
									&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
									pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
							}
						}
						pEffect->SetMatrices("amPalette", _workingPalettes, pBoneMesh->NumPaletteEntries);

						pEffect->SetInt("CurNumBones", pBoneMesh->MaxNumFaceInfls - 1);

						DWORD materialIndex = pBoneComb[i].AttribId;

						pEffect->SetTexture("DiffuseTexture", *VIDEO->GetTexture(pBoneMesh->_diffuseTextures[materialIndex]));
						pEffect->SetTexture("SpecularTexture", *VIDEO->GetTexture(pBoneMesh->_specularTextures[materialIndex]));
						pEffect->SetTexture("NormalTexture", *VIDEO->GetTexture(pBoneMesh->_normalTextures[materialIndex]));
						pEffect->SetTexture("EmissionTexture", *VIDEO->GetTexture(pBoneMesh->_emissionTexture[materialIndex]));
						pEffect->SetFloat("fSpecPower", pBoneMesh->_materials[materialIndex].Power);

						pEffect->CommitChanges();

						uint32 numPass = pEffect->BeginEffect();
						for (uint32 p = 0; p < numPass; ++p)
						{
							pEffect->BeginPass(p);
							pBoneMesh->WorkingMesh->DrawSubset(i);
							pEffect->EndPass();
						}
						pEffect->EndEffect();
					}
				}
				else
				{
					video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);
					pEffect->SetTechnique("RecieveShadowStaitc");
					pEffect->SetMatrix("matWorld", pBone->CombinedTransformationMatrix);
					for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
					{
						pEffect->SetTexture("DiffuseTexture", *VIDEO->GetTexture(pBoneMesh->_diffuseTextures[i]));
						pEffect->SetTexture("SpecularTexture", *VIDEO->GetTexture(pBoneMesh->_specularTextures[i]));
						pEffect->SetTexture("NormalTexture", *VIDEO->GetTexture(pBoneMesh->_normalTextures[i]));
						pEffect->SetTexture("EmissionTexture", *VIDEO->GetTexture(pBoneMesh->_emissionTexture[i]));
						pEffect->SetFloat("fSpecPower", pBoneMesh->_materials[i].Power);

						pEffect->CommitChanges();

						uint32 numPass = pEffect->BeginEffect();
						for (uint32 p = 0; p < numPass; ++p)
						{
							pEffect->BeginPass(p);
							pBoneMesh->MeshData.pMesh->DrawSubset(i);
							pEffect->EndPass();
						}
						pEffect->EndEffect();
					}
				}
			}
		}

		if (pBone->pFrameSibling)
		{
			RenderBone((Bone*)pBone->pFrameSibling);
		}
		if (pBone->pFrameFirstChild)
		{
			RenderBone((Bone*)pBone->pFrameFirstChild);
		}
	}

	void SkinnedXMesh::RenderShadowInternal(Bone * pBone)
	{
		if (nullptr == pBone)
		{
			return;
		}

		if (pBone->pMeshContainer)
		{
			//본에 있는 메쉬 컨테이너는 BONE_MESH 이다
			BoneMesh* pBoneMesh = (BoneMesh*)pBone->pMeshContainer;

			//본 컴비네이션 정보가  
			if (nullptr != pBoneMesh->BufBoneCombos)
			{
				video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);
				pEffect->SetTechnique("CreateShadowSkinned");

				LPD3DXBONECOMBINATION pBoneComb =
					(LPD3DXBONECOMBINATION)(pBoneMesh->BufBoneCombos->GetBufferPointer());

				for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
				{
					for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
					{
						DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

						if (dwMatrixIndex != UINT_MAX)
						{
							D3DXMatrixMultiply(&_workingPalettes[palEntry],
								&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
								pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
						}
					}
					pEffect->SetMatrices( "amPalette", _workingPalettes, pBoneMesh->NumPaletteEntries);

					pEffect->SetInt("CurNumBones", pBoneMesh->MaxNumFaceInfls - 1);

					DWORD materialIndex = pBoneComb[i].AttribId;

					pEffect->SetTexture("DiffuseTexture", *VIDEO->GetTexture(pBoneMesh->_diffuseTextures[materialIndex]));

					uint32 numPass = pEffect->BeginEffect();
					for (uint32 p = 0; p < numPass; ++p)
					{
						pEffect->BeginPass(p);
						pBoneMesh->WorkingMesh->DrawSubset(i);
						pEffect->EndPass();
					}
					pEffect->EndEffect();
				}
			}
			else
			{
				video::Effect *pEffect = VIDEO->GetEffect(_sEffectHandle);
				HRESULT re = pEffect->SetTechnique("CreateShadowStatic");
				pEffect->SetMatrix("matWorld", pBone->CombinedTransformationMatrix);
				for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
				{
					pEffect->SetTexture("DiffuseTexture", *VIDEO->GetTexture(pBoneMesh->_diffuseTextures[i]));
					uint32 numPass = pEffect->BeginEffect();
					for (uint32 p = 0; p < numPass; ++p)
					{
						pEffect->BeginPass(p);
						pBoneMesh->MeshData.pMesh->DrawSubset(i);
						pEffect->EndPass();
					}
					pEffect->EndEffect();
				}
			}
		}

		if (pBone->pFrameSibling)
		{
			RenderShadowInternal((Bone*)pBone->pFrameSibling);
		}
		if (pBone->pFrameFirstChild)
		{
			RenderShadowInternal((Bone*)pBone->pFrameFirstChild);
		}

	}

	void SkinnedXMesh::CalculateTotalBoundInfo(Bone *pBone)
	{
		if (nullptr == pBone)
		{
			return;
		}

		if (pBone->pMeshContainer)
		{
			BoneMesh *pMesh = (BoneMesh *)pBone->pMeshContainer;

			Vec3TransformCoord(&pMesh->_boundInfo._min, 
				&pMesh->_boundInfo._min, 
				&pBone->CombinedTransformationMatrix);

			Vec3TransformCoord(&pMesh->_boundInfo._max, 
				&pMesh->_boundInfo._max, 
				&pBone->CombinedTransformationMatrix);

			//정점 최소 값갱신
			if (_boundInfo._min.x > pMesh->_boundInfo._min.x)		_boundInfo._min.x = pMesh->_boundInfo._min.x;
			if (_boundInfo._min.y > pMesh->_boundInfo._min.y)		_boundInfo._min.y = pMesh->_boundInfo._min.y;
			if (_boundInfo._min.z > pMesh->_boundInfo._min.z)		_boundInfo._min.z = pMesh->_boundInfo._min.z;

			//정점 최대 값갱신
			if (_boundInfo._max.x < pMesh->_boundInfo._max.x)		_boundInfo._max.x = pMesh->_boundInfo._max.x;
			if (_boundInfo._max.y < pMesh->_boundInfo._max.y)		_boundInfo._max.y = pMesh->_boundInfo._max.y;
			if (_boundInfo._max.z < pMesh->_boundInfo._max.z)		_boundInfo._max.z = pMesh->_boundInfo._max.z;

			//Bound 추가 계산
			_boundInfo._center = (_boundInfo._min + _boundInfo._max) * 0.5f;

			_boundInfo._size = Vector3(_boundInfo._max.x - _boundInfo._min.x, 
				_boundInfo._max.y - _boundInfo._min.y, 
				_boundInfo._max.z - _boundInfo._min.z);

			_boundInfo._halfSize = _boundInfo._size * 0.5f;
			_boundInfo._radius = D3DXVec3Length(&(_boundInfo._center - _boundInfo._min));
		}

		if (pBone->pFrameSibling)
		{
			CalculateTotalBoundInfo((Bone *)pBone->pFrameSibling);
		}
		if (pBone->pFrameFirstChild)
		{
			CalculateTotalBoundInfo((Bone *)pBone->pFrameFirstChild);
		}
	}
	

	//Skinned Animation ////////////////////////////////////////////////////////
	bool AnimationInstance::Create(video::SkinnedXMeshHandle handle)
	{
		if (!handle.IsValid())
		{
			return false;
		}

		_skinnedMeshHandle = handle;
		_pSkinnedMesh = VIDEO->GetSkinnedXMesh(handle);
		Assert(_pSkinnedMesh);

		//SKinned Mesh 에 Animation 를 복사한다.
		_pSkinnedMesh->_pAnimationController->CloneAnimationController(
			_pSkinnedMesh->_pAnimationController->GetMaxNumAnimationOutputs(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumAnimationSets(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumTracks(),
			_pSkinnedMesh->_pAnimationController->GetMaxNumEvents(), &_pAnimationController);

		return true;
	}

	void AnimationInstance::Destroy()
	{
		COM_RELEASE(_pAnimationController);
	}

}

void BoneHierachy::SetSkinnedMesh(video::SkinnedXMesh *pSkinnedMesh)
{
	_pSkinnedMesh = pSkinnedMesh;
}

STDMETHODIMP BoneHierachy::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	//ppNewFrame 으로 넘겨줄 새로운 Bone 생성
	Bone *newBone = new Bone;
	ZeroMemory(newBone, sizeof(Bone));
	//본이름 복사
	if (nullptr != Name)
	{
		int len = strlen(Name);
		newBone->Name = new char[len + 1];
		strcpy_s(newBone->Name, len + 1, Name);
	}
	//else
	//{
	//	int len = strlen(Name);
	//	newBone->Name = new char[len + 1];
	//	strcpy_s(newBone->Name, len + 1, Name);
	//}
	//매트릭스 정보 초기화
	D3DXMatrixIdentity(&newBone->TransformationMatrix);
	MatrixIdentity(&newBone->CombinedTransformationMatrix);

	//리턴값에 새로운 본 주소 대입
	*ppNewFrame = newBone;
	return S_OK;
}

STDMETHODIMP BoneHierachy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, 
	CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstance, 
	DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	//ppNewMeshContainer 로 넘겨줄 새로운 BONEMESH 생성
	BoneMesh* boneMesh = new BoneMesh;
	ZeroMemory(boneMesh, sizeof(BoneMesh));
	boneMesh->_visible = true;

	if (nullptr != Name)
	{
		int len = strlen(Name);
		boneMesh->Name = new char[len + 1];
		strcpy_s(boneMesh->Name, len + 1, Name);
	}

	//매개변수로 받은 메쉬 데이터의 Device 를 얻는다.
	LPDIRECT3DDEVICE9 d3dDevice = NULL;
	pMeshData->pMesh->GetDevice(&d3dDevice);

	if (strcmp(Name, "hair") == 0 ||
		strcmp(Name, "_1H_sword") == 0 ||
		strcmp(Name, "helmet_metal") == 0 )
	{
		boneMesh->_visible = false;
	}

	//if ( strcmp(Name, "_1H_sword") == 0 )
	//{
	//	boneMesh->_visible = false;
	//}

	//메시데이터를 물린다.
	boneMesh->MeshData.pMesh = pMeshData->pMesh;
	boneMesh->MeshData.Type = D3DXMESHTYPE_MESH;

	//메시데이터의 레퍼런스 카운트 증가
	pMeshData->pMesh->AddRef();
	//레퍼런스 카운트를 명시적으로 해주지 않는다면 매개변수로 받은 pMeshData 가 함수 호출후 해제 되어 버린다!
	//즉 함수 호출후에도 내부적으로 참조수가 0 이면 자동으로 해제되는 식이다.
	//따라서 새로만들어진 BONEMESH 맴버데이터에 pMeshData 포인터를 물고있으니
	//레퍼런스 카운트 수를 증가 시켜 함수가 끈난뒤에도 해제를 막는다.
	if (nullptr != Name)
	{
		int len = strlen(Name);
		boneMesh->Name = new char[len + 1];
		strcpy_s(boneMesh->Name, len + 1, Name);
	}

	//인접정보 복사 ( 인접 정보는 면단 3개씩 지니게 된다 왜냐면 폴리곤의 인접수는 3개니깐 )
	DWORD numAdjacency = 3 * (boneMesh->MeshData.pMesh->GetNumFaces());		//인접정보의 수
	boneMesh->pAdjacency = new DWORD[numAdjacency];							//인접 정보의 수만큼 증가
	memcpy(boneMesh->pAdjacency, pAdjacency, sizeof(DWORD) * numAdjacency);

	std::string texFilePath;
	std::string texExp;			//파일 확장자 명
	std::string texFile;		//파일 명`

	std::string texPath;

	//여기서 Material정보 가져오자..
	boneMesh->NumMaterials = NumMaterials;
	boneMesh->pMaterials = new D3DXMATERIAL[NumMaterials];
	for (uint32 i = 0; i < NumMaterials; i++)
	{
		//로드된 메터리얼
		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//메쉬의 메터리얼에는 Ambient 정보가 없으니
		//Diffuse 정보와 동일하게 하자..
		mtrl.Ambient = mtrl.Diffuse;

		//메터리얼 정보 푸쉬
		boneMesh->_materials.push_back(mtrl);

		//메터리얼의 Texture 정보가 있다면..
		if (pMaterials[i].pTextureFilename != nullptr)
		{
			//이름 깊은 복사
			int len = strlen(pMaterials[i].pTextureFilename);
			boneMesh->pMaterials[i].pTextureFilename = new char[len + 1];
			strcpy_s(boneMesh->pMaterials[i].pTextureFilename, len + 1, pMaterials[i].pTextureFilename);

			//tex 파일경로는 Mesh 파일경로 + texture 파일이름
			texFilePath = this->_filePath+ pMaterials[i].pTextureFilename;
			//Texture 로딩하고 푸쉬
			video::TextureHandle diffuseHandle = VIDEO->GetTexture(texFilePath);
			if (!diffuseHandle.IsValid())
			{
				video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
				if (!loadedHandle.IsValid())
				{
					boneMesh->_diffuseTextures.push_back(VIDEO->GetTexture("diffuseDefault.png"));
				}
				else
				{
					boneMesh->_diffuseTextures.push_back(loadedHandle);
				}
			}
			else
			{
				boneMesh->_diffuseTextures.push_back(diffuseHandle);
			}

			//파일 이름과 확장자 위치
			int dotIndex = texFilePath.find_last_of(".");

			//파일 명과 확장자를 나눈다.
			texFile = texFilePath.substr(0, dotIndex);
			texExp = texFilePath.substr(dotIndex + 1, texFilePath.length());

			texFilePath = texFile + "_N." + texExp;

			video::TextureHandle normalHandle = VIDEO->GetTexture(texFilePath);
			if (!normalHandle.IsValid())
			{
				video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
				if (!loadedHandle.IsValid())
				{
					boneMesh->_normalTextures.push_back(VIDEO->GetTexture("normalDefault.png"));
				}
				else
				{
					boneMesh->_normalTextures.push_back(loadedHandle);
				}
			}
			else
			{
				boneMesh->_normalTextures.push_back(normalHandle);
			}

			texFilePath = texFile + "_S." + texExp;
			video::TextureHandle specularHandle = VIDEO->GetTexture(texFilePath);
			if (!specularHandle.IsValid())
			{
				video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
				if (!loadedHandle.IsValid())
				{
					boneMesh->_specularTextures.push_back(VIDEO->GetTexture("specularDefault.png"));
				}
				else
				{
					boneMesh->_specularTextures.push_back(loadedHandle);
				}
			}
			else
			{
				boneMesh->_specularTextures.push_back(specularHandle);
			}

			texFilePath = texFile + "_E." + texExp;
			video::TextureHandle emissionHandle = VIDEO->GetTexture(texFilePath);
			if (!emissionHandle.IsValid())
			{
				video::TextureHandle loadedHandle = VIDEO->CreateTexture(texFilePath, texFilePath);
				if (!loadedHandle.IsValid())
				{
					boneMesh->_emissionTexture.push_back(VIDEO->GetTexture("emissionDefault.png"));
				}
				else
				{
					boneMesh->_emissionTexture.push_back(loadedHandle);
				}
			}
			else
			{
				boneMesh->_emissionTexture.push_back(emissionHandle);
			}

		}
		else
		{
			boneMesh->_diffuseTextures.push_back(VIDEO->GetTexture("diffuseDefault.png"));
			boneMesh->_normalTextures.push_back(VIDEO->GetTexture("normalDefault.png"));
			boneMesh->_specularTextures.push_back(VIDEO->GetTexture("specularDefault.png"));
			boneMesh->_emissionTexture.push_back(VIDEO->GetTexture("emissionDefault.png"));
		}
	}

	//속성테이블을 얻는다.
	boneMesh->MeshData.pMesh->GetAttributeTable(NULL, &boneMesh->NumAttributesGroup);

	//메쉬의 바운드 정보를 얻는다
	Matrix matIden;
	MatrixIdentity(&matIden);
	video::CalculateMeshBoundInfo(boneMesh->MeshData.pMesh, matIden, &boneMesh->_boundInfo);

	//SkinInfo가 있다면, 여기서 WorkingMesh로 인덱스 버퍼를 얻고 아니라면 MeshData안에 있는 메쉬로 버퍼를 가져온다.
	if (nullptr != pSkinInfo)
	{
		boneMesh->pSkinInfo = pSkinInfo;

		pSkinInfo->AddRef();

		int32 numBone = pSkinInfo->GetNumBones();

		//사용되는 본의 수만큼 행렬 동적할당
		//Inverse Bind Pose Matrix
		boneMesh->pBoneOffsetMatices = new Matrix[numBone];
		//본 Offset 행렬 셋팅
		for (int32 i = 0; i < numBone; i++)
		{
			boneMesh->pBoneOffsetMatices[i] = *(boneMesh->pSkinInfo->GetBoneOffsetMatrix(i));
		}

		////Effect 에서 사용되는 행렬 팔래트 쵀대 수를 얻는다.
		//uint ipalettesize = 0;
		//_pSkinnedMesh->m_pSkinnedEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&iPaletteSize);

		//기본적으로 적용되는 본수 만큼 팔래트를 할당하는데 Effect 의 최대 MATRIX_PALETTE_SIZE 보다 적용되는 본의 
		//수가 많다면 팔래트 수는 Effect MATRIX_PALETTE_SIZE 수로 고정 한다.
		boneMesh->NumPaletteEntries = boneMesh->pSkinInfo->GetNumBones();

		if (boneMesh->NumPaletteEntries > DEFAULT_MATRIX_PALETTE_SIZE)
		{
			boneMesh->NumPaletteEntries = DEFAULT_MATRIX_PALETTE_SIZE;
		}

		//위의 작업에이어 Mesh에  적용되는 bone의 수가 Effect 에서 지원되는 MATRIX_PALETTE_SIZE 수보다 많은 경우
		//boneMesh->NumPaletteEntries 수에 맞추어서 Mesh 의 SubSet 을 나눌 필요가 있다.
		//아래의 함수는 그러한 작업 들을 WorkingMesh 에 해주고 또한 WorkingMesh 에 
		//정점 가중치 개념을 추가해준다.

		boneMesh->pSkinInfo->ConvertToIndexedBlendedMesh(
			boneMesh->MeshData.pMesh,
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE | D3DXMESHOPT_ATTRSORT,
			boneMesh->NumPaletteEntries,
			boneMesh->pAdjacency,
			NULL,							//갱신된 인접 버퍼를 받아온다 ( 필요없으면 NULL )
			NULL,							//갱신된 폴리곤(감각인덱스) 버퍼를 받아온다 ( 필요없으면 NULL )
			NULL,							//갱신된 정점 버퍼를 받아온다 ( 필요없으면 NULL )
			&boneMesh->MaxNumFaceInfls,		//한 버텍스에 영향을 주는 본의 최대 갯수를 얻는더.
			&boneMesh->NumAttributesGroup,	//본의 컴비네이션 속성그룹의 수를 얻는다ㅓ.
			&boneMesh->BufBoneCombos,		//본의 컴비네이션 버퍼를 얻는다.
			&boneMesh->WorkingMesh );

		//본의 컴비네이션 은 메쉬의 Subset 별로 어떠한 본에 영향을 받는지에 대한 정보를 지니게 된다.
		//본매트릭스 인덱스 = 본컴비네이션[Subset].BoneID[팔래트]
		if ( _pSkinnedMesh->_numWorkingPalette < boneMesh->NumPaletteEntries )
		{
			if ( _pSkinnedMesh->_numWorkingPalette )
				delete[] _pSkinnedMesh->_workingPalettes;

			_pSkinnedMesh->_numWorkingPalette = boneMesh->NumPaletteEntries;
			_pSkinnedMesh->_workingPalettes = new Matrix[boneMesh->NumPaletteEntries];
		}
	}
	*ppNewMeshContainer = boneMesh;

	
	SAFE_RELEASE(d3dDevice);
	return S_OK;
}

STDMETHODIMP BoneHierachy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree)
	{
		//Transform 작살냄
		//SAFE_DELETE( ( (Bone*)pFrameToFree )->pBoneTransform );

		//이름 해제
		SAFE_DELETE_ARRAY(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);
	}

	return S_OK;
}

STDMETHODIMP BoneHierachy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	//boneMesh 로 형변환
	BoneMesh* boneMesh = (BoneMesh*)pMeshContainerToFree;

	//텍스쳐 해제
	//해제 코드도 작성하자...
	//UINT numTex = boneMesh->_materialHandles.size();
	//for (UINT i = 0; i < numTex; i++)
	//{
	//	//if (boneMesh->pMaterials[i].pTextureFilename != NULL)
	//	//{
	//	//	SAFE_DELETE_ARRAY(boneMesh->pMaterials[i].pTextureFilename);
	//	//}
	//}
	SAFE_DELETE_ARRAY(boneMesh->Name);
	SAFE_DELETE_ARRAY(boneMesh->pMaterials);
	SAFE_DELETE_ARRAY(boneMesh->pAdjacency);
	SAFE_DELETE_ARRAY(boneMesh->_attributeRange);

	SAFE_RELEASE(boneMesh->MeshData.pMesh);
	SAFE_RELEASE(boneMesh->WorkingMesh);
	SAFE_RELEASE(boneMesh->pSkinInfo);

	SAFE_DELETE_ARRAY(boneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(boneMesh->pBoneOffsetMatices);
	SAFE_RELEASE(boneMesh->BufBoneCombos);

	SAFE_DELETE(boneMesh);

	return S_OK;
}