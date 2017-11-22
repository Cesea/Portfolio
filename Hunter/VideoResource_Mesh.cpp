﻿#include "stdafx.h"
#include "VideoResource_Mesh.h"

namespace video
{
	EffectHandle StaticXMesh::sDefaultEffectHandle;
	EffectHandle AnimationInstance::sDefaultEffectHandle;

	////Free Function
	TextureHandle LoadTextureWithStrings(const std::string &filePath, const std::string &fileName, const std::string &fileExtension, const std::string &userStr)
	{
		TextureHandle resultHandle;
		//텍스쳐를 로드하는데 텍스쳐가 이미 있다면 그것을 반환하고
		resultHandle = VIDEO->GetTexture(fileName + userStr + fileExtension);
		if (!resultHandle.IsValid())
		{
			//만약 없다면 새로 로드한다
			resultHandle = VIDEO->CreateTexture(filePath + fileName + userStr + fileExtension,
				fileName + userStr + fileExtension);
			//새로 로드한 텍스쳐가 유효하지 않다면
			if (!resultHandle.IsValid())
			{
				//기본 흰 텍스쳐를 불러온다
				if (userStr == ".")
				{
					resultHandle = VIDEO->GetTexture("diffuseDefault.png");
				}
				else if (userStr == "_N.")
				{
					resultHandle = VIDEO->GetTexture("normalDefault.png");
				}
				else if (userStr == "_S.")
				{
					resultHandle = VIDEO->GetTexture("specularDefault.png");
				}
				else if (userStr == "_E.")
				{
					resultHandle = VIDEO->GetTexture("emissionDefault.png");
				}
			}
		}
		return resultHandle;
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
		_materialHandles.reserve(_numMaterial + 1);
		_submeshBoundInfos.reserve(_numMaterial + 1);



		std::string stringCopy = fileName;
		std::string name;
		std::string path;
		std::string extension;

		SplitFilePathToNamePathExtension(stringCopy, name, path, extension);

		std::string textureExtension;		//파일 확장자 명
		std::string textureFileName;		//파일 명	

		LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)pMaterial->GetBufferPointer();

		for (uint32 i = 0; i < this->_numMaterial; i++)
		{
			D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

			//메터리얼의 Texture 정보가 있다면..
			if (nullptr != pMaterials[i].pTextureFilename)
			{
				//tex 파일경로는 Mesh 파일경로 + texture 파일이름
				std::string textureString = pMaterials[i].pTextureFilename;
				SplitNameToNameExtension(textureString, textureFileName, textureExtension);

				TextureHandle diffuse = LoadTextureWithStrings(path, textureFileName, textureExtension, ".");
				TextureHandle normal = LoadTextureWithStrings(path, textureFileName, textureExtension, "_N.");
				TextureHandle specular = LoadTextureWithStrings(path, textureFileName, textureExtension, "_S.");
				TextureHandle emission = LoadTextureWithStrings(path, textureFileName, textureExtension, "_E.");

				MaterialHandle material = VIDEO->CreateMaterial(textureFileName);
				VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE_DIFFUSE, diffuse);
				VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE_NORMAL, normal);
				VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE_SPECULAR, specular);
				VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE_EMISSIVE, emission);

				_materialHandles.push_back(material);
			}
			else
			{
				MaterialHandle material = VIDEO->GetMaterial("Default");
				_materialHandles.push_back(material);
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

		_vHandle = VIDEO->GetVertexBufferFromXMesh(_pMesh);
		_iHandle = VIDEO->GetIndexBufferFromXMesh(_pMesh);

		BuidSubMeshBoundInfo();
	}

	void StaticXMesh::Destroy()
	{
		COM_RELEASE(_pMesh);
		SAFE_DELETE_ARRAY(_attributeRange);

		VIDEO->DestroyVertexBuffer(_vHandle);
		VIDEO->DestroyIndexBuffer(_iHandle);
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

	void StaticXMesh::FillRenderCommand(RenderView &renderView, video::EffectHandle effect, const Matrix *pMatrix)
	{
		MatrixCache::CacheRange range;
		if (nullptr != pMatrix)
		{
			range = renderView._matrixCache.Add(pMatrix);

		}

		for (uint32 i = 0; i < _numMaterial; ++i)
		{
			RenderCommand &command = renderView.GetCommand();
			command._drawType = RenderCommand::DrawType::eStatic;

			command._vHandle = _vHandle;
			command._iHandle = _iHandle;
			command._cacheRange = range;

			command._startIndex = _attributeRange[i].FaceStart * 3;
			command._numVertices = _attributeRange[i].VertexCount;
			command._numPrim = _attributeRange[i].FaceCount;

			command._primType = RenderCommand::PrimType::eTriangleList;
			command._materialHandle = _materialHandles[i];
			command._effectHandle = effect;
		}
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
		boneHierachy.SetFilePath(fileName);
		boneHierachy.SetSkinnedMesh(this);

		if (FAILED(D3DXLoadMeshHierarchyFromX(fileName.c_str(), D3DXMESH_MANAGED, gpDevice,
			&boneHierachy, nullptr, (D3DXFRAME **)&_pRootBone, &_pAnimationController)))
		{
			Console::Log("Load Mesh HierarchyFromX failed\n");
			return false;
		}
		//본 매트릭스 포인터 생성
		InitBoneMatrixPointer((Bone*)_pRootBone);

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

	//Skinned Animation ////////////////////////////////////////////////////////
	bool AnimationInstance::Create(video::SkinnedXMeshHandle handle)
	{
		if (!handle.IsValid())
		{
			return false;
		}

		_pSkinnedMesh = VIDEO->GetSkinnedXMesh(handle);
		Assert(_pSkinnedMesh);

		_numPalette = _pSkinnedMesh->_numWorkingPalette;
		Assert(_numPalette);
		_workingPalettes = new Matrix[_numPalette];

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

	void AnimationInstance::FillRenderCommand(RenderView & renderView, 
		video::EffectHandle skinnedEffect, video::EffectHandle staticEffect)
	{
		FillRenderCommandInternal(renderView, skinnedEffect, staticEffect, _pSkinnedMesh->_pRootBone);
	}

	void AnimationInstance::FillRenderCommandInternal(RenderView & renderView, 
		video::EffectHandle skinnedEffect, video::EffectHandle staticEffect, Bone *pBone)
	{
		if (nullptr == pBone)
		{
			return;
		}

		if (pBone->pMeshContainer)
		{
			BoneMesh* pBoneMesh = (BoneMesh*)pBone->pMeshContainer;

			//만약 본이 있고 스킨 정보가 있다면
			if (nullptr != pBoneMesh->BufBoneCombos)
			{
				LPD3DXBONECOMBINATION pBoneCombinations = (LPD3DXBONECOMBINATION)pBoneMesh->BufBoneCombos->GetBufferPointer();
				for (uint32 i = 0; i < pBoneMesh->NumAttributesGroup; ++i)
				{
					//해당 속성의 팔래트 엔트리 수만큼 돌면서 작업용 팔래트 행렬 갱신
					for (uint32 palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
					{
						//적용되는 행렬 ID 를 얻는다
						DWORD dwMatrixIndex = pBoneCombinations[i].BoneId[palEntry];

						//행렬 인덱스가 유효하다면...
						if (dwMatrixIndex != UINT_MAX)
						{
							//작업 앵렬을 만든다.
							MatrixMultiply(&_workingPalettes[palEntry],
								&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
								pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
						}
					}

					MatrixCache::CacheRange range = renderView._matrixCache.Add(_workingPalettes, _numPalette);

					Assert(pBoneMesh->_vHandle.IsValid());

					RenderCommand &command = renderView.GetCommand();
					command._drawType = RenderCommand::DrawType::eAnimated;
					command._vertInfluence = pBoneMesh->MaxNumFaceInfls;

					command._vHandle = pBoneMesh->_vHandle;
					command._iHandle = pBoneMesh->_iHandle;
					command._cacheRange = range;

					command._startIndex = pBoneCombinations[i].FaceStart * 3;
					command._numVertices = pBoneCombinations[i].VertexCount;
					command._numPrim = pBoneCombinations[i].FaceCount;

					command._primType = RenderCommand::PrimType::eTriangleList;
					command._materialHandle = pBoneMesh->_materialHandles[i];
					command._effectHandle = skinnedEffect;
				}
			}
			//스킨 정보가 없다면...
			else
			{
				MatrixCache::CacheRange range = renderView._matrixCache.Add(&pBone->CombinedTransformationMatrix);
				Assert(pBoneMesh->_attributeRange);
				for (uint32 i = 0; i < pBoneMesh->NumMaterials; ++i)
				{
					RenderCommand &command = renderView.GetCommand();
					command._drawType = RenderCommand::DrawType::eStatic;

					command._vHandle = pBoneMesh->_vHandle;
					command._iHandle = pBoneMesh->_iHandle;
					command._cacheRange = range;

					command._startIndex = pBoneMesh->_attributeRange[i].FaceStart * 3;
					command._numVertices = pBoneMesh->_attributeRange[i].VertexCount;
					command._numPrim = pBoneMesh->_attributeRange[i].FaceCount;

					command._primType = RenderCommand::PrimType::eTriangleList;
					command._materialHandle = pBoneMesh->_materialHandles[i];
					command._effectHandle = staticEffect;
				}

			}
		}

		if (pBone->pFrameSibling)
		{
			FillRenderCommandInternal(renderView, skinnedEffect, staticEffect, (Bone*)pBone->pFrameSibling);
		}
		if (pBone->pFrameFirstChild)
		{
			FillRenderCommandInternal(renderView, skinnedEffect, staticEffect, (Bone*)pBone->pFrameFirstChild);
		}
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

	//테스트용 본들 추가...
	//if (nullptr != Name)
	//{
	//	auto found = this->_pSkinnedMesh->_boneTable.find((*ppNewFrame)->Name);
	//	if (found == this->_pSkinnedMesh->_boneTable.end())
	//	{
	//		this->_pSkinnedMesh->_boneTable.insert(std::make_pair(std::string(Name), (Bone *)*ppNewFrame));
	//	}
	//}

	return S_OK;
}

STDMETHODIMP BoneHierachy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, 
	CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstance, 
	DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	//ppNewMeshContainer 로 넘겨줄 새로운 BONEMESH 생성
	BoneMesh* boneMesh = new BoneMesh;
	ZeroMemory(boneMesh, sizeof(BoneMesh));

	//매개변수로 받은 메쉬 데이터의 Device 를 얻는다.
	LPDIRECT3DDEVICE9 d3dDevice = NULL;
	pMeshData->pMesh->GetDevice(&d3dDevice);

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

	std::string PathCopy = _filePath;
	std::string name;
	std::string path;
	std::string extension;

	SplitFilePathToNamePathExtension(PathCopy, name, path, extension);

	std::string textureFileName;
	std::string textureExtension;

	std::string texPath = path;

	//여기서 Material정보 가져오자..
	boneMesh->NumMaterials = NumMaterials;
	boneMesh->pMaterials = new D3DXMATERIAL[NumMaterials];
	for (uint32 i = 0; i < NumMaterials; i++)
	{
		//메터리얼의 Texture 정보가 있다면..
		if (nullptr != pMaterials[i].pTextureFilename)
		{
			//tex 파일경로는 Mesh 파일경로 + texture 파일이름
			std::string textureString = pMaterials[i].pTextureFilename;
			SplitNameToNameExtension(textureString, textureFileName, textureExtension);

			video::TextureHandle diffuse = video::LoadTextureWithStrings(path, textureFileName, textureExtension, ".");
			video::TextureHandle normal = video::LoadTextureWithStrings(path, textureFileName, textureExtension, "_N.");
			video::TextureHandle specular = video::LoadTextureWithStrings(path, textureFileName, textureExtension, "_S.");
			video::TextureHandle emission = video::LoadTextureWithStrings(path, textureFileName, textureExtension, "_E.");

			video::MaterialHandle material = VIDEO->CreateMaterial(textureFileName);
			VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE_DIFFUSE, diffuse);
			VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE_NORMAL, normal);
			VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE_SPECULAR, specular);
			VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE_EMISSIVE, emission);

			boneMesh->_materialHandles.push_back(material);
		}
		else
		{
			video::MaterialHandle material = VIDEO->GetMaterial("default");
			boneMesh->_materialHandles.push_back(material);
		}
	}

	//속성테이블을 얻는다.
	boneMesh->MeshData.pMesh->GetAttributeTable(NULL, &boneMesh->NumAttributesGroup);
																					

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
		//UINT iPaletteSize = 0;
		//m_pSkinnedMesh->m_pSkinnedEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&iPaletteSize);

		//기본적으로 적용되는 본수 만큼 팔래트를 할당하는데 Effect 의 최대 MATRIX_PALETTE_SIZE 보다 적용되는 본의 
		//수가 많다면 팔래트 수는 Effect MATRIX_PALETTE_SIZE 수로 고정 한다.
		boneMesh->NumPaletteEntries = boneMesh->pSkinInfo->GetNumBones();

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
		if (_pSkinnedMesh->_numWorkingPalette < boneMesh->NumPaletteEntries)
		{
			_pSkinnedMesh->_numWorkingPalette = boneMesh->NumPaletteEntries;
		}
		boneMesh->_vHandle = VIDEO->GetVertexBufferFromXMesh(boneMesh->WorkingMesh);
		boneMesh->_iHandle = VIDEO->GetIndexBufferFromXMesh(boneMesh->WorkingMesh);
	}
	else
	{
		HRESULT re = boneMesh->MeshData.pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
			pAdjacency, nullptr, nullptr, nullptr);

		boneMesh->_attributeRange = new D3DXATTRIBUTERANGE[boneMesh->NumAttributesGroup];
		Assert(boneMesh->_attributeRange);

		boneMesh->MeshData.pMesh->GetAttributeTable(boneMesh->_attributeRange, &boneMesh->NumAttributesGroup);

		boneMesh->_vHandle = VIDEO->GetVertexBufferFromXMesh(boneMesh->MeshData.pMesh);
		boneMesh->_iHandle = VIDEO->GetIndexBufferFromXMesh(boneMesh->MeshData.pMesh);
	}

	*ppNewMeshContainer = boneMesh;
	
	//auto found = this->_pSkinnedMesh->_meshTable.find(Name);
	//if (found == this->_pSkinnedMesh->_meshTable.end())
	//{
	//	this->_pSkinnedMesh->_meshTable.insert(std::make_pair(std::string(Name), (BoneMesh *)*ppNewMeshContainer));
	//}

	//D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
	//boneMesh->WorkingMesh->GetDeclaration(elements);

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

	VIDEO->DestroyVertexBuffer(boneMesh->_vHandle);
	VIDEO->DestroyIndexBuffer(boneMesh->_iHandle);

	//텍스쳐 해제
	//해제 코드도 작성하자...
	UINT numTex = boneMesh->_materialHandles.size();
	for (UINT i = 0; i < numTex; i++)
	{
		//if (boneMesh->pMaterials[i].pTextureFilename != NULL)
		//{
		//	SAFE_DELETE_ARRAY(boneMesh->pMaterials[i].pTextureFilename);
		//}
	}
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
