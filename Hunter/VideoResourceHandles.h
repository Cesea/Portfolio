#ifndef VIDEO_RESOURCE_HANDLES_H
#define VIDEO_RESOURCE_HANDLES_H

#include "VideoDefines.h"

#include "Typedefs.h"

struct ResourceHandle
{
	uint16 index;
	uint16 count;

	ResourceHandle()
	{
		this->index = 0;
		this->count = 0;
	}

	ResourceHandle(const ResourceHandle &other)
	{
		this->index = other.index;
		this->count	= other.count;
	}

	const ResourceHandle &operator= (const ResourceHandle &other)
	{
		this->index = other.index;
		this->count	= other.count;
		return *this;
	}

	ResourceHandle(uint16 index, uint16 count)
	{
		this->index = index;
		this->count = count;
	}

	uint32 Value() const
	{
		return (count << VIDEO_CONFIG_RESOURCE_COUNT_BIT) | index;
	}

	bool32 IsValid() const
	{
		return (Value() != 0);
	}

	void MakeInvalid()
	{
		this->index = 0;
		this->count = 0;
	}

	bool operator== (const ResourceHandle &other) const
	{
		if (this->Value() == other.Value())
		{
			return true;
		}
		return false;
	}

	bool operator!= (const ResourceHandle &other) const
	{
		return !(*this == other);
	}
};


namespace video
{

	struct VertexBufferHandle : public ResourceHandle
	{
		VertexBufferHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		VertexBufferHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct IndexBufferHandle : public ResourceHandle
	{
		IndexBufferHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		IndexBufferHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};


	struct VertexDeclHandle : public ResourceHandle
	{
		VertexDeclHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		VertexDeclHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};
	struct EffectHandle : public ResourceHandle
	{
		EffectHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		EffectHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct TextureHandle : public ResourceHandle
	{
		TextureHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		TextureHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct MaterialHandle : public ResourceHandle
	{
		MaterialHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		MaterialHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct RenderViewHandle : public ResourceHandle
	{
		RenderViewHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		RenderViewHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct RenderGroupHandle : public ResourceHandle
	{
		RenderGroupHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		RenderGroupHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct FrameBufferHandle : public ResourceHandle
	{
		FrameBufferHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		FrameBufferHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct StaticXMeshHandle : public ResourceHandle
	{
		StaticXMeshHandle()
		{
			this->index = 0;
			this->count = 0;
		}

		StaticXMeshHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct SkinnedXMeshHandle : public ResourceHandle
	{
		SkinnedXMeshHandle()
		{
			this->index = 0;
			this->count = 0;
		}

		SkinnedXMeshHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct AnimationInstanceHandle : public ResourceHandle
	{
		AnimationInstanceHandle()
		{
			this->index = 0;
			this->count = 0;
		}

		AnimationInstanceHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};
	
	struct FontHandle : public ResourceHandle
	{
		FontHandle()
		{
			this->index = 0;
			this->count = 0;
		}

		FontHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};
}
#endif