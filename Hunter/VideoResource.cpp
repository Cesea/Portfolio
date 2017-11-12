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

	void VertexDecl::Add(D3DVERTEXELEMENT9 & element)
	{
		memcpy(&_elements[_count++], &element, sizeof(D3DVERTEXELEMENT9));
	}

	void VertexDecl::End(uint32 stride)
	{
		D3DVERTEXELEMENT9 end = D3DDECL_END();
		memcpy(&_elements[_count], &end, sizeof(D3DVERTEXELEMENT9));

		_stride = stride;
		VIDEO_CHECK(gpDevice->CreateVertexDeclaration(_elements, &_ptr));
	}

	void VertexDecl::Destroy()
	{
		COM_RELEASE(_ptr);
	}

	bool Texture::Create(const std::string fileName)
	{
		D3DXIMAGE_INFO imageInfo;
		VIDEO_CHECK(D3DXGetImageInfoFromFile(fileName.c_str(), &imageInfo));

		VIDEO_CHECK(D3DXCreateTextureFromFile(gpDevice, fileName.c_str(), &_ptr));

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
}