#include "stdafx.h"
#include "VideoResource_Mesh.h"

namespace video
{
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

	//Free Function
	//void ExtractMeshFromXStatic(XMeshStatic &xMesh, Model *model, const std::string &filePath)
	//{
	//	std::string nameCopy = filePath;
	//	std::string path;
	//	std::string extension;

	//	SplitFilePathToNamePathExtension(nameCopy, model->_name, path, extension);

	//	VertexBufferHandle vertexHandle;
	//	IndexBufferHandle indexHandle;

	//	//우선 버텍스 버퍼랑 인덱스 퍼버의 정보를 가져온다
	//	IDirect3DVertexBuffer9 *pVertexBuffer = nullptr;
	//	IDirect3DIndexBuffer9 *pIndexBuffer = nullptr;
	//	xMesh._pMesh->GetVertexBuffer(&pVertexBuffer);
	//	xMesh._pMesh->GetIndexBuffer(&pIndexBuffer);

	//	D3DVERTEXBUFFER_DESC vertexDesc;
	//	pVertexBuffer->GetDesc(&vertexDesc);

	//	D3DINDEXBUFFER_DESC indexDesc;
	//	pIndexBuffer->GetDesc(&indexDesc);

	//	//TODO : VertexDecl을 미리 만들어두고 하도록 하자....
	//	VertexDeclHandle declHandle = VertexDeclHandle();
	//	//VertexDeclHandle declHandle = VIDEO->GetVertexDecl("StaticTestVertex");
	//	//static decl이 이미 없다면
	//	if (!declHandle.IsValid())
	//	{
	//		VertexDecl decl;
	//		decl.Begin();
	//		for (uint32 i = 0; i < MAX_FVF_DECL_SIZE; i++)
	//		{
	//			if (xMesh._pVerElement[i].Type == D3DDECLTYPE_UNUSED)
	//			{
	//				break;
	//			}
	//			decl.Add(xMesh._pVerElement[i]);
	//		}

	//		//이미 셋팅된 vertexElement에서 알맞은 fvf를 추출하는 함수이다.
	//		decl.End(D3DXGetDeclVertexSize(xMesh._pVerElement, 0));

	//		declHandle = VIDEO->CreateVertexDecl(&decl);
	//	}

	//	Memory mem;

	//	//버텍스 버퍼 락 하고 정보를 가져온다
	//	void *pVertexData = nullptr;
	//	pVertexBuffer->Lock(0, 0, (void **)&pVertexData, 0);
	//	mem._size = vertexDesc.Size;
	//	mem._data = pVertexData;
	//	vertexHandle =  VIDEO->CreateVertexBuffer(&mem, declHandle, model->_name);
	//	pVertexBuffer->Unlock();

	//	//인덱스 버퍼 락 하고 정보를 가져온다
	//	void *pIndexData = nullptr;
	//	pIndexBuffer->Lock(0, 0, (void **)&pIndexData, 0);
	//	mem._size = indexDesc.Size;
	//	mem._data = pIndexData;
	//	indexHandle = VIDEO->CreateIndexBuffer(&mem, model->_name);
	//	pIndexBuffer->Unlock();

	//	COM_RELEASE(pVertexBuffer);
	//	COM_RELEASE(pIndexBuffer);

	//	std::string texturePath;
	//	std::string textureName;
	//	std::string textureExtension;

	//	//Material Range정보와 머테리얼 정보를 xMesh에서 가져온다
	//	for (uint32 i = 0; i < xMesh._numMaterial; ++i)
	//	{
	//		TextureHandle diffuseHandle;
	//		TextureHandle normalHandle;
	//		TextureHandle specularHandle;
	//		TextureHandle emissionHandle;
	//		//텍스쳐를 로딩한다
	//		//텍스쳐 정보가 없다면 흰색 텍스쳐를 로드를 시킨다
	//		if (0 == xMesh._texturePaths[i].size())
	//		{
	//			diffuseHandle = VIDEO->GetTexture("diffuseDefault.png");
	//			normalHandle = VIDEO->GetTexture("normalDefault.png");
	//			specularHandle = VIDEO->GetTexture("specularDefault.png");
	//			emissionHandle = VIDEO->GetTexture("emissionDefault.png");
	//		}
	//		//텍스쳐 정보가 있다면 디퓨즈를 먼저 로딩하고, 스펙, 노말, 이미션... 등등을 로딩한다
	//		//텍스쳐는 video device에 이름만으로 저장이 되어있다. 이미 있는지 체크할때 이름만 사용하도록 하자.
	//		else
	//		{
	//			SplitFilePathToNamePathExtension(xMesh._texturePaths[i], textureName, texturePath, textureExtension);

	//			diffuseHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, ".");
	//			normalHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_N.");
	//			specularHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_S.");
	//			emissionHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_E.");
	//		}
	//		RenderGroup::MaterialRange matRange;

	//		//머테리얼을 찾아보고 없다면 만든다
	//		std::string renderGroupName = model->_name + "_subset_" + std::to_string(i);
	//		MaterialHandle matHandle = VIDEO->GetMaterial(renderGroupName);
	//		if (!matHandle.IsValid())
	//		{
	//			matHandle = VIDEO->CreateMaterial(renderGroupName);
	//		}
	//		matRange._material = matHandle;

	//		VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_DIFFUSE, diffuseHandle);
	//		VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_NORMAL, normalHandle);
	//		VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_SPECULAR, specularHandle);
	//		VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_EMISSIVE, emissionHandle);

	//		//MaterialRange를 불러온다
	//		matRange._numPrim = xMesh._attributeRange[i].FaceCount;
	//		matRange._numVertices = xMesh._attributeRange[i].VertexCount;
	//		matRange._startIndex = xMesh._attributeRange[i].FaceStart * 3;
	//		matRange._startVertex = xMesh._attributeRange[i].VertexStart;

	//		RenderGroupHandle renderGroup = VIDEO->CreateRenderGroup(vertexHandle, indexHandle, 
	//			matRange, model->_name + "subset_" + std::to_string(i));
	//		model->_groups.push_back(renderGroup);
	//	}
	//}

	////Free Function
	//void ExtractModelFromContainerEX(ContainerEX &container, Model *model, uint32 index, const std::string filePath)
	//{
	//	std::string nameCopy = filePath;
	//	std::string path;
	//	std::string extension;

	//	SplitFilePathToNamePathExtension(nameCopy, model->_name, path, extension);

	//	VertexBufferHandle vertexHandle;
	//	IndexBufferHandle indexHandle;

	//	//우선 버텍스 버퍼랑 인덱스 퍼버의 정보를 가져온다
	//	IDirect3DVertexBuffer9 *pVertexBuffer = nullptr;
	//	IDirect3DIndexBuffer9 *pIndexBuffer = nullptr;
	//	container.MeshData.pMesh->GetVertexBuffer(&pVertexBuffer);
	//	container.MeshData.pMesh->GetIndexBuffer(&pIndexBuffer);

	//	D3DVERTEXBUFFER_DESC vertexDesc;
	//	pVertexBuffer->GetDesc(&vertexDesc);

	//	D3DINDEXBUFFER_DESC indexDesc;
	//	pIndexBuffer->GetDesc(&indexDesc);

	//	//TODO : VertexDecl을 미리 만들어두고 하도록 하자....
	//	VertexDeclHandle declHandle = VIDEO->GetVertexDecl(video::StaticMeshVertex::_name);
	//	Assert(declHandle.IsValid());

	//	Memory mem;

	//	//버텍스 버퍼 락 하고 정보를 가져온다
	//	void *pVertexData = nullptr;
	//	pVertexBuffer->Lock(0, 0, (void **)&pVertexData, 0);
	//	mem._size = vertexDesc.Size;
	//	mem._data = pVertexData;
	//	vertexHandle = VIDEO->CreateVertexBuffer(&mem, declHandle, model->_name);
	//	pVertexBuffer->Unlock();

	//	//인덱스 버퍼 락 하고 정보를 가져온다
	//	void *pIndexData = nullptr;
	//	pIndexBuffer->Lock(0, 0, (void **)&pIndexData, 0);
	//	mem._size = indexDesc.Size;
	//	mem._data = pIndexData;
	//	indexHandle = VIDEO->CreateIndexBuffer(&mem, model->_name);
	//	pIndexBuffer->Unlock();

	//	COM_RELEASE(pVertexBuffer);
	//	COM_RELEASE(pIndexBuffer);

	//	std::string texturePath;
	//	std::string textureName;
	//	std::string textureExtension;

	//	//Material Range정보와 머테리얼 정보를 xMesh에서 가져온다
	//	for (uint32 i = 0; i < container._texturePaths.size(); ++i)
	//	{
	//		TextureHandle diffuseHandle;
	//		TextureHandle normalHandle;
	//		TextureHandle specularHandle;
	//		TextureHandle emissionHandle;
	//		//텍스쳐를 로딩한다
	//		//텍스쳐 정보가 없다면 흰색 텍스쳐를 로드를 시킨다
	//		if (0 == container._texturePaths[i].size())
	//		{
	//			diffuseHandle = VIDEO->GetTexture("diffuseDefault.png");
	//			normalHandle = VIDEO->GetTexture("normalDefault.png");
	//			specularHandle = VIDEO->GetTexture("specularDefault.png");
	//			emissionHandle = VIDEO->GetTexture("emissionDefault.png");
	//		}
	//		//텍스쳐 정보가 있다면 디퓨즈를 먼저 로딩하고, 스펙, 노말, 이미션... 등등을 로딩한다
	//		//텍스쳐는 video device에 이름만으로 저장이 되어있다. 이미 있는지 체크할때 이름만 사용하도록 하자.
	//		else
	//		{
	//			texturePath = path;
	//			SplitNameToNameExtension(container._texturePaths[i], textureName, textureExtension);

	//			diffuseHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, ".");
	//			normalHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_N.");
	//			specularHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_S.");
	//			emissionHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_E.");
	//		}
	//		RenderGroup::MaterialRange matRange;
	//		//머테리얼을 찾아보고 없다면 만든다
	//		std::string renderGroupName = model->_name + "_subset_" + std::to_string(i);
	//		MaterialHandle matHandle = VIDEO->GetMaterial(renderGroupName);
	//		if (!matHandle.IsValid())
	//		{
	//			matHandle = VIDEO->CreateMaterial(renderGroupName);
	//		}
	//		matRange._material = matHandle;
	//		VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_DIFFUSE, diffuseHandle);
	//		VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_NORMAL, normalHandle);
	//		VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_SPECULAR, specularHandle);
	//		VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_EMMISIVE, emissionHandle);
	//		//MaterialRange를 불러온다
	//		matRange._numPrim = container._attributeRange[i].FaceCount;
	//		matRange._numVertices = container._attributeRange[i].VertexCount;
	//		matRange._startIndex = container._attributeRange[i].FaceStart * 3;
	//		matRange._startVertex = container._attributeRange[i].VertexStart;
	//		RenderGroupHandle renderGroup = VIDEO->CreateRenderGroup(vertexHandle, indexHandle,
	//			matRange, model->_name + "subset_" + std::to_string(i));
	//		model->_skeleton._renderGroups[index] = renderGroup;
	//	}
	//}

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

		D3DVERTEXELEMENT9 vertexElement[MAX_FVF_DECL_SIZE];
		_pMesh->GetDeclaration(vertexElement);



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
				MaterialHandle material = VIDEO->GetMaterial("default");
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
		//보정행렬을 받았다면..
		if (matCorrection)
		{
			this->MeshCorrection(matCorrection);
		}
		else
		{
			//보정행렬이 없더라도 보정처리를해야 Bound 정보를 얻을수 있다.
			Matrix matIden;
			MatrixIdentity(&matIden);
			this->MeshCorrection(&matIden);
		}

		BuidSubMeshBoundInfo();
	}

	void StaticXMesh::Destroy()
	{
		COM_RELEASE(_pMesh);
		SAFE_DELETE_ARRAY(_attributeRange);
	}

	void StaticXMesh::MeshCorrection(const Matrix * pMatCorrection)
	{
		D3DVERTEXELEMENT9 vertexElements[MAX_FVF_DECL_SIZE];

		_pMesh->GetDeclaration(vertexElements);
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

		_numVertices = _pMesh->GetNumVertices();
		uint32 vertexStride = D3DXGetDeclVertexSize(vertexElements, 0);

		//메쉬의 버텍스 버퍼를 Lock 한다
		void* pVertexData = nullptr;
		_pMesh->LockVertexBuffer(0, &pVertexData);

		//바운드 MinMax 계산을 위한 초기화......
		_meshBoundInfo._min = Vector3(0, 0, 0);
		_meshBoundInfo._max = Vector3(0, 0, 0);

		//버텍스 수만클 돌아 재낀다....
		for (uint32 i = 0; i < _numVertices; i++)
		{
			//버텍스 시작 주소
			uint8* pVertex = ((uint8*)pVertexData + (i * vertexStride));

			//정점 위치가 있다면...
			if (positionOffset != -1)
			{
				Vector3* pos = (Vector3*)(pVertex + positionOffset);

				Vec3TransformCoord(pos, pos, pMatCorrection);

				//정점 최소 값갱신
				if (_meshBoundInfo._min.x > pos->x)		_meshBoundInfo._min.x = pos->x;
				if (_meshBoundInfo._min.y > pos->y)		_meshBoundInfo._min.y = pos->y;
				if (_meshBoundInfo._min.z > pos->z)		_meshBoundInfo._min.z = pos->z;

				//정점 최대 값갱신
				if (_meshBoundInfo._max.x < pos->x)		_meshBoundInfo._max.x = pos->x;
				if (_meshBoundInfo._max.y < pos->y)		_meshBoundInfo._max.y = pos->y;
				if (_meshBoundInfo._max.z < pos->z)		_meshBoundInfo._max.z = pos->z;

				//정점 위치 푸쉬
				_vertices.push_back(*pos);
			}

			//노말정보가 있다면..
			if (normalOffet != -1)
			{
				Vector3* nor = (Vector3*)(pVertex + normalOffet);
				Vec3TransformNormal(nor, nor, pMatCorrection);
				Vec3Normalize(nor, nor);
				_normals.push_back(*nor);
			}
			//tangent 정보가 있다면.
			if (tangentOffet != -1)
			{
				Vector3* tangent = (Vector3*)(pVertex + tangentOffet);
				Vec3TransformNormal(tangent, tangent, pMatCorrection);
				Vec3Normalize(tangent, tangent);
			}
			//binormal 정보가 있다면
			if (binormalOffet != -1)
			{
				Vector3* binor = (Vector3*)(pVertex + binormalOffet);
				Vec3TransformNormal(binor, binor, pMatCorrection);
				Vec3Normalize(binor, binor);
			}
		}
		_pMesh->UnlockVertexBuffer();

		//Bound 추가 계산
		_meshBoundInfo._center = (_meshBoundInfo._min + _meshBoundInfo._max) * 0.5f;

		_meshBoundInfo._size =
			Vector3(_meshBoundInfo._max.x - _meshBoundInfo._min.x,
				_meshBoundInfo._max.y - _meshBoundInfo._min.y,
				_meshBoundInfo._max.z - _meshBoundInfo._min.z);

		_meshBoundInfo._halfSize = _meshBoundInfo._size * 0.5f;
		_meshBoundInfo._radius = D3DXVec3Length(&(_meshBoundInfo._center - _meshBoundInfo._min));

		LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
		_pMesh->GetIndexBuffer(&pIndexBuffer);

		//면의 갯수
		_numFaces = _pMesh->GetNumFaces();

		//인덱스 버퍼에 대한 정보를 얻는다.
		D3DINDEXBUFFER_DESC desc;
		pIndexBuffer->GetDesc(&desc);

		if (desc.Format == D3DFMT_INDEX16)
		{
			uint16* pIndexData = NULL;
			pIndexBuffer->Lock(0, 0, (void**)&pIndexData, 0);

			for (uint32 i = 0; i < _numFaces; i++)
			{
				_indices.push_back(pIndexData[i * 3 + 0]);
				_indices.push_back(pIndexData[i * 3 + 1]);
				_indices.push_back(pIndexData[i * 3 + 2]);

			}
			pIndexBuffer->Unlock();
		}
		else if (desc.Format == D3DFMT_INDEX32)
		{
			uint32* pIndexData = NULL;
			pIndexBuffer->Lock(0, 0, (void**)&pIndexData, 0);

			for (uint32 i = 0; i < _numFaces; i++)
			{
				_indices.push_back(pIndexData[i * 3 + 0]);
				_indices.push_back(pIndexData[i * 3 + 1]);
				_indices.push_back(pIndexData[i * 3 + 2]);
			}

			pIndexBuffer->Unlock();
		}
		//얻어온 인덱스 버퍼는 해재
		SAFE_RELEASE(pIndexBuffer);
	}

	void StaticXMesh::BuidSubMeshBoundInfo()
	{
		for (uint32 i = 0; i < _numMaterial; ++i)
		{
			const D3DXATTRIBUTERANGE &pAttributeRange = _attributeRange[i];
			_submeshBoundInfos.push_back(MeshBoundInfo());
			CalculateBoundingInfo(_vertices, &_submeshBoundInfos[i],
				pAttributeRange.VertexStart, pAttributeRange.VertexStart + pAttributeRange.VertexCount);
		}
	}

	void StaticXMesh::CalculateBoundingInfo(std::vector<Vector3>& vertices, MeshBoundInfo * pOutBoundInfo, uint32 startVertex, uint32 endVertex)
	{
		memset(pOutBoundInfo, 0, sizeof(MeshBoundInfo));
		Assert(startVertex >= 0);
		Assert(endVertex <= _vertices.size());
		for (uint32 i = startVertex; i < endVertex; ++i)
		{
			const Vector3 &refVertex = _vertices[i];
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

		if (FAILED(D3DXLoadMeshHierarchyFromX( fileName.c_str(), D3DXMESH_MANAGED, gpDevice,
			&boneHierachy, nullptr, (D3DXFRAME **)&_pRootBone, &_pAnimationController)))
		{
			Console::Log("Load Mesh HierarchyFromX failed\n");
			return false;
		}
		//본 매트릭스 포인터 생성
		InitBoneMatrixPointer((Bone*)_pRootBone);

		//Animation 갯수를 얻는다.
		//m_AnimNum = m_pAnimController->GetNumAnimationSets();
		//for (UINT i = 0; i < m_AnimNum; i++)
		//{
		//	//에니메이션 셋 정보
		//	LPD3DXANIMATIONSET animSet;
		//	//i 번째 Animation 셋 정보를 얻는다.
		//	m_pAnimController->GetAnimationSet(i, &animSet);
		//	//AnimationSet Vector 에 저장
		//	this->m_vecAnimSet.push_back(animSet);
		//	//AnimationSet Map 에 저장
		//	this->m_mapAnimSet.insert(std::make_pair(
		//		animSet->GetName(),
		//		animSet));
		//}
	}

	void SkinnedXMesh::Destroy()
	{
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

	void SkinnedXMesh::UpdateMatrices(Bone * pBone, Matrix *pParentMatrix)
	{
		if (nullptr == pBone)
		{
			return;
		}

		//부모 행렬이 있다면..
		if (nullptr != pParentMatrix)
		{
			Matrix matTransformation = pBone->TransformationMatrix;

			//적용 Transform 이 물려 있다면...
			//if (pBone->pApplyTransform != NULL)
			//{
			//	D3DXMATRIXA16 pApplyMatrix = pBone->pApplyTransform->GetWorldRotateMatrix();
			//	matTransformation = matTransformation * pApplyMatrix;
			//	pBone->pApplyTransform = NULL;
			//}

			//pBone->CombinedTransformationMatrix = pBone->TransformationMatrix * *(pParentMatrix);
			//pBone 의 행렬 부모행렬을 곱하여 pBone 의 최종 행렬을 구한다.
			MatrixMultiply( &pBone->CombinedTransformationMatrix, &matTransformation, pParentMatrix);
		}
		//부모 행렬이 없다면..
		else
		{
			//자신의 행렬이 현재 행렬이 된다.
			pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
		}

		////본 Transform 이 물려 있다면....
		//if (pBone->pBoneTransform != NULL)
		//{
		//	pBone->pBoneTransform->SetWorldMatrix(
		//		pBone->CombinedTransformationMatrix);
		//	pBone->pBoneTransform->SetScale(1, 1, 1);
		//	pBone->pBoneTransform = NULL;
		//}

		//이웃 본이 존재한다면...
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

	void SkinnedXMesh::Render()
	{
		RenderBone(_pRootBone);
	}

	void SkinnedXMesh::RenderBone(Bone * pBone)
	{
		if (nullptr != pBone)
		{
			return;
		}

		//본에 메쉬 컨테이너가 존재한다면 그려야한다.
		if (pBone->pMeshContainer)
		{
			//본에 있는 메쉬 컨테이너는 BONE_MESH 이다
			BoneMesh *pBoneMesh = (BoneMesh*)pBone->pMeshContainer;
			//본 컴비네이션 정보가  
			if (nullptr != pBoneMesh->BufBoneCombos)
			{
				//해당 본의 컴비네이션 정보를 얻는다.
				LPD3DXBONECOMBINATION pBoneComb = (LPD3DXBONECOMBINATION)(pBoneMesh->BufBoneCombos->GetBufferPointer());

				//본에 물려있는 메쉬의 서브셋갯수을 속성그룹수와 같다
				for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
				{
					//해당 속성의 팔래트 엔트리 수만큼 돌면서 작업용 팔래트 행렬 갱신
					for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
					{
						//적용되는 행렬 ID 를 얻는다
						DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

						//행렬 인덱스가 유효하다면...
						if (dwMatrixIndex != UINT_MAX)
						{
							//작업 앵렬을 만든다.
							MatrixMultiply(&_workingPalettes[palEntry],
								&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
								pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
						}
					}

					////위에서 셋팅됭 작업행렬을 Effect 팔래스에 적용한다.
					//_pSkinnedEffect->SetMatrixArray(
					//	"amPalette", m_pmWorkingPalette, pBoneMesh->NumPaletteEntries);

					////적용되는 정점의 본최대 영향수 를 대입 최대 영향수  -1 
					//m_pSkinnedEffect->SetInt("CurNumBones", pBoneMesh->MaxNumFaceInfls - 1);

					////메터리얼 인덱스
					//DWORD materialIndex = pBoneComb[i].AttribId;
					////텍스쳐 셋팅
					//m_pSkinnedEffect->SetTexture("Diffuse_Tex", pBoneMesh->DiffuseTexs[materialIndex]);
					//m_pSkinnedEffect->SetTexture("Specular_Tex", pBoneMesh->SpecularTexs[materialIndex]);
					//m_pSkinnedEffect->SetTexture("Normal_Tex", pBoneMesh->NormalTexs[materialIndex]);
					//m_pSkinnedEffect->SetTexture("Emission_Tex", pBoneMesh->EmissionTexs[materialIndex]);
					//m_pSkinnedEffect->SetFloat("fSpecPower", pBoneMesh->Materials[materialIndex].Power);

					//uint32 uiPasses, uiPass;
					//_pSkinnedEffect->Begin(&uiPasses, 0);
					//for (uiPass = 0; uiPass < uiPasses; ++uiPass)
					//{
					//	m_pSkinnedEffect->BeginPass(uiPass);

					pBoneMesh->WorkingMesh->DrawSubset(i);

					//	m_pSkinnedEffect->EndPass();
					//}
					//m_pSkinnedEffect->End();
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
																					
	//NOTE : 아래에서 converteToIndexedblendMesh로 변환할떄 bonCombo를 얻는데 그 정보를 사용한다...
	//boneMesh->MeshData.pMesh->GetAttributeTable( boneMesh->AttributeTable, NULL );//속성 테이블을 얻는다.

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

		//cMesh_Skinned 에 작업에 사용되는 팔래트 행렬 수가 부족하면 
		//다시 재할당 한다.
		if (_pSkinnedMesh->_numWorkingPalette < boneMesh->NumPaletteEntries)
		{
			if (_pSkinnedMesh->_numWorkingPalette)
			{
				SAFE_DELETE_ARRAY(_pSkinnedMesh->_workingPalettes);
			}

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

	SAFE_RELEASE(boneMesh->MeshData.pMesh);
	SAFE_RELEASE(boneMesh->WorkingMesh);
	SAFE_RELEASE(boneMesh->pSkinInfo);

	SAFE_DELETE_ARRAY(boneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(boneMesh->pBoneOffsetMatices);
	SAFE_RELEASE(boneMesh->BufBoneCombos);

	SAFE_DELETE(boneMesh);

	return S_OK;
}
