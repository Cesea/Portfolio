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

	uint32 Effect::BeginEffect() const
	{
		Assert(_ptr);
		uint32 result{};
		_ptr->Begin(&result, 0);
		return result;
	}

	void Effect::BeginPass(uint32 passNum) const
	{
		_ptr->BeginPass(passNum);
	}

	void Effect::EndPass() const
	{
		_ptr->EndPass();
	}

	void Effect::EndEffect() const
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

	void Effect::SetMatrix(LPCSTR name, Matrix &matrix) const
	{
		_ptr->SetMatrix(name, &matrix);
	}

	void Effect::SetMatrices(LPCSTR name, const Matrix *matrices, uint32 numMatrices) const
	{
		HRESULT re = _ptr->SetMatrixArray(name, matrices, numMatrices);
		int a = 0;
	}

	void Effect::SetTexture(const std::string &name, const Texture &texture) const
	{
		HRESULT re = _ptr->SetTexture(name.c_str(), texture._ptr);
		int a = 0;
	}

	void Effect::SetMaterial(const Material & material) const
	{
		for (uint32 i = 0; i < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM; ++i)
		{
			if (material._textureHandles[i].IsValid())
			{
				this->SetTexture("gTexture" + std::to_string(i), *VIDEO->GetTexture(material._textureHandles[i]));
			}
		}
	}

	void Effect::SetInt(LPCSTR name, const int32 value) const
	{
		_ptr->SetInt(name, value);
	}

	void Effect::CommitChanges() const
	{
		_ptr->CommitChanges();
	}

	void Effect::DrawStaticMesh(const StaticXMesh &mesh, LPCSTR technique) const
	{
		for (uint32 i = 0; i < mesh._numMaterial; ++i)
		{
			uint32 numPass = this->BeginEffect();
			const Material *mat = VIDEO->GetMaterial(mesh._materialHandles[i]);
			this->SetMaterial(*mat);
			this->CommitChanges();

			for (uint32 j = 0; j < numPass; ++j)
			{
				this->BeginPass(j);
				mesh._pMesh->DrawSubset(j);
				this->EndPass();
			}
			this->EndEffect();
		}
	}

	void Effect::DrawSkinnedMesh(const SkinnedXMesh &mesh, LPCSTR technique) const
	{
		mesh.RenderBone(*this, mesh._pRootBone);
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
		_commandBuffer.Write<CommandBuffer::Enum>(CommandBuffer::Enum::eChangeFillMode);
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



	//NOTE : Implement this
	bool Skeleton::Create()
	{
		return false;
	}

	void Skeleton::Destroy()
	{
	}

	//bool Model::CreateFromXStatic(const std::string & filePath, const Matrix *pMatCorrection)
	//{
	//	XMeshStatic xMesh;
	//	HRESULT hresult = xMesh.Load(filePath, pMatCorrection);
	//	if (FAILED(hresult))
	//	{
	//		Assert(false);// xmesh load failed
	//		return false;
	//	}
	//	ExtractMeshFromXStatic(xMesh, this, filePath);
	//	xMesh.Release();
	//}

	//bool Model::CreateFromXAnimated(const std::string & filePath, const Matrix * pMatCorrection)
	//{
	//	std::string nameCopy = filePath;
	//	std::string path;
	//	std::string extension;

	//	SplitFilePathToNamePathExtension(nameCopy, _name, path, extension);

	//	VertexBufferHandle vertexHandle;
	//	IndexBufferHandle indexHandle;

	//	XMeshAnimated xMesh;
	//	HRESULT hresult = xMesh.Load(filePath, pMatCorrection);
	//	if (FAILED(hresult))
	//	{
	//		Assert(false);// xmesh load failed
	//		return false;
	//	}

	//	uint32 numBone;
	//	xMesh.GetTotalNumFrame(&numBone);
	//	Assert(numBone > 0);

	//	_skeleton._hierachy = new uint16[numBone];
	//	_skeleton._globalPoses = new Matrix[numBone];
	//	_skeleton._localPoses = new Matrix[numBone];
	//	_skeleton._offsetMatrices = new Matrix[numBone];
	//	_skeleton._names = new Skeleton::SkeletonName[numBone];
	//	_skeleton._renderGroups = new video::RenderGroupHandle[numBone];
	//	_skeleton._numhierachy = numBone;

	//	_skeleton._hierachy[0] = 0;
	//	MatrixIdentity(&_skeleton._globalPoses[0]);
	//	MatrixIdentity(&_skeleton._localPoses[0]);


	//	std::vector<std::string> texturePaths;
	//	for (uint32 i = 0; i < numBone; ++i)
	//	{
	//		_skeleton._hierachy[i] = xMesh._frameIndex[i];
	//		_skeleton._localPoses[i] = xMesh._linearFrames[i]->TransformationMatrix;

	//		if (nullptr != xMesh._linearFrames[i]->Name)
	//		{
	//			strncpy(_skeleton._names[i]._name, xMesh._linearFrames[i]->Name, 64);
	//		}
	//		else
	//		{
	//			char buffer[64] = {0,};
	//			sprintf(buffer, "Bone %d", i);
	//			strncpy(_skeleton._names[i]._name, buffer, 64);
	//		}

	//		if (nullptr != xMesh._linearFrames[i]->pMeshContainer)
	//		{
	//			ContainerEX *pContainerMesh = (ContainerEX *)xMesh._linearFrames[i]->pMeshContainer;
	//			if (pContainerMesh->_texturePaths.size() > 0)
	//			{
	//				memcpy(_skeleton._offsetMatrices, pContainerMesh->_pBoneOffsetMatices, sizeof(Matrix) * numBone);
	//				ExtractModelFromContainerEX(*pContainerMesh, this, i, filePath);
	//			}
	//		}
	//	}

	//	animation::UpdateSkeleton(&_skeleton, nullptr);

	//	xMesh.Release();
	//}

	//void Model::Destroy()
	//{
	//}

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
		_drawData._primitiveType = D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST;
	}

	

}