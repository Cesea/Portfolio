#include "stdafx.h"
#include "VideoResource.h"

namespace video
{
	LPCSTR PredefinedUniform::ParamName[Enum::Count] = 
	{
		"gWorld",
		"gView",
		"gViewProjection",
		"gWorldViewProjection",
		"gDirectionalLight0",
		"gDirectionalLight1",
		"gDirectionalLight2",
		"gPointLight0",
		"gPointLight1",
		"gPointLight2",
		"gSpotLight0",
		"gSpotLight1",
		"gSpotLight2",
		"gMatrixPallete",
		"gTexture0",
		"gTexture1",
		"gTexture2",
		"gTexture3",
		"gTexture4",
		"gTexture5",
		"gTexture6"
	};


	//TODO : 지금 현재 다이나믹 버퍼는 지원이 되지 않는다. 만들자....
	bool VertexBuffer::Create(uint32 size, void *data, VertexDeclHandle decl)
	{
		if (nullptr == data)
		{
			_dynamic = true;
		}
		_size = size;
		_decl = decl;

		DWORD usage = D3DUSAGE_WRITEONLY;

		if (_dynamic)
		{
			usage |= D3DUSAGE_DYNAMIC;
		}

		VIDEO_CHECK(gpDevice->CreateVertexBuffer(size, usage, 0,
			D3DPOOL_DEFAULT, &_ptr, nullptr));

		if (nullptr != data)
		{
			Update(size, data);
		}
		return true;
	}

	void VertexBuffer::Update(uint32 size, void *data)
	{
		void *pWrite = nullptr;
		_ptr->Lock(0, size, (void **)&pWrite, 0);
		memcpy(pWrite, data, size);
		_ptr->Unlock();
	}

	void VertexBuffer::Destroy()
	{
		COM_RELEASE(_ptr);
	}

	void VertexBuffer::PreReset()
	{
	}

	void VertexBuffer::PostReset()
	{
	}

	bool Effect::Create(const std::string fileName)
	{
		DWORD flags = 0;
#if defined (DEBUG) || defined (_DEBUG)
		flags |= D3DXSHADER_DEBUG;
#endif
		
		LPD3DXBUFFER errorBuffer;
		HRESULT hresult = D3DXCreateEffectFromFile(gpDevice, fileName.c_str(), nullptr, nullptr, flags, nullptr, &_ptr, &errorBuffer);
		if (FAILED(hresult))
		{
			MessageBox(NULL, (char *)errorBuffer->GetBufferPointer(), "Error", MB_OK);
			errorBuffer->Release();
			Assert(false);
		}
		return true;
	}

	void Effect::Destroy()
	{
		COM_RELEASE(_ptr);
	}

	uint32 Effect::BeginEffect()
	{
		Assert(_ptr);
		uint32 result{};
		_ptr->Begin(&result, 0);
		return result;
	}

	void Effect::BeginPass(uint32 passNum)
	{
		_ptr->BeginPass(passNum);
	}

	void Effect::EndPass()
	{
		_ptr->EndPass();
	}

	void Effect::EndEffect()
	{
		_ptr->End();
	}

	void Effect::SetUniform(PredefinedUniform::Enum uniform, void *data)
	{
		switch (uniform)
		{
		case PredefinedUniform::eWorld:
		{
		} break;
		case PredefinedUniform::eView:
		{
		} break;
		case PredefinedUniform::eViewProjection:
		{
		} break;
		case PredefinedUniform::eWorldViewProjection:
		{
		} break;
		case PredefinedUniform::eDirectionalLight0:
		{
		} break;
		case PredefinedUniform::eDirectionalLight1:
		{
		} break;
		case PredefinedUniform::eDirectionalLight22:
		{
		} break;
		case PredefinedUniform::ePointLight0:
		{
		} break;
		case PredefinedUniform::ePointLight1:
		{
		} break;
		case PredefinedUniform::ePointLight2:
		{
		} break;
		case PredefinedUniform::eSpotLight0:
		{
		} break;
		case PredefinedUniform::eSpotLight1:
		{
		} break;
		case PredefinedUniform::eSpotLight2:
		{
		} break;
		case PredefinedUniform::eMatrixPallete:
		{
		} break;
		case PredefinedUniform::eTexture0:
		{
		} break;
		case PredefinedUniform::eTexture1:
		{
		} break;
		case PredefinedUniform::eTexture2:
		{
		} break;
		case PredefinedUniform::eTexture3:
		{
		} break;
		case PredefinedUniform::eTexture4:
		{
		} break;
		case PredefinedUniform::eTexture5:
		{
		} break;
		case PredefinedUniform::eTexture6:
		{
		} break;
		case PredefinedUniform::eTime:
		{
		} break;
		}
	}

	void Effect::SetMatrix(LPCSTR name, Matrix &matrix)
	{
		_ptr->SetMatrix(name, &matrix);
	}
	void Effect::SetTexture(LPCSTR name, Texture &texture)
	{
		_ptr->SetTexture(name, texture._ptr);
	}

	bool IndexBuffer::Create(uint32 size, void * data)
	{
		if (nullptr == data)
		{
			_dynamic = true;
		}
		_size = size;

		DWORD usage = D3DUSAGE_WRITEONLY;

		if (_dynamic)
		{
			usage |= D3DUSAGE_DYNAMIC;
		}

		VIDEO_CHECK(gpDevice->CreateIndexBuffer(size, usage, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &_ptr, nullptr));

		if (nullptr != data)
		{
			Update(size, data);
		}
		return true;
	}

	void IndexBuffer::Update(uint32 size, void *data)
	{
		void *pWrite = nullptr;
		_ptr->Lock(0, size, (void **)&pWrite, 0);
		memcpy(pWrite, data, size);
		_ptr->Unlock();
	}

	void IndexBuffer::Destroy()
	{
		COM_RELEASE(_ptr);
	}

	void IndexBuffer::PreReset()
	{
	}

	void IndexBuffer::PostReset()
	{
	}

	void VertexDecl::Begin()
	{
	}

	void VertexDecl::Add(D3DVERTEXELEMENT9 &element)
	{
		memcpy(&_elements[_count++], &element, sizeof(D3DVERTEXELEMENT9));
	}

	void VertexDecl::End(uint32 stride)
	{
		_stride = stride;

		D3DVERTEXELEMENT9 end = D3DDECL_END();
		memcpy(&_elements[_count], &end, sizeof(D3DVERTEXELEMENT9));

		HRESULT result = D3DXFVFFromDeclarator(_elements, &_fvf);

		VIDEO_CHECK(gpDevice->CreateVertexDeclaration(_elements, &_ptr));
	}

	void VertexDecl::Destroy()
	{
		COM_RELEASE(_ptr);
	}

	bool Texture::Create(const std::string fileName)
	{
		D3DXIMAGE_INFO imageInfo;
		if (FAILED(D3DXGetImageInfoFromFile(fileName.c_str(), &imageInfo)))
		{
			return false;
		}

		if (FAILED(D3DXCreateTextureFromFile(gpDevice, fileName.c_str(), &_ptr)))
		{
			return false;
		}

		_width = imageInfo.Width;
		_height = imageInfo.Height;
		_format = imageInfo.Format;

		return true;
	}

	void Texture::Destroy()
	{
		COM_RELEASE(_ptr);
	}

	bool FrameBuffer::Create(uint8 num, const TextureHandle * handles)
	{
		return false;
	}

	void FrameBuffer::Destroy()
	{
	}

	void FrameBuffer::Resolve() const
	{
	}

	void FrameBuffer::PreReset()
	{
	}

	void FrameBuffer::PostReset()
	{
	}

	void FrameBuffer::CreateNullColorRT()
	{
	}

	bool RenderView::Create()
	{
		return true;
	}

	void RenderView::Begin()
	{
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::Enum::eRendererInit);
	}
	void RenderView::End()
	{
		_commandBuffer.Finish();
	}

	void RenderView::SetViewProjection(const Matrix & viewMatrix, const Matrix & projectionMatrix)
	{
		_viewMatrix = viewMatrix;
		_projectionMatrix = projectionMatrix;
	}


	void RenderView::SetTransform(const Matrix &matrix)
	{
		uint32 matrixIndex = _matrixCache.Add(&matrix);
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::Enum::eSetTransform);
		_commandBuffer.Write<uint32>(matrixIndex);
	}

	void RenderView::SetEffect(EffectHandle handle)
	{
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::Enum::eSetEffect);
		_commandBuffer.Write<EffectHandle>(handle);
	}

	void RenderView::SetMaterial(MaterialHandle handle)
	{
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::Enum::eSetMaterial);
		_commandBuffer.Write<MaterialHandle>(handle);
	}

	void RenderView::SetFillMode(RenderState::FillMode mode)
	{
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::Enum::eSetFillMode);
		_commandBuffer.Write<RenderState::FillMode>(mode);
	}

	void RenderView::Submit(VertexBufferHandle handle, uint32 startVertex, uint32 primCount)
	{
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::eSetVertexBuffer);
		_commandBuffer.Write<VertexBufferHandle>(handle);
		_commandBuffer.Write<uint32>(startVertex);
		_commandBuffer.Write<uint32>(primCount);
	}

	void RenderView::Submit(IndexBufferHandle handle, uint32 startIndex, uint32 numVertices, uint32 primCount)
	{
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::eSetIndexBuffer);
		_commandBuffer.Write<IndexBufferHandle>(handle);

		_commandBuffer.Write<uint32>(startIndex);
		_commandBuffer.Write<uint32>(numVertices);
		_commandBuffer.Write<uint32>(primCount);
	}

	//SubmitGroup은 effect를 셋팅하지는 않는다
	void RenderView::SubmitGroup(RenderGroupHandle handle)
	{
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::eSetRenderGroup);
		_commandBuffer.Write<RenderGroupHandle>(handle);
	}

	void RenderView::Draw()
	{
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::eDraw);
	}

	void RenderView::PreRender()
	{
	}

	void RenderView::PostRender()
	{
		_commandBuffer.Reset();
		_matrixCache.Reset();
	}

	void RenderView::Destroy()
	{
	}

	bool Material::Create()
	{
		for (uint32 i = 0; i < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM; ++i)
		{
			_textureHandles[i].MakeInvalid();
		}
		return true;
	}

	void Material::Destroy()
	{
		for (uint32 i = 0; i < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM; ++i)
		{
			_textureHandles[i].MakeInvalid();
		}
	}
	void Material::AddTexture(uint32 textureSlot, TextureHandle handle)
	{
		Assert(textureSlot < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM);
		_textureHandles[textureSlot] = handle;
	}


	//Free Function
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
				resultHandle = VIDEO->GetTexture("diffuseDefault.png");
			}
		}
		return resultHandle;
	}

	bool Skeleton::Create()
	{
		return false;
	}

	void Skeleton::Destroy()
	{
	}

	bool Model::CreateFromXStatic(const std::string & filePath, const Matrix *pMatCorrection)
	{
		std::string nameCopy = filePath;
		std::string path;
		std::string extension;

		SplitFilePathToNamePathExtension(nameCopy, _name, path, extension);

		VertexBufferHandle vertexHandle;
		IndexBufferHandle indexHandle;

		XMeshStatic xMesh;
		HRESULT hresult = xMesh.Load(filePath, pMatCorrection);
		if (FAILED(hresult))
		{
			Assert(false);// xmesh load failed
			return false;
		}
		//우선 버텍스 버퍼랑 인덱스 퍼버의 정보를 가져온다
		IDirect3DVertexBuffer9 *pVertexBuffer = nullptr;
		IDirect3DIndexBuffer9 *pIndexBuffer = nullptr;
		xMesh._pMesh->GetVertexBuffer(&pVertexBuffer);
		xMesh._pMesh->GetIndexBuffer(&pIndexBuffer);

		D3DVERTEXBUFFER_DESC vertexDesc;
		pVertexBuffer->GetDesc(&vertexDesc);

		D3DINDEXBUFFER_DESC indexDesc;
		pIndexBuffer->GetDesc(&indexDesc);

		VertexDeclHandle declHandle = VertexDeclHandle();
		//VertexDeclHandle declHandle = VIDEO->GetVertexDecl("StaticTestVertex");
		//static decl이 이미 없다면
		if (!declHandle.IsValid())
		{
			VertexDecl decl;
			decl.Begin();
			for (uint32 i = 0; i < MAX_FVF_DECL_SIZE; i++)
			{
				if (xMesh._pVerElement[i].Type == D3DDECLTYPE_UNUSED)
				{
					break;
				}

				decl.Add(xMesh._pVerElement[i]);
			}

			//이미 셋팅된 vertexElement에서 알맞은 fvf를 추출하는 함수이다.
			decl.End(D3DXGetDeclVertexSize(xMesh._pVerElement, 0));

			declHandle = VIDEO->CreateVertexDecl(&decl);
		}

		Memory mem;

		//버텍스 버퍼 락 하고 정보를 가져온다
		void *pVertexData = nullptr;
		pVertexBuffer->Lock(0, 0, (void **)&pVertexData, 0);
		mem._size = vertexDesc.Size;
		mem._data = pVertexData;
		vertexHandle =  VIDEO->CreateVertexBuffer(&mem, declHandle, _name);
		pVertexBuffer->Unlock();

		//인덱스 버퍼 락 하고 정보를 가져온다
		void *pIndexData = nullptr;
		pIndexBuffer->Lock(0, 0, (void **)&pIndexData, 0);
		mem._size = indexDesc.Size;
		mem._data = pIndexData;
		indexHandle = VIDEO->CreateIndexBuffer(&mem, _name);
		pIndexBuffer->Unlock();

		COM_RELEASE(pVertexBuffer);
		COM_RELEASE(pIndexBuffer);

		std::string texturePath;
		std::string textureName;
		std::string textureExtension;

		//Material Range정보와 머테리얼 정보를 xMesh에서 가져온다
		for (uint32 i = 0; i < xMesh._numMaterial; ++i)
		{
			TextureHandle diffuseHandle;
			TextureHandle normalHandle;
			TextureHandle specularHandle;
			TextureHandle emissionHandle;
			//텍스쳐를 로딩한다
			//텍스쳐 정보가 없다면 흰색 텍스쳐를 로드를 시킨다
			if (0 == xMesh._texturePaths[i].size())
			{
				diffuseHandle = VIDEO->GetTexture("diffuseDefault.png");
				normalHandle = VIDEO->GetTexture("normalDefault.png");
				specularHandle = VIDEO->GetTexture("specularDefault.png");
				emissionHandle = VIDEO->GetTexture("emissionDefault.png");
			}
			//텍스쳐 정보가 있다면 디퓨즈를 먼저 로딩하고, 스펙, 노말, 이미션... 등등을 로딩한다
			//텍스쳐는 video device에 이름만으로 저장이 되어있다. 이미 있는지 체크할때 이름만 사용하도록 하자.
			else
			{
				SplitFilePathToNamePathExtension(xMesh._texturePaths[i], textureName, texturePath, textureExtension);

				diffuseHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, ".");
				normalHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_N.");
				specularHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_S.");
				emissionHandle = LoadTextureWithStrings(texturePath, textureName, textureExtension, "_E.");
			}
			RenderGroup::MaterialRange matRange;

			//머테리얼을 찾아보고 없다면 만든다
			std::string renderGroupName = _name + "_subset_" + std::to_string(i);
			MaterialHandle matHandle = VIDEO->GetMaterial(renderGroupName);
			if (!matHandle.IsValid())
			{
				matHandle = VIDEO->CreateMaterial(renderGroupName);
			}
			matRange._material = matHandle;

			VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_DIFFUSE, diffuseHandle);
			VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_NORMAL, normalHandle);
			VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_SPECULAR, specularHandle);
			VIDEO->MaterialAddTexture(matRange._material, VIDEO_TEXTURE_EMMISIVE, emissionHandle);

			//MaterialRange를 불러온다
			matRange._numPrim = xMesh._attributeRange[i].FaceCount;
			matRange._numVertices = xMesh._attributeRange[i].VertexCount;
			matRange._startIndex = xMesh._attributeRange[i].FaceStart * 3;
			matRange._startVertex = xMesh._attributeRange[i].VertexStart;

			RenderGroupHandle renderGroup = VIDEO->CreateRenderGroup(vertexHandle, indexHandle, matRange, _name + "subset_" + std::to_string(i));
			this->_groups.push_back(renderGroup);
		}
		xMesh.Release();
	}

	bool Model::CreateFromXAnimated(const std::string & filePath, const Matrix * pMatCorrection)
	{
		std::string nameCopy = filePath;
		std::string path;
		std::string extension;

		SplitFilePathToNamePathExtension(nameCopy, _name, path, extension);

		VertexBufferHandle vertexHandle;
		IndexBufferHandle indexHandle;

		XMeshAnimated xMesh;
		HRESULT hresult = xMesh.Load(filePath, pMatCorrection);
		if (FAILED(hresult))
		{
			Assert(false);// xmesh load failed
			return false;
		}

		uint32 numBone;
		xMesh.GetTotalNumFrame(&numBone);
		Assert(numBone > 0);

		_skeleton._hierachy = new uint16[numBone];
		_skeleton._globalPoses = new Matrix[numBone];
		_skeleton._localPoses = new Matrix[numBone];
		_skeleton._names = new Skeleton::SkeletonName[numBone];
		_skeleton._numhierachy = numBone;

		_skeleton._hierachy[0] = 0;
		MatrixIdentity(&_skeleton._globalPoses[0]);
		MatrixIdentity(&_skeleton._localPoses[0]);

		for (uint32 i = 0; i < numBone; ++i)
		{
			_skeleton._hierachy[i] = xMesh._frameIndex[i];
			_skeleton._localPoses[i] = xMesh._linearFrames[i]->TransformationMatrix;

			if (nullptr != xMesh._linearFrames[i]->Name)
			{
				strncpy(_skeleton._names[i]._name, xMesh._linearFrames[i]->Name, 64);
			}
			else
			{
				char buffer[64] = {0,};
				sprintf(buffer, "Bone %d", i);
				strncpy(_skeleton._names[i]._name, buffer, 64);
			}
		}

		animation::UpdateSkeleton(&_skeleton, nullptr);

		xMesh.Release();
	}

	void Model::Destroy()
	{
	}

	bool RenderGroup::Create(video::VertexBufferHandle vHandle, video::IndexBufferHandle iHandle, const RenderGroup::MaterialRange & materialRange)
	{
		_vertexBuffer = vHandle;
		_indexBuffer = iHandle;
		
		_materialRange = materialRange;
		return true;
	}

	void RenderGroup::Destroy()
	{
		return;
	}

	void RenderState::ResetDefault()
	{
		_indexBuffer.MakeInvalid();

		//NOTE : 전에 렌더를 했을때 이펙트를 사용하여 렌더 하지 않않다
		// fvf나 다른 device와 관련된 셋팅을 변경했을 가능성이 있으므로 상태를 리셋해준다
		if (!_effect.IsValid())
		{
			_fillMode = FillMode::eFillSolid;
			gpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			gpDevice->SetFVF(0);
		}
	}

	

}