#ifndef VIDEO_RESOURCE_HANDLES_H
#define VIDEO_RESOURCE_HANDLES_H

#include "VideoDefines.h"

#include "Typedefs.h"

namespace video
{
	struct VideoResourceHandle
	{
		uint16 index;
		uint16 count;

		VideoResourceHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		VideoResourceHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
		uint32 Value() const
		{
			return (count << VIDEO_CONFIG_RESOURCE_COUNT_BIT) | index;
		}
		bool32 IsValid()
		{
			return (Value() != 0);
		}
		void MakeInvalid()
		{
			this->index = 0;
			this->count = 0;
		}

		bool operator== (const VideoResourceHandle &other) const
		{
			if (this->Value() == other.Value())
			{
				return true;
			}
			return false;
		}

		bool operator!= (const VideoResourceHandle &other) const
		{
			return !(*this == other);
		}
	};

	struct VertexBufferHandle : public VideoResourceHandle
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
	struct IndexBufferHandle : public VideoResourceHandle
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


	struct VertexDeclHandle : public VideoResourceHandle
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
	struct EffectHandle : public VideoResourceHandle
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

	struct TextureHandle : public VideoResourceHandle
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

	struct MaterialHandle : public VideoResourceHandle
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

	struct RenderViewHandle : public VideoResourceHandle
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

	struct ModelHandle : public VideoResourceHandle
	{
		ModelHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		ModelHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};

	struct FrameBufferHandle : public VideoResourceHandle
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
}
#endif